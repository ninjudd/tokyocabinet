/*************************************************************************************************
 * The test cases of the fixed-length database API
 *                                                               Copyright (C) 2006-2010 FAL Labs
 * This file is part of Tokyo Cabinet.
 * Tokyo Cabinet is free software; you can redistribute it and/or modify it under the terms of
 * the GNU Lesser General Public License as published by the Free Software Foundation; either
 * version 2.1 of the License or any later version.  Tokyo Cabinet is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with Tokyo
 * Cabinet; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 *************************************************************************************************/


package tokyocabinet;

import java.util.*;
import java.io.*;
import java.net.*;



/**
 * The test cases of the fixed-length database API.
 */
class FDBTest {
  //----------------------------------------------------------------
  // private constants
  //----------------------------------------------------------------
  private static final PrintStream STDOUT = System.out;
  private static final PrintStream STDERR = System.err;
  private static final Random RND = new Random();
  //----------------------------------------------------------------
  // public static methods
  //----------------------------------------------------------------
  /**
   * Main routine of the test command.
   */
  public static void main(String[] args){
    int rv = 0;
    if(args.length < 1) usage();
    if(args[0].equals("write")){
      rv = runwrite(args);
    } else if(args[0].equals("read")){
      rv = runread(args);
    } else if(args[0].equals("remove")){
      rv = runremove(args);
    } else if(args[0].equals("misc")){
      rv = runmisc(args);
    } else {
      usage();
    }
    System.gc();
    System.exit(rv);
  }
  //----------------------------------------------------------------
  // private static methods
  //----------------------------------------------------------------
  /* print the usage and exit */
  private static void usage(){
    STDERR.println("test cases of the fixed-length database API");
    STDERR.println("");
    STDERR.println("synopsis:");
    STDERR.println("  java " + FDBTest.class.getName() + " arguments...");
    STDERR.println("");
    STDERR.println("arguments:");
    STDERR.println("  tchtest.rb write [-nl|-nb] path rnum [width [limsiz]]");
    STDERR.println("  tchtest.rb read [-nl|-nb] path");
    STDERR.println("  tchtest.rb remove [-nl|-nb] path");
    STDERR.println("  tchtest.rb misc [-nl|-nb] path rnum");
    STDERR.println("");
    System.exit(1);
  }
  /* print error message of fixed-length database */
  private static void eprint(FDB fdb, String func){
    String path = fdb.path();
    STDERR.println(FDBTest.class.getName() + ": " + path + ": " +
                   func + ": " + fdb.errmsg());
  }
  /* parse arguments of write command */
  private static int runwrite(String[] args){
    String path = null;
    String rstr = null;
    String wstr = null;
    String lstr = null;
    int omode = 0;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(path == null && arg.startsWith("-")){
        if(arg.equals("-nl")){
          omode |= FDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= FDB.OLCKNB;
        } else {
          usage();
        }
      } else if(path == null){
        path = arg;
      } else if(rstr == null){
        rstr = arg;
      } else if(wstr == null){
        wstr = arg;
      } else if(lstr == null){
        lstr = arg;
      } else {
        usage();
      }
    }
    if(path == null || rstr == null) usage();
    int rnum = Util.atoi(rstr);
    if(rnum < 1) usage();
    int width = (wstr != null) ? Util.atoi(wstr) : -1;
    long limsiz = (lstr != null) ? Util.atol(lstr) : -1;
    int rv = procwrite(path, rnum, width, limsiz, omode);
    return rv;
  }
  /* parse arguments of read command */
  private static int runread(String[] args){
    String path = null;
    int omode = 0;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(path == null && arg.startsWith("-")){
        if(arg.equals("-nl")){
          omode |= FDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= FDB.OLCKNB;
        } else {
          usage();
        }
      } else if(path == null){
        path = arg;
      } else {
        usage();
      }
    }
    if(path == null) usage();
    int rv = procread(path, omode);
    return rv;
  }
  /* parse arguments of remove command */
  private static int runremove(String[] args){
    String path = null;
    int omode = 0;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(path == null && arg.startsWith("-")){
        if(arg.equals("-nl")){
          omode |= FDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= FDB.OLCKNB;
        } else {
          usage();
        }
      } else if(path == null){
        path = arg;
      } else {
        usage();
      }
    }
    if(path == null) usage();
    int rv = procremove(path, omode);
    return rv;
  }
  /* parse arguments of misc command */
  private static int runmisc(String[] args){
    String path = null;
    String rstr = null;
    int omode = 0;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(path == null && arg.startsWith("-")){
        if(arg.equals("-nl")){
          omode |= FDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= FDB.OLCKNB;
        } else {
          usage();
        }
      } else if(path == null){
        path = arg;
      } else if(rstr == null){
        rstr = arg;
      } else {
        usage();
      }
    }
    if(path == null || rstr == null) usage();
    int rnum = Util.atoi(rstr);
    if(rnum < 1) usage();
    int rv = procmisc(path, rnum, omode);
    return rv;
  }
  /* perform write command */
  private static int procwrite(String path, int rnum, int width, long limsiz, int omode){
    STDOUT.println("<Writing Test>");
    STDOUT.println("  path=" + path + "  rnum=" + rnum +
                   "  width=" + width + "  limsiz=" + limsiz + "  omode=" + omode);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    FDB fdb = new FDB();
    if(!fdb.tune(width, limsiz)){
      eprint(fdb, "tune");
      err = true;
    }
    if(!fdb.open(path, FDB.OWRITER | FDB.OCREAT | FDB.OTRUNC | omode)){
      eprint(fdb, "open");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!fdb.put(buf, buf)){
        eprint(fdb, "put");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + fdb.rnum());
    STDOUT.println("size: " + fdb.fsiz());
    if(!fdb.close()){
      eprint(fdb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  /* perform read command */
  private static int procread(String path, int omode){
    STDOUT.println("<Reading Test>");
    STDOUT.println("  path=" + path + "  omode=" + omode);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    FDB fdb = new FDB();
    if(!fdb.open(path, FDB.OREADER | omode)){
      eprint(fdb, "open");
      err = true;
    }
    int rnum = (int)fdb.rnum();
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(fdb.get(buf) == null){
        eprint(fdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + fdb.rnum());
    STDOUT.println("size: " + fdb.fsiz());
    if(!fdb.close()){
      eprint(fdb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  /* perform remove command */
  private static int procremove(String path, int omode){
    STDOUT.println("<Removing Test>");
    STDOUT.println("  path=" + path + "  omode=" + omode);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    FDB fdb = new FDB();
    if(!fdb.open(path, FDB.OWRITER | omode)){
      eprint(fdb, "open");
      err = true;
    }
    int rnum = (int)fdb.rnum();
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!fdb.out(buf)){
        eprint(fdb, "out");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + fdb.rnum());
    STDOUT.println("size: " + fdb.fsiz());
    if(!fdb.close()){
      eprint(fdb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  /* perform misc command */
  private static int procmisc(String path, int rnum, int omode){
    STDOUT.println("<Miscellaneous Test>");
    STDOUT.println("  path=" + path + "  rnum=" + rnum + "  omode=" + omode);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    FDB fdb = new FDB();
    if(!fdb.tune(10, 1024 + 32 * rnum)){
      eprint(fdb, "tune");
      err = true;
    }
    if(!fdb.open(path, FDB.OWRITER | FDB.OCREAT | FDB.OTRUNC | omode)){
      eprint(fdb, "open");
      err = true;
    }
    STDOUT.println("writing:");
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!fdb.put(buf, buf)){
        eprint(fdb, "put");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("reading:");
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(fdb.get(buf) == null){
        eprint(fdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("removing:");
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(RND.nextInt(2) == 0 && !fdb.out(buf)){
        eprint(fdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("checking iterator:");
    if(!fdb.iterinit()){
      eprint(fdb, "iterinit");
      err = true;
    }
    int inum = 0;
    String key;
    while((key = fdb.iternext2()) != null){
      String value = fdb.get(key);
      if(value == null){
        eprint(fdb, "get");
        err = true;
      }
      if(inum > 0 && rnum > 250 && inum % (rnum / 250) == 0){
        STDOUT.print('.');
        if(inum == rnum || inum % (rnum / 10) == 0)
          STDOUT.println(" (" + Util.itoa(inum, 8, '0') + ")");
      }
      inum++;
    }
    if(rnum > 250) STDOUT.println(" (" + Util.itoa(inum, 8, '0') + ")");
    if(fdb.ecode() != FDB.ENOREC || inum != fdb.rnum()){
      eprint(fdb, "(validation)");
      err = true;
    }
    List keys = fdb.range("[min,max]", 10);
    if(fdb.rnum() >= 10 && keys.size() != 10){
      eprint(fdb, "range");
      err = true;
    }
    STDOUT.println("checking counting:");
    for(int i = 1; i <= rnum; i++){
      String buf = "[" + (RND.nextInt(rnum) + 1) + "]";
      if(RND.nextInt(2) == 0){
        if(fdb.addint(buf, 1) == Integer.MIN_VALUE && fdb.ecode() != FDB.EKEEP){
          eprint(fdb, "addint");
          err = true;
          break;
        }
      } else {
        if(fdb.adddouble(buf, 1.0) == Integer.MIN_VALUE && fdb.ecode() != FDB.EKEEP){
          eprint(fdb, "adddouble");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!fdb.sync()){
      eprint(fdb, "sync");
      err = true;
    }
    if(!fdb.optimize()){
      eprint(fdb, "optimize");
      err = true;
    }
    String npath = path + "-tmp";
    if(!fdb.copy(npath)){
      eprint(fdb, "copy");
      err = true;
    }
    (new File(npath)).delete();
    if(!fdb.vanish()){
      eprint(fdb, "vanish");
      err = true;
    }
    STDOUT.println("checking transaction commit:");
    if(!fdb.tranbegin()){
      eprint(fdb, "tranbegin");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String buf = i + "";
      if(RND.nextInt(2) == 0){
        if(!fdb.putcat(buf, buf)){
          eprint(fdb, "putcat");
          err = true;
          break;
        }
      } else {
        if(!fdb.out(buf) && fdb.ecode() != FDB.ENOREC){
          eprint(fdb, "out");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!fdb.trancommit()){
      eprint(fdb, "trancommit");
      err = true;
    }
    STDOUT.println("checking transaction abort:");
    long ornum = fdb.rnum();
    long ofsiz = fdb.fsiz();
    if(!fdb.tranbegin()){
      eprint(fdb, "tranbegin");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String buf = i + "";
      if(RND.nextInt(2) == 0){
        if(!fdb.putcat(buf, buf)){
          eprint(fdb, "putcat");
          err = true;
          break;
        }
      } else {
        if(!fdb.out(buf) && fdb.ecode() != FDB.ENOREC){
          eprint(fdb, "out");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!fdb.tranabort()){
      eprint(fdb, "trancommit");
      err = true;
    }
    if(fdb.rnum() != ornum || fdb.fsiz() != ofsiz){
      eprint(fdb, "(validation)");
      err = true;
    }
    STDOUT.println("checking common interface:");
    DBM dbm = fdb;
    for(int i = 1; i <= rnum; i++){
      String buf = "[" + Util.itoa(RND.nextInt(rnum), 8, '0') + "]";
      int rnd = RND.nextInt(3);
      if(rnd == 0){
        dbm.put(buf, buf);
      } else if(rnd == 1){
        dbm.get(buf);
      } else {
        dbm.out(buf);
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0)
          STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("checking iterator:");
    inum = 0;
    if(!dbm.iterinit()){
      eprint(fdb, "iterinit");
      err = true;
    }
    while((key = dbm.iternext2()) != null){
      String value = dbm.get(key);
      if(value == null){
        eprint(fdb, "get");
        err = true;
      }
      if(inum > 0 && rnum > 250 && inum % (rnum / 250) == 0){
        STDOUT.print('.');
        if(inum == rnum || inum % (rnum / 10) == 0)
          STDOUT.println(" (" + Util.itoa(inum, 8, '0') + ")");
      }
      inum++;
    }
    if(rnum > 250) STDOUT.println(" (" + Util.itoa(inum, 8, '0') + ")");
    STDOUT.println("record number: " + fdb.rnum());
    STDOUT.println("size: " + fdb.fsiz());
    if(!fdb.close()){
      eprint(fdb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  //----------------------------------------------------------------
  // constructors and finalizers
  //----------------------------------------------------------------
  /**
   * Dummy constructor.
   */
  private FDBTest() throws NoSuchMethodException {
    throw new NoSuchMethodException();
  }
}



/* END OF FILE */
