import tokyocabinet.*;

public class TCADBEX {
  public static void main(String[] args){

    // create the object
    ADB adb = new ADB();

    // open the database
    if(!adb.open("casket.tch")){
      System.err.println("open error");
    }

    // store records
    if(!adb.put("foo", "hop") ||
       !adb.put("bar", "step") ||
       !adb.put("baz", "jump")){
      System.err.println("put error");
    }

    // retrieve records
    String value = adb.get("foo");
    if(value != null){
      System.out.println(value);
    } else {
      System.err.println("get error");
    }

    // traverse records
    adb.iterinit();
    String key;
    while((key = adb.iternext2()) != null){
      value = adb.get(key);
      if(value != null){
        System.out.println(key + ":" + value);
      }
    }

    // close the database
    if(!adb.close()){
      System.err.println("close error");
    }

  }
}
