Native [Tokyo Cabinet](http://1978th.net/tokyocabinet) libraries for Java and Clojure.

# Compiling

If you have any problems with `lein compile-native`, make sure that
`JAVA_HOME` is set properly: it should point at a JDK, not a JRE.

To cross-compile from 64-bit Linux to 32-bit, you will need to first:

    # apt-get install gcc-multilib lib32z1-dev lib32bz2-dev
