(ns tasks
  (:use [cake.core :only [deftask]]
        [cake.file :only [file rm rmdir]]
        [bake.core :only [log os-name os-arch]]
        [uncle.core :only [ant args env add-zipfileset]])
  (:import [org.apache.tools.ant.taskdefs Copy ExecTask Replace]))

(def tokyocabinet      "tokyocabinet")
(def tokyocabinet-java "tokyocabinet-java")

(defn clean [srcdir]
  (when (.exists (file srcdir "Makefile"))
    (ant ExecTask {:dir (file srcdir) :executable "make"} (args "distclean"))))

(deftask clean
  (clean (file "src" tokyocabinet))
  (clean (file "src" tokyocabinet-java)))

(defn fix-install-path [src lib]
  (case (os-name)
        "macosx"
        (ant Replace {:file (file src "Makefile")
                      :token (str "-install_name $(LIBDIR)/lib" lib ".$(LIBVER).dylib")
                      :value (str "-install_name @loader_path/lib" lib ".dylib")})
        "linux"
        (let [token "\nLDFLAGS ="
              rpath "-Wl,-R,'$$ORIGIN'"]
          (ant Replace {:file (file src "Makefile") :token token :value (str token " " rpath)}))))

(deftask compile-native
  (let [os-name (os-name)
        os-arch (os-arch)
        dest    (file "build" "native" os-name os-arch)]
    (when-not (.exists dest)
      (log (format "Compiling tokyocabinet for %s/%s" os-name os-arch))
      (let [dest    (.getPath dest)
            prefix  (str "--prefix=" dest)
            classes (file "classes")
            arch    (case os-arch "x86_64" "-m64" "x86" "-m32" "")]
        (let [src (file "src" tokyocabinet)]
          (ant ExecTask {:executable "./configure" :dir src :failonerror true}
               (args prefix)
               (env {"CFLAGS" arch "LDFLAGS" arch}))
          (fix-install-path src "tokyocabinet")
          (ant ExecTask {:executable "make" :dir src :failonerror true})
          (ant ExecTask {:executable "make" :dir src :failonerror true}
               (args "install")))
        (let [cflags (format " %s -I%s/include -L%s/lib " arch dest dest)
              src    (file "src" tokyocabinet-java)]
          (ant ExecTask {:executable "./configure" :dir src :failonerror true}
               (args prefix)
               (env {"JAVA_HOME" (or (System/getenv "JAVA_HOME")
                                     (System/getProperty "java.home"))
                     "CFLAGS" cflags}))
          (let [token "\nCFLAGS ="] ; hack because configure doesn't set CFLAGS correctly in Makefile
            (ant Replace {:file (file src "Makefile") :token token :value (str token " " cflags)}))
          (fix-install-path src "jtokyocabinet")
          (ant ExecTask {:executable "make" :dir src :failonerror true})
          (ant ExecTask {:executable "make" :dir src :failonerror true}
               (args "install")))
        (ant Copy {:todir classes}
             (add-zipfileset {:src (str dest "/lib/tokyocabinet.jar") :includes "**/*.class"}))
        (rm (file dest "lib/tokyocabinet.jar"))
        (rmdir (file dest "lib/pkgconfig"))))))
