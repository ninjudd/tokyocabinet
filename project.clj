(defproject tokyocabinet "1.23-SNAPSHOT"
  :resources-path "jar"
  :description "native tokyo cabinet libraries")

(ns leiningen.jar
  (:require leiningen.jar))

(defn jar
  ([project jar-name]
     (let [jar-path (get-jar-filename project jar-name)
           filespecs [{:type :bytes
                       :path (format "meta-inf/maven/%s/%s/pom.xml"
                                     (:group project)
                                     (:name project))
                       :bytes (make-pom project)}
                      {:type :bytes
                       :path (format "meta-inf/maven/%s/%s/pom.properties"
                                     (:group project)
                                     (:name project))
                       :bytes (make-pom-properties project)}
                      (when (and (:resources-path project)
                                 (.exists (file (:resources-path project))))
                        {:type :path :path (:resources-path project)})]]
       (write-jar project jar-path filespecs)
       (println "Created" jar-path)
       jar-path))
  ([project] (jar project (str (:name project) "-" (:version project) ".jar"))))
