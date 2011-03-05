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
class ADBTest {
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
    STDERR.println("test cases of the abstract database API");
    STDERR.println("");
    STDERR.println("synopsis:");
    STDERR.println("  java " + ADBTest.class.getName() + " arguments...");
    STDERR.println("");
    STDERR.println("arguments:");
    STDERR.println("  tcatest.rb write name rnum");
    STDERR.println("  tcatest.rb read name");
    STDERR.println("  tcatest.rb remove name");
    STDERR.println("  tcatest.rb misc name rnum");
    STDERR.println("");
    System.exit(1);
  }
  /* print error message of hash database */
  private static void eprint(ADB adb, String func){
    String path = adb.path();
    STDERR.println(ADBTest.class.getName() + ": " + path + ": " + func + ": error");
  }
  /* parse arguments of write command */
  private static int runwrite(String[] args){
    String name = null;
    String rstr = null;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(name == null && arg.startsWith("-")){
        usage();
      } else if(name == null){
        name = arg;
      } else if(rstr == null){
        rstr = arg;
      } else {
        usage();
      }
    }
    if(name == null || rstr == null) usage();
    int rnum = Util.atoi(rstr);
    if(rnum < 1) usage();
    int rv = procwrite(name, rnum);
    return rv;
  }
  /* parse arguments of read command */
  private static int runread(String[] args){
    String name = null;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(name == null && arg.startsWith("-")){
        usage();
      } else if(name == null){
        name = arg;
      } else {
        usage();
      }
    }
    if(name == null) usage();
    int rv = procread(name);
    return rv;
  }
  /* parse arguments of remove command */
  private static int runremove(String[] args){
    String name = null;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(name == null && arg.startsWith("-")){
        usage();
      } else if(name == null){
        name = arg;
      } else {
        usage();
      }
    }
    if(name == null) usage();
    int rv = procremove(name);
    return rv;
  }
  /* parse arguments of misc command */
  private static int runmisc(String[] args){
    String name = null;
    String rstr = null;
    for(int i = 1; i < args.length; i++){
      String arg = args[i];
      if(name == null && arg.startsWith("-")){
        usage();
      } else if(name == null){
        name = arg;
      } else if(rstr == null){
        rstr = arg;
      } else {
        usage();
      }
    }
    if(name == null || rstr == null) usage();
    int rnum = Util.atoi(rstr);
    if(rnum < 1) usage();
    int rv = procmisc(name, rnum);
    return rv;
  }
  /* perform write command */
  private static int procwrite(String name, int rnum){
    STDOUT.println("<Writing Test>");
    STDOUT.println("  name=" + name + "  rnum=" + rnum);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    ADB adb = new ADB();
    if(!adb.open(name)){
      eprint(adb, "open");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!adb.put(buf, buf)){
        eprint(adb, "put");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + adb.rnum());
    STDOUT.println("size: " + adb.size());
    if(!adb.close()){
      eprint(adb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  /* perform read command */
  private static int procread(String name){
    STDOUT.println("<Reading Test>");
    STDOUT.println("  name=" + name);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    ADB adb = new ADB();
    if(!adb.open(name)){
      eprint(adb, "open");
      err = true;
    }
    int rnum = (int)adb.rnum();
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(adb.get(buf) == null){
        eprint(adb, "get");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + adb.rnum());
    STDOUT.println("size: " + adb.size());
    if(!adb.close()){
      eprint(adb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  /* perform remove command */
  private static int procremove(String name){
    STDOUT.println("<Removing Test>");
    STDOUT.println("  name=" + name);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    ADB adb = new ADB();
    if(!adb.open(name)){
      eprint(adb, "open");
      err = true;
    }
    int rnum = (int)adb.rnum();
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!adb.out(buf)){
        eprint(adb, "out");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("record number: " + adb.rnum());
    STDOUT.println("size: " + adb.size());
    if(!adb.close()){
      eprint(adb, "close");
      err = true;
    }
    STDOUT.println("time: " + (Util.time() - stime));
    STDOUT.println(err ? "error" : "ok");
    STDOUT.println("");
    return err ? 1 : 0;
  }
  /* perform misc command */
  private static int procmisc(String name, int rnum){
    STDOUT.println("<Miscellaneous Test>");
    STDOUT.println("  name=" + name + "  rnum=" + rnum);
    STDOUT.println("");
    boolean err = false;
    double stime = Util.time();
    ADB adb = new ADB();
    if(!adb.open(name)){
      eprint(adb, "open");
      err = true;
    }
    STDOUT.println("writing:");
    for(int i = 1; i <= rnum; i++){
      String buf = Util.itoa(i, 8, '0');
      if(!adb.put(buf, buf)){
        eprint(adb, "put");
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
      if(adb.get(buf) == null){
        eprint(adb, "get");
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
      if(RND.nextInt(2) == 0 && !adb.out(buf)){
        eprint(adb, "out");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + buf + ")");
      }
    }
    STDOUT.println("checking iterator:");
    if(!adb.iterinit()){
      eprint(adb, "iterinit");
      err = true;
    }
    int inum = 0;
    String key;
    while((key = adb.iternext2()) != null){
      String value = adb.get(key);
      if(value == null){
        eprint(adb, "get");
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
    if(inum != adb.rnum()){
      eprint(adb, "(validation)");
      err = true;
    }
    List keys = adb.fwmkeys("0", 10);
    if(adb.rnum() >= 10 && keys.size() != 10){
      eprint(adb, "fwmkeys");
      err = true;
    }
    STDOUT.println("checking counting:");
    for(int i = 1; i <= rnum; i++){
      String buf = "[" + RND.nextInt(rnum) + "]";
      if(RND.nextInt(2) == 0){
        adb.addint(buf, 1);
      } else {
        adb.adddouble(buf, 1.0);
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    STDOUT.println("checking versatile functions:");
    for(int i = 1; i <= rnum; i++){
      int rnd = RND.nextInt(3);
      String cname;
      if(rnd == 0){
        cname = "putlist";
      } else if(rnd == 1){
        cname = "outlist";
      } else {
        cname = "getlist";
      }
      List args = new ArrayList();
      args.add(new Integer(RND.nextInt(rnum)));
      args.add(new Integer(RND.nextInt(rnum)));
      if(adb.misc(cname, args) == null){
        eprint(adb, "(validation)");
        err = true;
        break;
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!adb.sync()){
      eprint(adb, "sync");
      err = true;
    }
    if(!adb.optimize()){
      eprint(adb, "optimize");
      err = true;
    }
    String npath = adb.path() + "-tmp";
    if(!adb.copy(npath)){
      eprint(adb, "copy");
      err = true;
    }
    (new File(npath)).delete();
    if(!adb.vanish()){
      eprint(adb, "vanish");
      err = true;
    }
    STDOUT.println("checking transaction commit:");
    if(!adb.tranbegin()){
      eprint(adb, "tranbegin");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String buf = i + "";
      if(RND.nextInt(2) == 0){
        if(!adb.putcat(buf, buf)){
          eprint(adb, "putcat");
          err = true;
          break;
        }
      } else {
        adb.out(buf);
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!adb.trancommit()){
      eprint(adb, "trancommit");
      err = true;
    }
    STDOUT.println("checking transaction abort:");
    long ornum = adb.rnum();
    long ofsiz = adb.fsiz();
    if(!adb.tranbegin()){
      eprint(adb, "tranbegin");
      err = true;
    }
    for(int i = 1; i <= rnum; i++){
      String buf = i + "";
      if(RND.nextInt(2) == 0){
        if(!adb.putcat(buf, buf)){
          eprint(adb, "putcat");
          err = true;
          break;
        }
      } else {
        adb.out(buf);
      }
      if(rnum > 250 && i % (rnum / 250) == 0){
        STDOUT.print('.');
        if(i == rnum || i % (rnum / 10) == 0) STDOUT.println(" (" + Util.itoa(i, 8, '0') + ")");
      }
    }
    if(!adb.tranabort()){
      eprint(adb, "trancommit");
      err = true;
    }
    if(adb.rnum() != ornum || adb.fsiz() != ofsiz){
      eprint(adb, "(validation)");
      err = true;
    }
    STDOUT.println("checking common interface:");
    DBM dbm = adb;
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
      eprint(adb, "iterinit");
      err = true;
    }
    while((key = dbm.iternext2()) != null){
      String value = dbm.get(key);
      if(value == null){
        eprint(adb, "get");
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
    STDOUT.println("record number: " + adb.rnum());
    STDOUT.println("size: " + adb.size());
    if(!adb.close()){
      eprint(adb, "close");
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
  private ADBTest() throws NoSuchMethodException {
    throw new NoSuchMethodException();
  }
}



/* END OF FILE */
