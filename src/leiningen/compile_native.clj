(ns leiningen.compile-native
  (:use [leiningen.core.eval :only [get-os get-arch]]
        [clojure.java.io :only [copy file]])
  (:require [fs.core :as fs]
            [clojure.string :as string]
            [conch.core :as sh])
  (:import java.util.zip.ZipFile))

(defn replace-text [f re text]
  (spit f (string/replace (slurp f) re text)))

(defn fix-install-path [src lib]
  (case (get-os)
        :macosx
        (replace-text (file src "Makefile")
                      (str "-install_name $(LIBDIR)/lib" lib ".$(LIBVER).dylib")
                      (str "-install_name @loader_path/lib" lib ".dylib"))
        :linux
        (let [token "\nLDFLAGS ="
              rpath "-Wl,-R,'$$ORIGIN'"]
          (replace-text (file src "Makefile") token (str token " " rpath)))))

;; This was copied from Leiningen (and heavily modified). Probably want
;; to move this to fs.
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

(defn compile-native [project]
  (let [os-name (name (get-os))
        os-arch (name (get-arch))
        dest    (fs/file (:target-path project) "native" os-name os-arch)]
    (when-not (.exists dest)
      (println (format "Compiling tokyocabinet for %s/%s" os-name os-arch))
      (let [prefix  (str "--prefix=" dest)
            classes (file "classes")
            arch    (case os-arch :x86_64 "-m64" :x86 "-m32" "")
            src     (file "src" "tokyocabinet")]
        (sh/stream-to-out 
          (sh/proc "./configure" prefix :dir src :env {"CFLAGS" arch "LDFLAGS" arch})
          :out) 
        (fix-install-path src "tokyocabinet")
        (sh/stream-to-out (sh/proc "make" "-j" :dir src) :out)
        (sh/stream-to-out (sh/proc "make" "install" :dir src):out )
        (let [cflags (format " %s -I%s/include -L%s/lib " arch dest dest)
              src    (file "src" "tokyocabinet")]
          (sh/stream-to-out (sh/proc "./configure" prefix 
                                     :dir src 
                                     :env {"JAVA_HOME" (or (System/getenv "JAVA_HOME")
                                                           (System/getProperty "java.home"))
                                           "CFLAGS" cflags})
                            :out) 
          (let [token "\nCFLAGS ="] ; hack because configure doesn't set CFLAGS correctly in Makefile
            (replace-text (file src "Makefile") token (str token " " cflags)))
          (fix-install-path src "jtokyocabinet")
          (sh/stream-to-out (sh/proc "make" :dir src) :out)
          (sh/stream-to-out (sh/proc "make" "install" :dir src) :out))
        (copy-entries (str dest "/lib/tokyocabinet.jar") classes #(.endsWith (str %) ".class"))
        (fs/delete (file dest "lib/tokyocabinet.jar"))
        (fs/delete-dir (file dest "lib/pkgconfig"))))))
