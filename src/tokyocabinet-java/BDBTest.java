/*************************************************************************************************
 * The test cases of the B+ tree database API
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
 * The test cases of the B+ tree database API.
 */
class BDBTest {
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
    STDERR.println("test cases of the B+ tree database API");
    STDERR.println("");
    STDERR.println("synopsis:");
    STDERR.println("  java " + BDBTest.class.getName() + " arguments...");
    STDERR.println("");
    STDERR.println("arguments:");
    STDERR.println("  tcbtest.rb write [-tl] [-td|-tb|-tt] [-nl|-nb] path rnum" +
                   " [lmemb [nmemb [bnum [apow [fpow]]]]]");
    STDERR.println("  tcbtest.rb read [-nl|-nb] path");
    STDERR.println("  tcbtest.rb remove [-nl|-nb] path");
    STDERR.println("  tcbtest.rb misc [-tl] [-td|-tb|-tt] [-nl|-nb] path rnum");
    STDERR.println("");
    System.exit(1);
  }
  /* print error message of hash database */
  private static void eprint(BDB bdb, String func){
    String path = bdb.path();
    STDERR.println(BDBTest.class.getName() + ": " + path + ": " +
                   func + ": " + bdb.errmsg());
  }
  /* parse arguments of write command */
  private static int runwrite(String[] args){
    String path = null;
    String rstr = null;
    String lmstr = null;
    String nmstr = null;
    String bstr = null;
    String astr = null;
    String fstr = null;
    int opts = 0;
    int omode = 0;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(path == null && arg.startsWith("-")){
        if(arg.equals("-tl")){
          opts |= BDB.TLARGE;
        } else if(arg.equals("-td")){
          opts |= BDB.TDEFLATE;
        } else if(arg.equals("-tb")){
          opts |= BDB.TBZIP;
        } else if(arg.equals("-tt")){
          opts |= BDB.TTCBS;
        } else if(arg.equals("-nl")){
          omode |= BDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= BDB.OLCKNB;
        } else {
          usage();
        }
      } else if(path == null){
        path = arg;
      } else if(rstr == null){
        rstr = arg;
      } else if(lmstr == null){
        lmstr = arg;
      } else if(nmstr == null){
        nmstr = arg;
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
    int lmemb = (lmstr != null) ? Util.atoi(lmstr) : -1;
    int nmemb = (nmstr != null) ? Util.atoi(nmstr) : -1;
    int bnum = (bstr != null) ? Util.atoi(bstr) : -1;
    int apow = (astr != null) ? Util.atoi(astr) : -1;
    int fpow = (fstr != null) ? Util.atoi(fstr) : -1;
    int rv = procwrite(path, rnum, lmemb, nmemb, bnum, apow, fpow, opts, omode);
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
          omode |= BDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= BDB.OLCKNB;
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
          omode |= BDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= BDB.OLCKNB;
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
          opts |= BDB.TLARGE;
        } else if(arg.equals("-td")){
          opts |= BDB.TDEFLATE;
        } else if(arg.equals("-tb")){
          opts |= BDB.TBZIP;
        } else if(arg.equals("-tt")){
          opts |= BDB.TTCBS;
        } else if(arg.equals("-nl")){
          omode |= BDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= BDB.OLCKNB;
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
  private static int procwrite(String path, int rnum, int lmemb, int nmemb,
                               int bnum, int apow, int fpow, int opts, int omode){
    STDOUT.println("<Writing Test>");
    STDOUT.println("  path=" + path + "  rnum=" + rnum + "  lmemb=" + lmemb +
                   "  nmemb=" + nmemb + "  bnum=" + bnum + "  apow=" + apow +
                   "  fpow=" + fpow + "  opts=" + opts + "  omode=" + omode);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    BDB bdb = new BDB();
    if(!bdb.tune(lmemb, nmemb, bnum, apow, fpow, opts)){
      eprint(bdb, "tune");
      err = true;
    }
    if(!bdb.open(path, BDB.OWRITER | BDB.OCREAT | BDB.OTRUNC | omode)){
      eprint(bdb, "open");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!bdb.put(buf, buf)){
        eprint(bdb, "put");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + bdb.rnum());
    STDOUT.println("size: " + bdb.fsiz());
    if(!bdb.close()){
      eprint(bdb, "close");
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
    BDB bdb = new BDB();
    if(!bdb.open(path, BDB.OREADER | omode)){
      eprint(bdb, "open");
      err = true;
    }
    int rnum = (int)bdb.rnum();
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(bdb.get(buf) == null){
        eprint(bdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + bdb.rnum());
    STDOUT.println("size: " + bdb.fsiz());
    if(!bdb.close()){
      eprint(bdb, "close");
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
    BDB bdb = new BDB();
    if(!bdb.open(path, BDB.OWRITER | omode)){
      eprint(bdb, "open");
      err = true;
    }
    int rnum = (int)bdb.rnum();
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!bdb.out(buf)){
        eprint(bdb, "out");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + bdb.rnum());
    STDOUT.println("size: " + bdb.fsiz());
    if(!bdb.close()){
      eprint(bdb, "close");
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
    BDB bdb = new BDB();
    if(!bdb.tune(10, 10, rnum / 50, 2, -1, opts)){
      eprint(bdb, "tune");
      err = true;
    }
    if(!bdb.setcache(128, 256)){
      eprint(bdb, "setcache");
      err = true;
    }
    if(!bdb.setxmsiz(rnum * 4)){
      eprint(bdb, "setxmsiz");
      err = true;
    }
    if(!bdb.setdfunit(8)){
      eprint(bdb, "setdfunit");
      err = true;
    }
    if(!bdb.open(path, BDB.OWRITER | BDB.OCREAT | BDB.OTRUNC | omode)){
      eprint(bdb, "open");
      err = true;
    }
    STDOUT.println("writing:");
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!bdb.put(buf, buf)){
        eprint(bdb, "put");
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
      if(bdb.get(buf) == null){
        eprint(bdb, "get");
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
      if(RND.nextInt(2) == 0 && !bdb.out(buf)){
        eprint(bdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("checking cursor:");
    BDBCUR cur = new BDBCUR(bdb);
    if(!cur.first() && bdb.ecode() != BDB.ENOREC){
      eprint(bdb, "cur::first");
      err = true;
    }
    int inum = 0;
    String key;
    while((key = cur.key2()) != null){
      String value = cur.val2();
      if(value == null){
        eprint(bdb, "cur::val");
        err = true;
      }
      cur.next();
      if(inum > 0 && rnum > 250 && inum % (rnum / 250) == 0){
        STDOUT.print('.');
        if(inum == rnum || inum % (rnum / 10) == 0)
          STDOUT.println(" (" + Util.itoa(inum, 8, '0') + ")");
      }
      inum++;
    }
    if(rnum > 250) STDOUT.println(" (" + Util.itoa(inum, 8, '0') + ")");
    if(bdb.ecode() != BDB.ENOREC || inum != bdb.rnum()){
      eprint(bdb, "(validation)");
      err = true;
    }
    List keys = bdb.fwmkeys("0", 10);
    if(bdb.rnum() >= 10 && keys.size() != 10){
      eprint(bdb, "fwmkeys");
      err = true;
    }
    STDOUT.println("checking counting:");
    for(int i = 1; i <= rnum; i++){
      String buf = "[" + RND.nextInt(rnum) + "]";
      if(RND.nextInt(2) == 0){
        if(bdb.addint(buf, 1) == Integer.MIN_VALUE && bdb.ecode() != BDB.EKEEP){
          eprint(bdb, "addint");
          err = true;
          break;
        }
      } else {
        if(bdb.adddouble(buf, 1.0) == Integer.MIN_VALUE && bdb.ecode() != BDB.EKEEP){
          eprint(bdb, "adddouble");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!bdb.sync()){
      eprint(bdb, "sync");
      err = true;
    }
    if(!bdb.optimize()){
      eprint(bdb, "optimize");
      err = true;
    }
    String npath = path + "-tmp";
    if(!bdb.copy(npath)){
      eprint(bdb, "copy");
      err = true;
    }
    (new File(npath)).delete();
    if(!bdb.vanish()){
      eprint(bdb, "vanish");
      err = true;
    }
    STDOUT.println("random writing:");
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(RND.nextInt(i), 8, '0');
      if(!bdb.putdup(buf, buf)){
        eprint(bdb, "putdup");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0)
          STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("cursor updating:");
    for(int i = 1; i <= rnum; i++){
      if(RND.nextInt(10) == 0){
        String buf = Util.itoa(RND.nextInt(rnum), 8, '0');
        cur.jump(buf);
        for(int j = 1; j <= 10; j++){
          key = cur.key2();
          if(key == null) break;
          if(RND.nextInt(3) == 0){
            cur.out();
          } else {
            int cpmode = BDBCUR.CPCURRENT + RND.nextInt(3);
            cur.put(buf, cpmode);
          }
          cur.next();
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0)
          STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!bdb.tranbegin()){
      eprint(bdb, "tranbegin");
      err = true;
    }
    bdb.putdup("::1", "1");
    bdb.putdup("::2", "2a");
    bdb.putdup("::2", "2b");
    bdb.putdup("::3", "3");
    cur.jump("::2");
    cur.put("2A", BDBCUR.CPCURRENT);
    cur.put("2-", BDBCUR.CPBEFORE);
    cur.put("2+", BDBCUR.CPCURRENT);
    cur.next();
    cur.next();
    cur.put("mid", BDBCUR.CPBEFORE);
    cur.put("2C", BDBCUR.CPAFTER);
    cur.prev();
    cur.out();
    List vals = bdb.getlist("::2");
    if(vals == null || vals.size() != 4){
      eprint(bdb, "getlist");
      err = true;
    }
    List pvals = new ArrayList();
    pvals.add("hop");
    pvals.add("step");
    pvals.add("jump");
    if(!bdb.putlist("::1", pvals)){
      eprint(bdb, "putlist");
      err = true;
    }
    if(!bdb.outlist("::1")){
      eprint(bdb, "outlist");
      err = true;
    }
    if(!bdb.trancommit()){
      eprint(bdb, "trancommit");
      err = true;
    }
    if(!bdb.tranbegin() || !bdb.tranabort()){
      eprint(bdb, "tranbegin");
      err = true;
    }
    STDOUT.println("checking common interface:");
    DBM dbm = bdb;
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
      eprint(bdb, "iterinit");
      err = true;
    }
    while((key = dbm.iternext2()) != null){
      String value = dbm.get(key);
      if(value == null){
        eprint(bdb, "get");
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
    STDOUT.println("record number: " + bdb.rnum());
    STDOUT.println("size: " + bdb.fsiz());
    if(!bdb.close()){
      eprint(bdb, "close");
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
  private BDBTest() throws NoSuchMethodException {
    throw new NoSuchMethodException();
  }
}



/* END OF FILE */
