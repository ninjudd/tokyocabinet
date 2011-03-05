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
 * The test cases of the table database API.
 */
class TDBTest {
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
    STDERR.println("  java " + TDBTest.class.getName() + " arguments...");
    STDERR.println("");
    STDERR.println("arguments:");
    STDERR.println("  tchtest.rb write [-tl] [-td|-tb] [-ip|-is|-in|-it|-if|-ix] [-nl|-nb] " +
                   " path rnum [bnum [apow [fpow]]]");
    STDERR.println("  tchtest.rb read [-nl|-nb] path");
    STDERR.println("  tchtest.rb remove [-nl|-nb] path");
    STDERR.println("  tchtest.rb misc [-tl] [-td|-tb] [-nl|-nb] path rnum");
    STDERR.println("");
    System.exit(1);
  }
  /* print error message of hash database */
  private static void eprint(TDB tdb, String func){
    String path = tdb.path();
    STDERR.println(TDBTest.class.getName() + ": " + path + ": " +
                   func + ": " + tdb.errmsg());
  }
  /* parse arguments of write command */
  private static int runwrite(String[] args){
    String path = null;
    String rstr = null;
    String bstr = null;
    String astr = null;
    String fstr = null;
    int opts = 0;
    int iflags = 0;
    int omode = 0;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(path == null && arg.startsWith("-")){
        if(arg.equals("-tl")){
          opts |= TDB.TLARGE;
        } else if(arg.equals("-td")){
          opts |= TDB.TDEFLATE;
        } else if(arg.equals("-tb")){
          opts |= TDB.TBZIP;
        } else if(arg.equals("-tt")){
          opts |= TDB.TTCBS;
        } else if(arg.equals("-ip")){
          iflags |= 1 << 0;
        } else if(arg.equals("-is")){
          iflags |= 1 << 1;
        } else if(arg.equals("-in")){
          iflags |= 1 << 2;
        } else if(arg.equals("-it")){
          iflags |= 1 << 3;
        } else if(arg.equals("-if")){
          iflags |= 1 << 4;
        } else if(arg.equals("-ix")){
          iflags |= 1 << 5;
        } else if(arg.equals("-nl")){
          omode |= TDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= TDB.OLCKNB;
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
    int rv = procwrite(path, rnum, bnum, apow, fpow, opts, iflags, omode);
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
          omode |= TDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= TDB.OLCKNB;
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
          omode |= TDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= TDB.OLCKNB;
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
          opts |= TDB.TLARGE;
        } else if(arg.equals("-td")){
          opts |= TDB.TDEFLATE;
        } else if(arg.equals("-tb")){
          opts |= TDB.TBZIP;
        } else if(arg.equals("-tt")){
          opts |= TDB.TTCBS;
        } else if(arg.equals("-nl")){
          omode |= TDB.ONOLCK;
        } else if(arg.equals("-nb")){
          omode |= TDB.OLCKNB;
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
                               int iflags, int omode){
    STDOUT.println("<Writing Test>");
    STDOUT.println("  path=" + path + "  rnum=" + rnum + "  bnum=" + bnum + "  apow=" + apow +
                   "  fpow=" + fpow + "  opts=" + opts + "  iflags=" + iflags +
                   "  omode=" + omode);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    TDB tdb = new TDB();
    if(!tdb.tune(bnum, apow, fpow, opts)){
      eprint(tdb, "tune");
      err = true;
    }
    if(!tdb.open(path, TDB.OWRITER | TDB.OCREAT | TDB.OTRUNC | omode)){
      eprint(tdb, "open");
      err = true;
    }
    if((iflags & (1 << 0)) != 0 && !tdb.setindex("", TDB.ITDECIMAL)){
      eprint(tdb, "setindex");
      err = true;
    }
    if((iflags & (1 << 1)) != 0 && !tdb.setindex("str", TDB.ITLEXICAL)){
      eprint(tdb, "setindex");
      err = true;
    }
    if((iflags & (1 << 2)) != 0 && !tdb.setindex("num", TDB.ITDECIMAL)){
      eprint(tdb, "setindex");
      err = true;
    }
    if((iflags & (1 << 3)) != 0 && !tdb.setindex("type", TDB.ITDECIMAL)){
      eprint(tdb, "setindex");
      err = true;
    }
    if((iflags & (1 << 4)) != 0 && !tdb.setindex("flag", TDB.ITTOKEN)){
      eprint(tdb, "setindex");
      err = true;
    }
    if((iflags & (1 << 5)) != 0 && !tdb.setindex("text", TDB.ITQGRAM)){
      eprint(tdb, "setindex");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      long id = tdb.genuid();
      Map cols = new HashMap();
      cols.put("str", new Long(id));
      cols.put("num", new Integer(RND.nextInt((int)id) + 1));
      cols.put("type", new Integer(RND.nextInt(32) + 1));
      StringBuffer sb = new StringBuffer();
      int num = RND.nextInt(5);
      int pt = 0;
      for(int j = 0; j < num; j++){
        pt += RND.nextInt(5) + 1;
        if(sb.length() > 0) sb.append(",");
        sb.append(pt);
      }
      if(sb.length() > 0){
        cols.put("flag", sb.toString());
        cols.put("text", sb.toString());
      }
      if(!tdb.put(new Long(id).toString().getBytes(), cols)){
        eprint(tdb, "put");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(id, 8, '0') + ")");
      }
    }
    STDOUT.println("record number: " + tdb.rnum());
    STDOUT.println("size: " + tdb.fsiz());
    if(!tdb.close()){
      eprint(tdb, "close");
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
    TDB tdb = new TDB();
    if(!tdb.open(path, TDB.OREADER | omode)){
      eprint(tdb, "open");
      err = true;
    }
    int rnum = (int)tdb.rnum();
    for(int i = 1; i <= rnum; i++){
      if(tdb.get(new Integer(i).toString()) == null){
        eprint(tdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("record number: " + tdb.rnum());
    STDOUT.println("size: " + tdb.fsiz());
    if(!tdb.close()){
      eprint(tdb, "close");
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
    TDB tdb = new TDB();
    if(!tdb.open(path, TDB.OWRITER | omode)){
      eprint(tdb, "open");
      err = true;
    }
    int rnum = (int)tdb.rnum();
    for(int i = 1; i <= rnum; i++){
      if(!tdb.out(new Integer(i).toString())){
        eprint(tdb, "out");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("record number: " + tdb.rnum());
    STDOUT.println("size: " + tdb.fsiz());
    if(!tdb.close()){
      eprint(tdb, "close");
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
    TDB tdb = new TDB();
    if(!tdb.tune(rnum / 50, 2, -1, opts)){
      eprint(tdb, "tune");
      err = true;
    }
    if(!tdb.setcache(rnum / 10, 128, 256)){
      eprint(tdb, "setcache");
      err = true;
    }
    if(!tdb.setxmsiz(rnum * 4)){
      eprint(tdb, "setxmsiz");
      err = true;
    }
    if(!tdb.setdfunit(8)){
      eprint(tdb, "setdfunit");
      err = true;
    }
    if(!tdb.open(path, TDB.OWRITER | TDB.OCREAT | TDB.OTRUNC | omode)){
      eprint(tdb, "open");
      err = true;
    }
    if(!tdb.setindex("", TDB.ITDECIMAL)){
      eprint(tdb, "setindex");
      err = true;
    }
    if(!tdb.setindex("str", TDB.ITLEXICAL)){
      eprint(tdb, "setindex");
      err = true;
    }
    if(!tdb.setindex("num", TDB.ITDECIMAL)){
      eprint(tdb, "setindex");
      err = true;
    }
    if(!tdb.setindex("type", TDB.ITDECIMAL)){
      eprint(tdb, "setindex");
      err = true;
    }
    if(!tdb.setindex("flag", TDB.ITTOKEN)){
      eprint(tdb, "setindex");
      err = true;
    }
    if(!tdb.setindex("text", TDB.ITQGRAM)){
      eprint(tdb, "setindex");
      err = true;
    }
    STDOUT.println("writing:");
    for(int i = 1; i <= rnum; i++){
      long id = tdb.genuid();
      Map cols = new HashMap();
      cols.put("str", new Long(id));
      cols.put("num", new Integer(RND.nextInt((int)id) + 1));
      cols.put("type", new Integer(RND.nextInt(32) + 1));
      StringBuffer sb = new StringBuffer();
      int num = RND.nextInt(5);
      int pt = 0;
      for(int j = 0; j < num; j++){
        pt += RND.nextInt(5) + 1;
        if(sb.length() > 0) sb.append(",");
        sb.append(pt);
      }
      if(sb.length() > 0){
        cols.put("flag", sb.toString());
        cols.put("text", sb.toString());
      }
      if(!tdb.put(new Long(id).toString().getBytes(), cols)){
        eprint(tdb, "put");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(id, 8, '0') + ")");
      }
    }
    STDOUT.println("reading:");
    for(int i = 1; i <= rnum; i++){
      if(tdb.get(new Integer(i).toString()) == null){
        eprint(tdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("removing:");
    for(int i = 1; i <= rnum; i++){
      if(RND.nextInt(2) == 0 && !tdb.out(new Integer(i).toString())){
        eprint(tdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("checking iterator:");
    if(!tdb.iterinit()){
      eprint(tdb, "iterinit");
      err = true;
    }
    int inum = 0;
    byte[] pkey;
    while((pkey = tdb.iternext()) != null){
      inum++;
      if(tdb.get(pkey) == null){
        eprint(tdb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && inum % (rnum / 250) == 0){
        STDOUT.print('.');
        if(inum == rnum || inum % (rnum / 10) == 0)
          STDOUT.println(" (" + Util.itoa(inum, 8, '0') + ")");
      }
    }
    if(rnum > 250) STDOUT.println(" (" + Util.itoa(inum, 8, '0') + ")");
    if(tdb.ecode() != TDB.ENOREC || inum != tdb.rnum()){
      eprint(tdb, "(validation)");
      err = true;
    }
    List keys = tdb.fwmkeys("1", 10);
    STDOUT.println("checking counting:");
    for(int i = 1; i <= rnum; i++){
      String buf = "i:" + RND.nextInt(rnum);
      if(RND.nextInt(2) == 0){
        if(tdb.addint(buf, 1) == Integer.MIN_VALUE){
          eprint(tdb, "addint");
          err = true;
          break;
        }
      } else {
        if(tdb.adddouble(buf, 1) == Double.NaN){
          eprint(tdb, "adddouble");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!tdb.sync()){
      eprint(tdb, "sync");
      err = true;
    }
    if(!tdb.optimize()){
      eprint(tdb, "optimize");
      err = true;
    }
    String npath = path + "-tmp";
    if(!tdb.copy(npath)){
      eprint(tdb, "copy");
      err = true;
    }
    File file = new File(npath).getAbsoluteFile();
    File dir = new File(file.getParent());
    String[] children = dir.list();
    for(int i = 0; i < children.length; i++){
      String child = children[i];
      if(child.indexOf(npath) >= 0 && child.indexOf(".idx.") >= 0) new File(child).delete();
    }
    file.delete();
    STDOUT.println("searching:");
    TDBQRY qry = new TDBQRY(tdb);
    String[] names = { "", "str", "num", "type", "flag", "text", "c1" };
    int ops[] = { TDBQRY.QCSTREQ, TDBQRY.QCSTRINC, TDBQRY.QCSTRBW, TDBQRY.QCSTREW,
                  TDBQRY.QCSTRAND, TDBQRY.QCSTROR, TDBQRY.QCSTROREQ, TDBQRY.QCSTRRX,
                  TDBQRY.QCNUMEQ, TDBQRY.QCNUMGT, TDBQRY.QCNUMGE, TDBQRY.QCNUMLT,
                  TDBQRY.QCNUMLE, TDBQRY.QCNUMBT, TDBQRY.QCNUMOREQ };
    int ftsops[] = { TDBQRY.QCFTSPH, TDBQRY.QCFTSAND, TDBQRY.QCFTSOR, TDBQRY.QCFTSEX };
    int types[] = { TDBQRY.QOSTRASC, TDBQRY.QOSTRDESC, TDBQRY.QONUMASC, TDBQRY.QONUMDESC };
    for(int i = 1; i <= rnum; i++){
      if(RND.nextInt(10) > 0) qry = new TDBQRY(tdb);
      int cnum = RND.nextInt(4);
      for(int j = 0; j < cnum; j++){
        String name = names[RND.nextInt(names.length)];
        int op = ops[RND.nextInt(ops.length)];
        if(RND.nextInt(10) == 0) op = ftsops[RND.nextInt(ftsops.length)];
        if(RND.nextInt(20) == 0) op |= TDBQRY.QCNEGATE;
        if(RND.nextInt(20) == 0) op |= TDBQRY.QCNOIDX;
        String expr = new Integer(RND.nextInt(i)).toString();
        if(RND.nextInt(10) == 0) expr += RND.nextInt(i);
        if(RND.nextInt(10) == 0) expr += RND.nextInt(i);
        qry.addcond(name, op, expr);
      }
      if(RND.nextInt(3) != 0){
        String name = names[RND.nextInt(names.length)];
        int type = types[RND.nextInt(types.length)];
        qry.setorder(name, type);
      }
      qry.setlimit(RND.nextInt(i), RND.nextInt(10));
      List res = qry.search();
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    qry = new TDBQRY(tdb);
    qry.addcond("", TDBQRY.QCSTRBW, "i:");
    qry.setorder("_num", TDBQRY.QONUMDESC);
    List ires = qry.search();
    int irnum = ires.size();
    int itnum = (int)tdb.rnum();
    class MyProc implements TDBQRYPROC {
      int icnt = 0;
      public int proc(byte[] key, Map cols){
        icnt += 1;
        cols.put("icnt", new Integer(icnt).toString());
        return QPPUT;
      }
    }
    MyProc proc = new MyProc();
    if(!qry.proc(proc)){
      eprint(tdb, "qry::proc");
      err = true;
    }
    qry.addcond("icnt", TDBQRY.QCNUMGT, "0");
    TDBQRY[] qrys = { qry, qry };
    List mures = qry.metasearch(qrys, TDBQRY.MSUNION);
    if(mures.size() != irnum){
      eprint(tdb, "qry::metasearch");
      err = true;
    }
    List mires = qry.metasearch(qrys, TDBQRY.MSISECT);
    if(mires.size() != irnum){
      eprint(tdb, "qry::metasearch");
      err = true;
    }
    List mdres = qry.metasearch(qrys, TDBQRY.MSDIFF);
    if(mdres.size() != 0){
      eprint(tdb, "qry::metasearch");
      err = true;
    }
    if(!qry.searchout()){
      eprint(tdb, "qry::searchout");
      err = true;
    }
    if(tdb.rnum() != itnum - irnum){
      eprint(tdb, "(validation)");
      err = true;
    }
    qry = new TDBQRY(tdb);
    qry.addcond("text", TDBQRY.QCSTRBW, "1");
    qry.setlimit(100, 1);
    Iterator it = qry.search().iterator();
    while(it.hasNext()){
      Map cols = tdb.get((byte[])it.next());
      if(cols != null){
        String[] texts = qry.kwic(cols, "text", -1, TDBQRY.KWMUBRCT);
        if(texts.length < 1){
          eprint(tdb, "(validation)");
          err = true;
          break;
        } else {
          for(int i = 0; i < texts.length; i++){
            if(texts[i].indexOf("1") < 0){
              eprint(tdb, "(validation)");
              err = true;
              break;
            }
          }
        }
      } else {
        eprint(tdb, "get");
        err = true;
        break;
      }
    }
    if(!tdb.vanish()){
      eprint(tdb, "vanish");
      err = true;
    }
    STDOUT.println("checking transaction commit:");
    if(!tdb.tranbegin()){
      eprint(tdb, "tranbegin");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      pkey = new Integer((RND.nextInt(rnum) + 1)).toString().getBytes();
      if(RND.nextInt(2) == 0){
        if(tdb.addint(pkey, 1) == Integer.MIN_VALUE){
          eprint(tdb, "addint");
          err = true;
          break;
        }
      } else {
        if(!tdb.out(pkey) && tdb.ecode() != TDB.ENOREC){
          eprint(tdb, "out");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!tdb.trancommit()){
      eprint(tdb, "trancommit");
      err = true;
    }
    STDOUT.println("checking transaction abort:");
    long ornum = tdb.rnum();
    long ofsiz = tdb.fsiz();
    if(!tdb.tranbegin()){
      eprint(tdb, "tranbegin");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String pkstr = new Integer((RND.nextInt(rnum) + 1)).toString();
      if(RND.nextInt(2) == 0){
        if(tdb.addint(pkstr, 1) == Integer.MIN_VALUE){
          eprint(tdb, "addint");
          err = true;
          break;
        }
      } else {
        if(!tdb.out(pkstr) && tdb.ecode() != TDB.ENOREC){
          eprint(tdb, "out");
          err = true;
          break;
        }
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!tdb.tranabort()){
      eprint(tdb, "tranabort");
      err = true;
    }
    if(tdb.rnum() != ornum || tdb.fsiz() != ofsiz){
      eprint(tdb, "(validation)");
      err = true;
    }
    STDOUT.println("record number: " + tdb.rnum());
    STDOUT.println("size: " + tdb.fsiz());
    if(!tdb.close()){
      eprint(tdb, "close");
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
  private TDBTest() throws NoSuchMethodException {
    throw new NoSuchMethodException();
  }
}



/* END OF FILE */
