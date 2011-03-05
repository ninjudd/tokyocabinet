import tokyocabinet.*;
import java.util.*;

public class TCTDBEX {
  public static void main(String[] args){

    // create the object
    TDB tdb = new TDB();

    // open the database
    if(!tdb.open("casket.tct", TDB.OWRITER | TDB.OCREAT)){
      int ecode = tdb.ecode();
      System.err.println("open error: " + tdb.errmsg(ecode));
    }

    // store a record
    String pkey = new Long(tdb.genuid()).toString();
    Map cols = new HashMap();
    cols.put("name", "mikio");
    cols.put("age", "30");
    cols.put("lang", "ja,en,c");
    if(!tdb.put(pkey, cols)){
      int ecode = tdb.ecode();
      System.err.println("put error: " + tdb.errmsg(ecode));
    }

    // store another record
    cols = new HashMap();
    cols.put("name", "falcon");
    cols.put("age", "31");
    cols.put("lang", "ja");
    cols.put("skill", "cook,blog");
    if(!tdb.put("x12345", cols)){
      int ecode = tdb.ecode();
      System.err.println("put error: " + tdb.errmsg(ecode));
    }

    // search for records
    TDBQRY qry = new TDBQRY(tdb);
    qry.addcond("age", TDBQRY.QCNUMGE, "20");
    qry.addcond("lang", TDBQRY.QCSTROR, "ja,en");
    qry.setorder("name", TDBQRY.QOSTRASC);
    qry.setlimit(10, 0);
    List res = qry.search();
    Iterator it = res.iterator();
    while(it.hasNext()){
      String rkey = new String((byte[])it.next());
      Map rcols = tdb.get(rkey);
      System.out.println("name:" + rcols.get("name"));
    }

    // close the database
    if(!tdb.close()){
      int ecode = tdb.ecode();
      System.err.println("close error: " + tdb.errmsg(ecode));
    }

  }
}
