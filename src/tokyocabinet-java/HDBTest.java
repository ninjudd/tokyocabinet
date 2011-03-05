/*************************************************************************************************
 * The test cases of the hash database API
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
 * The test cases of the hash database API.
 */
class HDBTest {
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
    STDERR.println("test cases of the hash database API");
    STDERR.println("");
    STDERR.println("synopsis:");
    STDERR.println("  java " + HDBTest.class.getName() + " arguments...");
    STDERR.println("");
    STDERR.println("arguments:");
    STDERR.println("  tchtest.rb write [-tl] [-td|-tb] [-nl|-nb] [-as]" +
                   " path rnum [bnum [apow [fpow]]]");
    STDERR.println("  tchtest.rb read [-nl|-nb] path");
    STDERR.println("  tchtest.rb remove [-nl|-nb] path");
    STDERR.println("  tchtest.rb misc [-tl] [-td|-tb] [-nl|-nb] path rnum");
    STDERR.println("");
    System.exit(1);
  }
  /* print error message of hash database */
  private static void eprint(HDB hdb, String func){
    String path = hdb.path();
    STDERR.println(HDBTest.class.getName() + ": " + path + ": " +
                   func + ": " + hdb.errmsg());
  }
  /* parse arguments of write command */
  private static int runwrite(String[] args){
    String path = null;
    String rstr = null;
    String bstr = null;
    String astr = null;
    String fstr = null;
    int opts = 0;
    int omode = 0;
    boolean as = false;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(path == null && arg.startsWith("-")){
        if(arg.equals("-tl")){
          opts |= HDB.TLARGE;
        } else if(arg.equals("-td")){
          opts |= HDB.TDEFLATE;
        } else if(arg.equals("-tb")){
          opts |= HDB.TBZIP;
        } else if(arg.equals("-tt")){
          opts |= HDB.TTCBS;
        } else if(arg.equals("-nl")){
          omode |= HDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= HDB.OLCKNB;
        } else if(arg.equals("-as")){
          as = true;
        } else {
          usage();
        }
      } else if(path == null){
        path = arg;
      } else if(rstr == null){
        rstr = arg;
      } else if(bstr == null){
        bstr = arg;
      } else if(astr == null){
        astr = arg;
      } else if(fstr == null){
        fstr = arg;
      } else {
        usage();
      }
    }
    if(path == null || rstr == null) usage();
    int rnum = Util.atoi(rstr);
    if(rnum < 1) usage();
    int bnum = (bstr != null) ? Util.atoi(bstr) : -1;
    int apow = (astr != null) ? Util.atoi(astr) : -1;
    int fpow = (fstr != null) ? Util.atoi(fstr) : -1;
    int rv = procwrite(path, rnum, bnum, apow, fpow, opts, omode, as);
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
          omode |= HDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= HDB.OLCKNB;
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
          omode |= HDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= HDB.OLCKNB;
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
    int opts = 0;
    int omode = 0;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(path == null && arg.startsWith("-")){
        if(arg.equals("-tl")){
          opts |= HDB.TLARGE;
        } else if(arg.equals("-td")){
          opts |= HDB.TDEFLATE;
        } else if(arg.equals("-tb")){
          opts |= HDB.TBZIP;
        } else if(arg.equals("-tt")){
          opts |= HDB.TTCBS;
        } else if(arg.equals("-nl")){
          omode |= HDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= HDB.OLCKNB;
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
    int rv = procmisc(path, rnum, opts, omode);
    return rv;
  }
  /* perform write command */
  private static int procwrite(String path, int rnum, int bnum, int apow, int fpow, int opts,
                               int omode, boolean as){
    STDOUT.println("<Writing Test>");
    STDOUT.println("  path=" + path + "  rnum=" + rnum + "  bnum=" + bnum + "  apow=" + apow +
                   "  fpow=" + fpow + "  opts=" + opts + "  omode=" + omode + "  as=" + as);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    HDB hdb = new HDB();
    if(!hdb.tune(bnum, apow, fpow, opts)){
      eprint(hdb, "tune");
      err = true;
    }
    if(!hdb.open(path, HDB.OWRITER | HDB.OCREAT | HDB.OTRUNC | omode)){
      eprint(hdb, "open");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(as){
        if(!hdb.putasync(buf, buf)){
          eprint(hdb, "putasync");
          err = true;
          break;
        }
      } else {
        if(!hdb.put(buf, buf)){
          eprint(hdb, "put");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + hdb.rnum());
    STDOUT.println("size: " + hdb.fsiz());
    if(!hdb.close()){
      eprint(hdb, "close");
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
    HDB hdb = new HDB();
    if(!hdb.open(path, HDB.OREADER | omode)){
      eprint(hdb, "open");
      err = true;
    }
    int rnum = (int)hdb.rnum();
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(hdb.get(buf) == null){
        eprint(hdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + hdb.rnum());
    STDOUT.println("size: " + hdb.fsiz());
    if(!hdb.close()){
      eprint(hdb, "close");
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
    HDB hdb = new HDB();
    if(!hdb.open(path, HDB.OWRITER | omode)){
      eprint(hdb, "open");
      err = true;
    }
    int rnum = (int)hdb.rnum();
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!hdb.out(buf)){
        eprint(hdb, "out");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + hdb.rnum());
    STDOUT.println("size: " + hdb.fsiz());
    if(!hdb.close()){
      eprint(hdb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  /* perform misc command */
  private static int procmisc(String path, int rnum, int opts, int omode){
    STDOUT.println("<Miscellaneous Test>");
    STDOUT.println("  path=" + path + "  rnum=" + rnum + "  opts=" + opts + "  omode=" + omode);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    HDB hdb = new HDB();
    if(!hdb.tune(rnum / 50, 2, -1, opts)){
      eprint(hdb, "tune");
      err = true;
    }
    if(!hdb.setcache(rnum / 10)){
      eprint(hdb, "setcache");
      err = true;
    }
    if(!hdb.setxmsiz(rnum * 4)){
      eprint(hdb, "setxmsiz");
      err = true;
    }
    if(!hdb.setdfunit(8)){
      eprint(hdb, "setdfunit");
      err = true;
    }
    if(!hdb.open(path, HDB.OWRITER | HDB.OCREAT | HDB.OTRUNC | omode)){
      eprint(hdb, "open");
      err = true;
    }
    STDOUT.println("writing:");
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!hdb.put(buf, buf)){
        eprint(hdb, "put");
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
      if(hdb.get(buf) == null){
        eprint(hdb, "get");
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
      if(RND.nextInt(2) == 0 && !hdb.out(buf)){
        eprint(hdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("checking iterator:");
    if(!hdb.iterinit()){
      eprint(hdb, "iterinit");
      err = true;
    }
    int inum = 0;
    String key;
    while((key = hdb.iternext2()) != null){
      String value = hdb.get(key);
      if(value == null){
        eprint(hdb, "get");
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
    if(hdb.ecode() != HDB.ENOREC || inum != hdb.rnum()){
      eprint(hdb, "(validation)");
      err = true;
    }
    List keys = hdb.fwmkeys("0", 10);
    if(hdb.rnum() >= 10 && keys.size() != 10){
      eprint(hdb, "fwmkeys");
      err = true;
    }
    STDOUT.println("checking counting:");
    for(int i = 1; i <= rnum; i++){
      String buf = "[" + RND.nextInt(rnum) + "]";
      if(RND.nextInt(2) == 0){
        if(hdb.addint(buf, 1) == Integer.MIN_VALUE && hdb.ecode() != HDB.EKEEP){
          eprint(hdb, "addint");
          err = true;
          break;
        }
      } else {
        if(hdb.adddouble(buf, 1.0) == Integer.MIN_VALUE && hdb.ecode() != HDB.EKEEP){
          eprint(hdb, "adddouble");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!hdb.sync()){
      eprint(hdb, "sync");
      err = true;
    }
    if(!hdb.optimize()){
      eprint(hdb, "optimize");
      err = true;
    }
    String npath = path + "-tmp";
    if(!hdb.copy(npath)){
      eprint(hdb, "copy");
      err = true;
    }
    (new File(npath)).delete();
    if(!hdb.vanish()){
      eprint(hdb, "vanish");
      err = true;
    }
    STDOUT.println("checking transaction commit:");
    if(!hdb.tranbegin()){
      eprint(hdb, "tranbegin");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String buf = i + "";
      if(RND.nextInt(2) == 0){
        if(!hdb.putcat(buf, buf)){
          eprint(hdb, "putcat");
          err = true;
          break;
        }
      } else {
        if(!hdb.out(buf) && hdb.ecode() != HDB.ENOREC){
          eprint(hdb, "out");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!hdb.trancommit()){
      eprint(hdb, "trancommit");
      err = true;
    }
    STDOUT.println("checking transaction abort:");
    long ornum = hdb.rnum();
    long ofsiz = hdb.fsiz();
    if(!hdb.tranbegin()){
      eprint(hdb, "tranbegin");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String buf = i + "";
      if(RND.nextInt(2) == 0){
        if(!hdb.putcat(buf, buf)){
          eprint(hdb, "putcat");
          err = true;
          break;
        }
      } else {
        if(!hdb.out(buf) && hdb.ecode() != HDB.ENOREC){
          eprint(hdb, "out");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!hdb.tranabort()){
      eprint(hdb, "trancommit");
      err = true;
    }
    if(hdb.rnum() != ornum || hdb.fsiz() != ofsiz){
      eprint(hdb, "(validation)");
      err = true;
    }
    STDOUT.println("checking common interface:");
    DBM dbm = hdb;
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
      eprint(hdb, "iterinit");
      err = true;
    }
    while((key = dbm.iternext2()) != null){
      String value = dbm.get(key);
      if(value == null){
        eprint(hdb, "get");
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
    STDOUT.println("record number: " + hdb.rnum());
    STDOUT.println("size: " + hdb.fsiz());
    if(!hdb.close()){
      eprint(hdb, "close");
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
  private HDBTest() throws NoSuchMethodException {
    throw new NoSuchMethodException();
  }
}



/* END OF FILE */
