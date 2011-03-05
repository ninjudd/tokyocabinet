/*************************************************************************************************
 * Java binding of Tokyo Cabinet
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


#include "tokyocabinet_TDB.h"
#include "myconf.h"


/* private global variables */
static jclass cls_tdb;
static jfieldID tdb_fid_ptr;



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


/* errmsg */
JNIEXPORT jstring JNICALL Java_tokyocabinet_TDB_errmsg
(JNIEnv *env, jclass cls, jint ecode){
  jstring jmsg = (*env)->NewStringUTF(env, tctdberrmsg(ecode));
  if(!jmsg){
    throwoutmem(env);
    return NULL;
  }
  return jmsg;
}


/* init */
JNIEXPORT void JNICALL Java_tokyocabinet_TDB_init
(JNIEnv *env, jclass cls){
  cls_tdb = cls;
  tdb_fid_ptr = (*env)->GetFieldID(env, cls, "ptr", "J");
}


/* ecode */
JNIEXPORT jint JNICALL Java_tokyocabinet_TDB_ecode
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbecode(tdb);
}


/* tune */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_tune
(JNIEnv *env, jobject self, jlong bnum, jint apow, jint fpow, jint opts){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbtune(tdb, bnum, apow, fpow, opts);
}


/* setcache */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_setcache
(JNIEnv *env, jobject self, jint rcnum, jint lcnum, jint ncnum){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbsetcache(tdb, rcnum, lcnum, ncnum);
}


/* setxmsiz */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_setxmsiz
(JNIEnv *env, jobject self, jlong xmsiz){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbsetxmsiz(tdb, xmsiz);
}


/* setdfunit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_setdfunit
(JNIEnv *env, jobject self, jint dfunit){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbsetdfunit(tdb, dfunit);
}


/* open */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_open
(JNIEnv *env, jobject self, jstring path, jint omode){
  if(!path){
    throwillarg(env);
    return false;
  }
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  bool rv = tctdbopen(tdb, tpath, omode);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* close */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_close
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbclose(tdb);
}


/* out */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_out
(JNIEnv *env, jobject self, jbyteArray pkey){
  if(!pkey){
    throwillarg(env);
    return false;
  }
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  bool rv = tctdbout(tdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return rv;
}


/* vsiz */
JNIEXPORT jint JNICALL Java_tokyocabinet_TDB_vsiz
(JNIEnv *env, jobject self, jbyteArray pkey){
  if(!pkey){
    throwillarg(env);
    return 0;
  }
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  int rv = tctdbvsiz(tdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return rv;
}


/* iterinit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_iterinit
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbiterinit(tdb);
}


/* iternext */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_TDB_iternext
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  int vsiz;
  char *vbuf = tctdbiternext(tdb, &vsiz);
  jbyteArray val;
  if(vbuf){
    val = (*env)->NewByteArray(env, vsiz);
    if(!val){
      throwoutmem(env);
      return NULL;
    }
    (*env)->SetByteArrayRegion(env, val, 0, vsiz, (jbyte *)vbuf);
    tcfree(vbuf);
  } else {
    val = NULL;
  }
  return val;
}


/* fwmkeys */
JNIEXPORT jobject JNICALL Java_tokyocabinet_TDB_fwmkeys
(JNIEnv *env, jobject self, jbyteArray prefix, jint max){
  if(!prefix){
    throwillarg(env);
    return NULL;
  }
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  jboolean icp;
  jbyte *pbuf = (*env)->GetByteArrayElements(env, prefix, &icp);
  if(!pbuf){
    throwoutmem(env);
    return NULL;
  }
  int psiz = (*env)->GetArrayLength(env, prefix);
  TCLIST *tkeys = tctdbfwmkeys(tdb, pbuf, psiz, max);
  jclass clslist = (*env)->FindClass(env, CLSARRAYLIST);
  jmethodID midinit = (*env)->GetMethodID(env, clslist, "<init>", "()V");
  jobject pkeys = (*env)->NewObject(env, clslist, midinit);
  jmethodID midadd = (*env)->GetMethodID(env, clslist, "add", "(L" CLSOBJECT ";)Z");
  for(int i = 0; i < tclistnum(tkeys); i++){
    int ksiz;
    const char *kbuf = tclistval(tkeys, i, &ksiz);
    jbyteArray pkey = (*env)->NewByteArray(env, ksiz);
    (*env)->SetByteArrayRegion(env, pkey, 0, ksiz, (jbyte *)kbuf);
    (*env)->CallVoidMethod(env, pkeys, midadd, pkey);
    (*env)->DeleteLocalRef(env, pkey);
  }
  tclistdel(tkeys);
  if(icp) (*env)->ReleaseByteArrayElements(env, prefix, pbuf, JNI_ABORT);
  return pkeys;
}


/* addint */
JNIEXPORT jint JNICALL Java_tokyocabinet_TDB_addint
(JNIEnv *env, jobject self, jbyteArray pkey, jint num){
  if(!pkey){
    throwillarg(env);
    return 0;
  }
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  num = tctdbaddint(tdb, kbuf, ksiz, num);
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return num;
}


/* adddouble */
JNIEXPORT jdouble JNICALL Java_tokyocabinet_TDB_adddouble
(JNIEnv *env, jobject self, jbyteArray pkey, jdouble num){
  if(!pkey){
    throwillarg(env);
    return 0.0;
  }
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0.0;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  num = tctdbadddouble(tdb, kbuf, ksiz, num);
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return num;
}


/* sync */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_sync
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbsync(tdb);
}


