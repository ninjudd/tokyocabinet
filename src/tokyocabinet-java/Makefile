# Makefile for Tokyo Cabinet for Java



#================================================================
# Setting Variables
#================================================================


# Generic settings
SHELL = /bin/sh

# Package information
PACKAGE = tokyocabinet-java
VERSION = 1.23
PACKAGEDIR = $(PACKAGE)-$(VERSION)
PACKAGETGZ = $(PACKAGE)-$(VERSION).tar.gz
LIBVER = 1
LIBREV = 1

# Targets
JARFILES = tokyocabinet.jar
JAVAFILES = DBM.java HDB.java BDB.java BDBCUR.java BDBCMP.java FDB.java TDB.java TDBQRY.java TDBQRYPROC.java ADB.java HDBTest.java BDBTest.java FDBTest.java TDBTest.java ADBTest.java Util.java Loader.java
LIBRARYFILES =  libjtokyocabinet.1.1.0.dylib libjtokyocabinet.1.dylib libjtokyocabinet.dylib libjtokyocabinet.jnilib
LIBOBJFILES = hdb.o bdb.o bdbcur.o fdb.o tdb.o tdbqry.o adb.o util.o myconf.o

# Install destinations
prefix = /Users/lance/code/tokyocabinet/build/native
exec_prefix = ${prefix}
datarootdir = ${prefix}/share
LIBDIR = ${exec_prefix}/lib
DESTDIR =

# Building configuration
JAVAC = /System/Library/Frameworks/JavaVM.framework/Versions/1.6.0/Home/bin/javac
JAVACFLAGS = -source 1.4 -d .
JAR = /System/Library/Frameworks/JavaVM.framework/Versions/1.6.0/Home/bin/jar
JAVAH = /System/Library/Frameworks/JavaVM.framework/Versions/1.6.0/Home/bin/javah
JAVADOC = /System/Library/Frameworks/JavaVM.framework/Versions/1.6.0/Home/bin/javadoc
JAVARUN = /System/Library/Frameworks/JavaVM.framework/Versions/1.6.0/Home/bin/java
CC = gcc
CPPFLAGS = -I. -I$(INCLUDEDIR) -L/Users/lance/include -L/usr/local/include -DNDEBUG -D_GNU_SOURCE=1 -I/System/Library/Frameworks/JavaVM.framework/Versions/1.6.0/Home/include -I/System/Library/Frameworks/JavaVM.framework/Versions/1.6.0/Home/include/mac
CFLAGS =  -m32 -I/Users/lance/code/tokyocabinet/build/native/include -L/Users/lance/code/tokyocabinet/build/native/lib  -std=c99 -Wall -fPIC -fsigned-char -O2
LDFLAGS = -L. -L$(LIBDIR) -L/Users/lance/lib -L/usr/local/lib
LIBS = -ltokyocabinet -lbz2 -lz -lpthread -lm -lc 
LDENV = LD_RUN_PATH=/lib:/usr/lib:$(LIBDIR):$(HOME)/lib:/usr/local/lib:$(LIBDIR):.
RUNENV = DYLD_LIBRARY_PATH=.:/lib:/usr/lib:$(LIBDIR):$(HOME)/lib:/usr/local/lib:$(LIBDIR)



#================================================================
# Suffix rules
#================================================================


.SUFFIXES :
.SUFFIXES : .c .o

.c.o :
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<



#================================================================
# Actions
#================================================================


all : $(JARFILES) $(LIBRARYFILES)
	@printf '\n'
	@printf '#================================================================\n'
	@printf '# Ready to install.\n'
	@printf '#================================================================\n'


clean :
	rm -rf $(JARFILES) $(LIBRARYFILES) $(LIBOBJFILES) \
	  *.o a.out *.class check.in check.out gmon.out leak.log \
	  casket casket-* casket.* *.wal *~ hoge moge


untabify :
	ls *.c *.h *.java | while read name ; \
	  do \
	    sed -e 's/\t/        /g' -e 's/ *$$//' $$name > $$name~; \
	    [ -f $$name~ ] && mv -f $$name~ $$name ; \
	  done


install :
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp -Rf $(JARFILES) $(DESTDIR)$(LIBDIR)
	cp -Rf $(LIBRARYFILES) $(DESTDIR)$(LIBDIR)
	@printf '\n'
	@printf '#================================================================\n'
	@printf '# Thanks for using Tokyo Cabinet for Java.\n'
	@printf '#================================================================\n'


uninstall :
	cd $(DESTDIR)$(LIBDIR) && rm -f $(JARFILES)
	cd $(DESTDIR)$(LIBDIR) && rm -f $(LIBRARYFILES)


dist :
	make untabify
	make distclean
	cd .. && tar cvf - $(PACKAGEDIR) | gzip -c > $(PACKAGETGZ)
	sync ; sync


distclean : clean
	rm -rf Makefile config.cache config.log config.status autom4te.cache


