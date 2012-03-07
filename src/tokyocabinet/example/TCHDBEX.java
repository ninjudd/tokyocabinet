import tokyocabinet.*;

public class TCHDBEX {
  public static void main(String[] args){

    // create the object
    HDB hdb = new HDB();

    // open the database
    if(!hdb.open("casket.tch", HDB.OWRITER | HDB.OCREAT)){
      int ecode = hdb.ecode();
      System.err.println("open error: " + hdb.errmsg(ecode));
    }

    // store records
    if(!hdb.put("foo", "hop") ||
       !hdb.put("bar", "step") ||
       !hdb.put("baz", "jump")){
      int ecode = hdb.ecode();
      System.err.println("put error: " + hdb.errmsg(ecode));
    }

    // retrieve records
    String value = hdb.get("foo");
    if(value != null){
      System.out.println(value);
    } else {
      int ecode = hdb.ecode();
      System.err.println("get error: " + hdb.errmsg(ecode));
    }

    // traverse records
    hdb.iterinit();
    String key;
    while((key = hdb.iternext2()) != null){
      value = hdb.get(key);
      if(value != null){
        System.out.println(key + ":" + value);
      }
    }

    // close the database
    if(!hdb.close()){
      int ecode = hdb.ecode();
      System.err.println("close error: " + hdb.errmsg(ecode));
    }

  }
}