/* optimize */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_optimize
(JNIEnv *env, jobject self, jlong bnum, jint apow, jint fpow, jint opts){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdboptimize(tdb, bnum, apow, fpow, opts);
}


/* vanish */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_vanish
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbvanish(tdb);
}


/* copy */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_copy
(JNIEnv *env, jobject self, jstring path){
  if(!path){
    throwillarg(env);
    return false;
  }
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  bool rv = tctdbcopy(tdb, tpath);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* tranbegin */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_tranbegin
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbtranbegin(tdb);
}


/* trancommit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_trancommit
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbtrancommit(tdb);
}


/* tranabort */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_tranabort
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbtranabort(tdb);
}


/* path */
JNIEXPORT jstring JNICALL Java_tokyocabinet_TDB_path
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  const char *path = tctdbpath(tdb);
  jstring jpath;
  if(path){
    jpath = (*env)->NewStringUTF(env, path);
    if(!path){
      throwoutmem(env);
      return NULL;
    }
  } else {
    jpath = NULL;
  }
  return jpath;
}


/* rnum */
JNIEXPORT jlong JNICALL Java_tokyocabinet_TDB_rnum
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbrnum(tdb);
}


/* fsiz */
JNIEXPORT jlong JNICALL Java_tokyocabinet_TDB_fsiz
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbfsiz(tdb);
}


/* setindex */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_setindex
(JNIEnv *env, jobject self, jstring name, jint type){
  if(!name){
    throwillarg(env);
    return false;
  }
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  jboolean icn;
  const char *tname = (*env)->GetStringUTFChars(env, name, &icn);
  if(!tname){
    throwoutmem(env);
    return false;
  }
  bool rv = tctdbsetindex(tdb, tname, type);
  if(icn) (*env)->ReleaseStringUTFChars(env, name, tname);
  return rv;
}


/* genuid */
JNIEXPORT jlong JNICALL Java_tokyocabinet_TDB_genuid
(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  return tctdbgenuid(tdb);
}


/* initialize */
JNIEXPORT void JNICALL
Java_tokyocabinet_TDB_initialize(JNIEnv *env, jobject self){
  TCTDB *tdb = tctdbnew();
  tctdbsetmutex(tdb);
  (*env)->SetLongField(env, self, tdb_fid_ptr, (intptr_t)tdb);
}


