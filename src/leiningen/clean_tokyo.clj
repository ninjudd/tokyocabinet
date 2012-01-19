(ns leiningen.clean-tokyo
  (:use [clojure.java.shell :only [sh]]
        [clojure.java.io :only [file]]))

(defn clean [srcdir]
  (when (.exists (file srcdir "Makefile"))
    (println (:out (sh "make" "distclean" :dir (file srcdir))))))

(defn clean-tokyo []
  (clean (file "src/tokyocabinet"))
  (clean (file "src/tokyocabinet-java")))

