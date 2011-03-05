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


#include "tokyocabinet_FDB.h"
#include "myconf.h"


/* private global variables */
static jclass cls_fdb;
static jfieldID fdb_fid_ptr;



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


/* errmsg */
JNIEXPORT jstring JNICALL Java_tokyocabinet_FDB_errmsg
(JNIEnv *env, jclass cls, jint ecode){
  jstring jmsg = (*env)->NewStringUTF(env, tcfdberrmsg(ecode));
  if(!jmsg){
    throwoutmem(env);
    return NULL;
  }
  return jmsg;
}


/* init */
JNIEXPORT void JNICALL Java_tokyocabinet_FDB_init
(JNIEnv *env, jclass cls){
  cls_fdb = cls;
  fdb_fid_ptr = (*env)->GetFieldID(env, cls, "ptr", "J");
}


/* ecode */
JNIEXPORT jint JNICALL Java_tokyocabinet_FDB_ecode
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbecode(fdb);
}


/* tune */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_tune
(JNIEnv *env, jobject self, jint width, jlong limsiz){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbtune(fdb, width, limsiz);
}


/* open */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_open
(JNIEnv *env, jobject self, jstring path, jint omode){
  if(!path){
    throwillarg(env);
    return false;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  bool rv = tcfdbopen(fdb, tpath, omode);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* close */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_close
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbclose(fdb);
}


/* put */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_put
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  jboolean icv;
  jbyte *vbuf = (*env)->GetByteArrayElements(env, val, &icv);
  if(!vbuf){
    throwoutmem(env);
    return false;
  }
  int vsiz = (*env)->GetArrayLength(env, val);
  bool rv = tcfdbput2(fdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putkeep */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_putkeep
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  jboolean icv;
  jbyte *vbuf = (*env)->GetByteArrayElements(env, val, &icv);
  if(!vbuf){
    throwoutmem(env);
    return false;
  }
  int vsiz = (*env)->GetArrayLength(env, val);
  bool rv = tcfdbputkeep2(fdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putcat */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_putcat
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  jboolean icv;
  jbyte *vbuf = (*env)->GetByteArrayElements(env, val, &icv);
  if(!vbuf){
    throwoutmem(env);
    return false;
  }
  int vsiz = (*env)->GetArrayLength(env, val);
  bool rv = tcfdbputcat2(fdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* out */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_out
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return false;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  bool rv = tcfdbout2(fdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* get */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_FDB_get
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return NULL;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return NULL;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  int vsiz;
  char *vbuf = tcfdbget2(fdb, kbuf, ksiz, &vsiz);
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
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return val;
}


/* vsiz */
JNIEXPORT jint JNICALL Java_tokyocabinet_FDB_vsiz
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return 0;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  int rv = tcfdbvsiz2(fdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* iterinit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_iterinit
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbiterinit(fdb);
}


/* iternext */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_FDB_iternext
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  int vsiz;
  char *vbuf = tcfdbiternext2(fdb, &vsiz);
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


/* range */
JNIEXPORT jobject JNICALL Java_tokyocabinet_FDB_range
(JNIEnv *env, jobject self, jbyteArray interval, jint max){
  if(!interval){
    throwillarg(env);
    return NULL;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean icp;
  jbyte *ibuf = (*env)->GetByteArrayElements(env, interval, &icp);
  if(!ibuf){
    throwoutmem(env);
    return NULL;
  }
  int isiz = (*env)->GetArrayLength(env, interval);
  TCLIST *tkeys = tcfdbrange4(fdb, ibuf, isiz, max);
  jclass clslist = (*env)->FindClass(env, CLSARRAYLIST);
  jmethodID midinit = (*env)->GetMethodID(env, clslist, "<init>", "()V");
  jobject keys = (*env)->NewObject(env, clslist, midinit);
  jmethodID midadd = (*env)->GetMethodID(env, clslist, "add", "(L" CLSOBJECT ";)Z");
  for(int i = 0; i < tclistnum(tkeys); i++){
    int ksiz;
    const char *kbuf = tclistval(tkeys, i, &ksiz);
    jbyteArray key = (*env)->NewByteArray(env, ksiz);
    (*env)->SetByteArrayRegion(env, key, 0, ksiz, (jbyte *)kbuf);
    (*env)->CallVoidMethod(env, keys, midadd, key);
    (*env)->DeleteLocalRef(env, key);
  }
  tclistdel(tkeys);
  if(icp) (*env)->ReleaseByteArrayElements(env, interval, ibuf, JNI_ABORT);
  return keys;
}


/* addint */
JNIEXPORT jint JNICALL Java_tokyocabinet_FDB_addint
(JNIEnv *env, jobject self, jbyteArray key, jint num){
  if(!key){
    throwillarg(env);
    return 0;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  num = tcfdbaddint(fdb, tcfdbkeytoid((char *)kbuf, ksiz), num);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return num;
}


/* adddouble */
JNIEXPORT jdouble JNICALL Java_tokyocabinet_FDB_adddouble
(JNIEnv *env, jobject self, jbyteArray key, jdouble num){
  if(!key){
    throwillarg(env);
    return 0.0;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0.0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  num = tcfdbadddouble(fdb, tcfdbkeytoid((char *)kbuf, ksiz), num);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return num;
}


/* sync */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_sync
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbsync(fdb);
}


/* optimize */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_optimize
(JNIEnv *env, jobject self, jint width, jlong limsiz){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdboptimize(fdb, width, limsiz);
}


/* vanish */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_vanish
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbvanish(fdb);
}


/* copy */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_copy
(JNIEnv *env, jobject self, jstring path){
  if(!path){
    throwillarg(env);
    return false;
  }
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  bool rv = tcfdbcopy(fdb, tpath);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* tranbegin */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_tranbegin
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbtranbegin(fdb);
}


/* trancommit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_trancommit
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbtrancommit(fdb);
}


/* tranabort */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_FDB_tranabort
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbtranabort(fdb);
}


/* path */
JNIEXPORT jstring JNICALL Java_tokyocabinet_FDB_path
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  const char *path = tcfdbpath(fdb);
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
JNIEXPORT jlong JNICALL Java_tokyocabinet_FDB_rnum
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbrnum(fdb);
}


/* fsiz */
JNIEXPORT jlong JNICALL Java_tokyocabinet_FDB_fsiz
(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  return tcfdbfsiz(fdb);
}


/* initialize */
JNIEXPORT void JNICALL
Java_tokyocabinet_FDB_initialize(JNIEnv *env, jobject self){
  TCFDB *fdb = tcfdbnew();
  tcfdbsetmutex(fdb);
  (*env)->SetLongField(env, self, fdb_fid_ptr, (intptr_t)fdb);
}


/* destruct */
JNIEXPORT void JNICALL
Java_tokyocabinet_FDB_destruct(JNIEnv *env, jobject self){
  TCFDB *fdb = (TCFDB *)(intptr_t)(*env)->GetLongField(env, self, fdb_fid_ptr);
  tcfdbdel(fdb);
}



/* END OF FILE */
