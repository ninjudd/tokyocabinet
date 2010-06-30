(ns user
  (:use cake cake.ant)
  (:import [org.apache.tools.ant.taskdefs Copy ExecTask]))

(defn build [srcdir dest environment]
  (ant ExecTask {:dir (file srcdir) :executable "./configure"}
       (args [(str "--prefix=" dest)])
       (env environment))
  (ant ExecTask {:dir (file srcdir) :executable "make"})
  (ant ExecTask {:dir (file srcdir) :executable "make"} (args ["install"])))

(deftask compile-native
  (let [dest (file "build/native")]
    (when-not (.exists dest)
      (.mkdirs dest)
      (let [dest (.getPath dest)]
        (build "src/tokyocabinet-1.4.45" dest {})
        (build "src/tokyocabinet-java-1.23" dest
               {"JAVA_HOME" (System/getProperty "java.home")
                "CPPFLAGS"  (str "-I" dest "/include -L" dest "/lib")})
        (ant Copy {:todir (file "classes")}
             (add-zipfileset {:src (str dest "/lib/tokyocabinet.jar") :includes "**/*.class"}))))))

(defn clean [srcdir]
  (ant ExecTask {:dir (file srcdir) :executable "make"} (args ["clean"])))

(deftask clean
  (clean "src/tokyocabinet-1.4.45")
  (clean "src/tokyocabinet-java-1.23"))