head : tokyocabinet.jar
	CLASSPATH=tokyocabinet.jar $(JAVAH) -jni \
	  tokyocabinet.HDB tokyocabinet.BDB tokyocabinet.BDBCUR \
	  tokyocabinet.FDB tokyocabinet.TDB tokyocabinet.TDBQRY \
	  tokyocabinet.ADB tokyocabinet.Util


check :
	rm -rf casket*
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.HDBTest write casket 10000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.HDBTest read casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.HDBTest remove casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.HDBTest misc casket 1000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar \
	  tokyocabinet.HDBTest write -tl -as -td casket 10000 10000 1 1
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.HDBTest read -nl casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.HDBTest remove -nb casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.HDBTest misc -tl -tb casket 1000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.BDBTest write casket 10000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.BDBTest read casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.BDBTest remove casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.BDBTest misc casket 1000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar \
	  tokyocabinet.BDBTest write -tl casket 10000 10 10 100 1 1
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.BDBTest read -nl casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.BDBTest remove -nb casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.BDBTest misc -tl -tb casket 1000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.FDBTest write casket 10000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.FDBTest read casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.FDBTest remove casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.FDBTest misc casket 1000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar \
	  tokyocabinet.TDBTest write -ip -is -in casket 10000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.TDBTest read casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.TDBTest remove casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.TDBTest misc casket 500
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar \
	  tokyocabinet.TDBTest write -tl -is -td casket 10000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.TDBTest read -nl casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.TDBTest remove -nb casket
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.TDBTest misc -tl -tb casket 500
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar \
	  tokyocabinet.ADBTest write "casket.tch#mode=wct" 10000
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.ADBTest read "casket.tch#mode=r"
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar tokyocabinet.ADBTest remove "casket.tch#mode=w"
	$(RUNENV) $(JAVARUN) -cp tokyocabinet.jar \
	  tokyocabinet.ADBTest misc "casket.tch#mode=wct" 1000
	rm -rf casket*
	@printf '\n'
	@printf '#================================================================\n'
	@printf '# Checking completed.\n'
	@printf '#================================================================\n'


doc :
	make docclean
	mkdir -p doc
	$(JAVADOC) -locale en -windowtitle tokyocabinet -overview overview.html \
	  -tag note:a:"Note:" -d doc -nodeprecated -nohelp -quiet *.java


docclean :
	rm -rf doc


.PHONY : all clean install casket check doc



#================================================================
# Building binaries
#================================================================


tokyocabinet.jar : $(JAVAFILES)
	$(JAVAC) $(JAVACFLAGS) $(JAVAFILES)
	$(JAR) cvf $@ tokyocabinet/*.class
	rm -rf tokyocabinet


libjtokyocabinet.so.$(LIBVER).$(LIBREV).0 : $(LIBOBJFILES)
	$(CC) $(CFLAGS) -shared -Wl,-soname,libjtokyocabinet.so.$(LIBVER) -o $@ \
	  $(LIBOBJFILES) $(LDFLAGS) $(LIBS)


libjtokyocabinet.so.$(LIBVER) : libjtokyocabinet.so.$(LIBVER).$(LIBREV).0
	ln -f -s libjtokyocabinet.so.$(LIBVER).$(LIBREV).0 $@


libjtokyocabinet.so : libjtokyocabinet.so.$(LIBVER).$(LIBREV).0
	ln -f -s libjtokyocabinet.so.$(LIBVER).$(LIBREV).0 $@


libjtokyocabinet.$(LIBVER).$(LIBREV).0.dylib : $(LIBOBJFILES)
	$(CC) $(CFLAGS) -dynamiclib -o $@ \
	  -install_name @loader_path/libjtokyocabinet.dylib \
	  -current_version $(LIBVER).$(LIBREV).0 -compatibility_version $(LIBVER) \
	  $(LIBOBJFILES) $(LDFLAGS) $(LIBS)


libjtokyocabinet.$(LIBVER).dylib : libjtokyocabinet.$(LIBVER).$(LIBREV).0.dylib
	ln -f -s libjtokyocabinet.$(LIBVER).$(LIBREV).0.dylib $@


libjtokyocabinet.dylib : libjtokyocabinet.$(LIBVER).$(LIBREV).0.dylib
	ln -f -s libjtokyocabinet.$(LIBVER).$(LIBREV).0.dylib $@


libjtokyocabinet.jnilib : libjtokyocabinet.dylib
	ln -f -s libjtokyocabinet.dylib $@


hdb.o : tokyocabinet_HDB.h

bdb.o : tokyocabinet_BDB.h

bdbcur.o : tokyocabinet_BDBCUR.h

fdb.o : tokyocabinet_FDB.h

tdb.o : tokyocabinet_TDB.h

tdbqry.o : tokyocabinet_TDBQRY.h

adb.o : tokyocabinet_ADB.h

util.o : tokyocabinet_Util.h

$(LIBOBJFILES) : myconf.h



# END OF FILE
