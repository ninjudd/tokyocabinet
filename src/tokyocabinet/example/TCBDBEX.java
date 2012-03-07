import tokyocabinet.*;

public class TCBDBEX {
  public static void main(String[] args){

    // create the object
    BDB bdb = new BDB();

    // open the database
    if(!bdb.open("casket.tcb", BDB.OWRITER | BDB.OCREAT)){
      int ecode = bdb.ecode();
      System.err.println("open error: " + bdb.errmsg(ecode));
    }

    // store records
    if(!bdb.put("foo", "hop") ||
       !bdb.put("bar", "step") ||
       !bdb.put("baz", "jump")){
      int ecode = bdb.ecode();
      System.err.println("put error: " + bdb.errmsg(ecode));
    }

    // retrieve records
    String value = bdb.get("foo");
    if(value != null){
      System.out.println(value);
    } else {
      int ecode = bdb.ecode();
      System.err.println("get error: " + bdb.errmsg(ecode));
    }

    // traverse records
    BDBCUR cur = new BDBCUR(bdb);
    cur.first();
    String key;
    while((key = cur.key2()) != null){
      value = cur.val2();
      if(value != null){
        System.out.println(key + ":" + value);
      }
      cur.next();
    }

    // close the database
    if(!bdb.close()){
      int ecode = bdb.ecode();
      System.err.println("close error: " + bdb.errmsg(ecode));
    }

  }
}
