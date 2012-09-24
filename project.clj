(defproject tokyocabinet "1.24.6"
  :url "https://github.com/flatland/tokyocabinet"
  :license {:name "GNU LESSER GENERAL PUBLIC LICENSE - Version 2.1"
            :url "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html"}
  :min-lein-version "2.0.0"
  :description "native tokyo cabinet libraries"
  :profiles {:dev {:dependencies [[fs "1.1.2"]
                                  [me.raynes/conch "0.3.2"]]}}
  :omit-source true
  :eval-in :leiningen)
