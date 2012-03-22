(ns leiningen.compile-native
  (:use [leiningen.core.eval :only [get-os]]
        [clojure.java.io :only [copy file]])
  (:require [fs.core :as fs]
            [clojure.string :as string]
            [conch.core :as sh])
  (:import java.util.zip.ZipFile))

(defn replace-text [f re text]
  (spit f (string/replace (slurp f) re text)))

(defn fix-install-path [os-name src lib]
  (case os-name
    "macosx"
    (replace-text (file src "Makefile")
                  (str "-install_name $(LIBDIR)/lib" lib ".$(LIBVER).dylib")
                  (str "-install_name @loader_path/lib" lib ".dylib"))
    "linux"
    (let [token "\nLDFLAGS ="
          rpath "-Wl,-R,'$$ORIGIN'"]
      (replace-text (file src "Makefile") token (str token " " rpath)))))

;; This was copied from Leiningen (and heavily modified). Probably want to move this to fs.
(defn copy-entries
  [in out pred]
  (let [zip (ZipFile. in)]
    (doseq [zfile (enumeration-seq (.entries zip))
            :let [filename (.getName zfile)]
            :when (pred filename)]
      (let [out-file (file out filename)]
        (.setCompressedSize zfile -1) ; some jars report size incorrectly
        (.mkdirs (.getParentFile out-file))
        (copy (.getInputStream zip zfile) out-file)))))

(defn configure [src target env]
  (fs/chmod "+x" (file src "configure"))
  (sh/stream-to-out
   (sh/proc "./configure" (str "--prefix=" target)
            :dir src :env env)
   :out))

(defn make [src & params]
  (sh/stream-to-out
   (apply sh/proc "make"
          (concat params [:dir src]))
   :out))

(defn make-native [target os arch-flag]
  (let [src (file target "tokyocabinet-native")]
    (configure src target {"CFLAGS" arch-flag "LDFLAGS" arch-flag})
    (fix-install-path os src "tokyocabinet-native")
    (make src "-j")
    (make src "install")))

(defn make-java [target os arch-flag]
  (let [src    (file target "tokyocabinet")
        cflags (format " %s -I%s/include -L%s/lib " arch-flag target target)]
    (configure src target {"JAVA_HOME" (or (System/getenv "JAVA_HOME")
                                           (System/getProperty "java.home"))
                           "CFLAGS" cflags})
    (let [token "\nCFLAGS ="] ; hack because configure doesn't set CFLAGS correctly in Makefile
      (replace-text (file src "Makefile") token (str token " " cflags)))
    (fix-install-path os src "jtokyocabinet")
    (make src)
    (make src "install")))

(defn compile-tokyo [target os arch-flag]
  ;; copy source so we can compile for multiple architectures
  (fs/copy-dir (file "src") target)
  (make-native target os arch-flag)
  (make-java   target os arch-flag))

(defn compile-native [project]
  (doseq [[arch arch-flag] {"x86_64" "-m64", "x86" "-m32"}]
    (let [os     (name (get-os))
          target (file (:target-path project) os arch)
          dest   (file "resources" "native" os arch)]
      (println (format "Compiling tokyocabinet for %s/%s" os arch))
      (compile-tokyo target os arch-flag)
      ;; we can't cross-compile yet, so copy lib and classes into resources/ so we can commit them
      (fs/delete-dir dest)
      (fs/copy-dir (file target "lib") dest)
      (copy-entries (file target "lib" "tokyocabinet.jar") (file "resources")
                    #(.endsWith (str %) ".class")))))