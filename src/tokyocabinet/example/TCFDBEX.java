import tokyocabinet.*;

public class TCFDBEX {
  public static void main(String[] args){

    // create the object
    FDB fdb = new FDB();

    // open the database
    if(!fdb.open("casket.tcf", FDB.OWRITER | FDB.OCREAT)){
      int ecode = fdb.ecode();
      System.err.println("open error: " + fdb.errmsg(ecode));
    }

    // store records
    if(!fdb.put("1", "one") ||
       !fdb.put("12", "twelve") ||
       !fdb.put("144", "one forty four")){
      int ecode = fdb.ecode();
      System.err.println("put error: " + fdb.errmsg(ecode));
    }

    // retrieve records
    String value = fdb.get("1");
    if(value != null){
      System.out.println(value);
    } else {
      int ecode = fdb.ecode();
      System.err.println("get error: " + fdb.errmsg(ecode));
    }

    // traverse records
    fdb.iterinit();
    String key;
    while((key = fdb.iternext2()) != null){
      value = fdb.get(key);
      if(value != null){
        System.out.println(key + ":" + value);
      }
    }

    // close the database
    if(!fdb.close()){
      int ecode = fdb.ecode();
      System.err.println("close error: " + fdb.errmsg(ecode));
    }

  }
}
