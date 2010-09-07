(ns user
  (:use cake cake.ant
        [useful :only [verify]]
        [cake.tasks.dependencies :only [os-name os-arch]])
  (:import [org.apache.tools.ant.taskdefs Copy ExecTask Replace]))

(def tokyocabinet      "tokyocabinet-1.4.45")
(def tokyocabinet-java "tokyocabinet-java-1.23")

(defn clean [srcdir]
  (when (.exists (file srcdir "Makefile"))
    (ant ExecTask {:dir (file srcdir) :executable "make"} (args "distclean"))))

(deftask clean
  (clean (file "src" tokyocabinet))
  (clean (file "src" tokyocabinet-java)))

(deftask compile-native #{clean}
  (log (format "Compiling tokyocabinet for %s/%s" (os-name) (os-arch)))
  (let [dest    (.getPath (file "build" "native"))
        prefix  (str "--prefix=" dest)
        classes (file "classes")
        arch    (case (os-arch) "x86_64" "-m64" "x86" "-m32" "")]
    (let [src (file "src" tokyocabinet)]
      (ant ExecTask {:executable "./configure" :dir src :failonerror true}
           (args prefix)
           (env {"CFLAGS" arch "LDFLAGS" arch}))
      (ant ExecTask {:executable "make" :dir src :failonerror true})
      (ant ExecTask {:executable "make" :dir src :failonerror true}
           (args "install")))
    (let [cflags (format " %s -I%s/include -L%s/lib " arch dest dest)
          src    (file "src" tokyocabinet-java)]
      (ant ExecTask {:executable "./configure" :dir src :failonerror true}
           (args prefix)
           (env {"JAVA_HOME" (System/getProperty "java.home") "CFLAGS" cflags}))
      (let [token "\nCFLAGS ="] ; hack because configure doesn't set CFLAGS correctly in Makefile
        (ant Replace {:file (file src "Makefile") :token token :value (str token cflags)}))
      (ant ExecTask {:executable "make" :dir src :failonerror true})
      (ant ExecTask {:executable "make" :dir src :failonerror true}
           (args "install")))
    (ant Copy {:todir classes}
         (add-zipfileset {:src (str dest "/lib/tokyocabinet.jar") :includes "**/*.class"}))))