/* destruct */
JNIEXPORT void JNICALL
Java_tokyocabinet_TDB_destruct(JNIEnv *env, jobject self){
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  tctdbdel(tdb);
}


/* putimpl */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDB_putimpl
(JNIEnv *env, jobject self, jbyteArray pkey, jobjectArray cols, jint dmode){
  if(!pkey || !cols){
    throwillarg(env);
    return false;
  }
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  jsize cnum = (*env)->GetArrayLength(env, cols);
  TCMAP *tcols = tcmapnew2(cnum + 1);
  cnum--;
  for(int i = 0; i < cnum; i += 2){
    jobject name = (*env)->GetObjectArrayElement(env, cols, i);
    jboolean icn;
    jbyte *nbuf = (*env)->GetByteArrayElements(env, name, &icn);
    if(!nbuf){
      throwoutmem(env);
      return false;
    }
    int nsiz = (*env)->GetArrayLength(env, name);
    jobject val = (*env)->GetObjectArrayElement(env, cols, i + 1);
    jboolean icv;
    jbyte *vbuf = (*env)->GetByteArrayElements(env, val, &icv);
    if(!vbuf){
      throwoutmem(env);
      return false;
    }
    int vsiz = (*env)->GetArrayLength(env, val);
    tcmapputkeep(tcols, nbuf, nsiz, vbuf, vsiz);
    if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
    if(icn) (*env)->ReleaseByteArrayElements(env, name, nbuf, JNI_ABORT);
  }
  bool rv;
  switch(dmode){
  case 0:
    rv = tctdbput(tdb, kbuf, ksiz, tcols);
    break;
  case 1:
    rv = tctdbputkeep(tdb, kbuf, ksiz, tcols);
    break;
  case 2:
    rv = tctdbputcat(tdb, kbuf, ksiz, tcols);
    break;
  default:
    rv = false;
    break;
  }
  tcmapdel(tcols);
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return rv;
}


/* getimpl */
JNIEXPORT jobjectArray JNICALL Java_tokyocabinet_TDB_getimpl
(JNIEnv *env, jobject self, jbyteArray pkey){
  if(!pkey){
    throwillarg(env);
    return 0;
  }
  TCTDB *tdb = (TCTDB *)(intptr_t)(*env)->GetLongField(env, self, tdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, pkey, &ick);
  if(!kbuf){
    throwoutmem(env);
    return NULL;
  }
  int ksiz = (*env)->GetArrayLength(env, pkey);
  jobjectArray ary = NULL;
  TCMAP *tcols = tctdbget(tdb, kbuf, ksiz);
  if(tcols){
    int anum = tcmaprnum(tcols) * 2;
    ary = (*env)->NewObjectArray(env, anum, (*env)->GetObjectClass(env, pkey), NULL);
    anum = 0;
    tcmapiterinit(tcols);
    const char *nbuf;
    int nsiz;
    while((nbuf = tcmapiternext(tcols, &nsiz)) != NULL){
      int vsiz;
      const char *vbuf = tcmapiterval(nbuf, &vsiz);
      jbyteArray nary = (*env)->NewByteArray(env, nsiz);
      if(!nary){
        throwoutmem(env);
        return NULL;
      }
      jbyteArray vary = (*env)->NewByteArray(env, vsiz);
      if(!vary){
        throwoutmem(env);
        return NULL;
      }
      (*env)->SetByteArrayRegion(env, nary, 0, nsiz, (jbyte *)nbuf);
      (*env)->SetByteArrayRegion(env, vary, 0, vsiz, (jbyte *)vbuf);
      (*env)->SetObjectArrayElement(env, ary, anum++, nary);
      (*env)->SetObjectArrayElement(env, ary, anum++, vary);
      (*env)->DeleteLocalRef(env, vary);
      (*env)->DeleteLocalRef(env, nary);
    }
    tcmapdel(tcols);
  }
  if(ick) (*env)->ReleaseByteArrayElements(env, pkey, kbuf, JNI_ABORT);
  return ary;
}



/* END OF FILE */
