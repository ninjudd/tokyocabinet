(ns leiningen.clean-tokyo
  (:use [clojure.java.io :only [file]])
  (:require [conch.core :as sh]))

(defn clean [srcdir]
  (when (.exists (file srcdir "Makefile"))
    (sh/stream-to-out (sh/proc "make" "distclean" :dir (file srcdir)) :out)))

(defn clean-tokyo [_]
  (clean (file "src/tokyocabinet-native"))
  (clean (file "src/tokyocabinet")))

