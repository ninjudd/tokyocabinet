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


#include "tokyocabinet_HDB.h"
#include "myconf.h"


/* private global variables */
static jclass cls_hdb;
static jfieldID hdb_fid_ptr;



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


/* errmsg */
JNIEXPORT jstring JNICALL Java_tokyocabinet_HDB_errmsg
(JNIEnv *env, jclass cls, jint ecode){
  jstring jmsg = (*env)->NewStringUTF(env, tchdberrmsg(ecode));
  if(!jmsg){
    throwoutmem(env);
    return NULL;
  }
  return jmsg;
}


/* init */
JNIEXPORT void JNICALL Java_tokyocabinet_HDB_init
(JNIEnv *env, jclass cls){
  cls_hdb = cls;
  hdb_fid_ptr = (*env)->GetFieldID(env, cls, "ptr", "J");
}


/* ecode */
JNIEXPORT jint JNICALL Java_tokyocabinet_HDB_ecode
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbecode(hdb);
}


/* tune */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_tune
(JNIEnv *env, jobject self, jlong bnum, jint apow, jint fpow, jint opts){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbtune(hdb, bnum, apow, fpow, opts);
}


/* setcache */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_setcache
(JNIEnv *env, jobject self, jint rcnum){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbsetcache(hdb, rcnum);
}


/* setxmsiz */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_setxmsiz
(JNIEnv *env, jobject self, jlong xmsiz){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbsetxmsiz(hdb, xmsiz);
}


/* setdfunit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_setdfunit
(JNIEnv *env, jobject self, jint dfunit){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbsetdfunit(hdb, dfunit);
}


/* open */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_open
(JNIEnv *env, jobject self, jstring path, jint omode){
  if(!path){
    throwillarg(env);
    return false;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  bool rv = tchdbopen(hdb, tpath, omode);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* close */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_close
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbclose(hdb);
}


/* put */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_put
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
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
  bool rv = tchdbput(hdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putkeep */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_putkeep
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
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
  bool rv = tchdbputkeep(hdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putcat */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_putcat
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
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
  bool rv = tchdbputcat(hdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putasync */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_putasync
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
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
  bool rv = tchdbputasync(hdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* out */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_out
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return false;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  bool rv = tchdbout(hdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* get */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_HDB_get
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return NULL;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return NULL;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  int vsiz;
  char *vbuf = tchdbget(hdb, kbuf, ksiz, &vsiz);
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
JNIEXPORT jint JNICALL Java_tokyocabinet_HDB_vsiz
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return 0;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  int rv = tchdbvsiz(hdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* iterinit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_iterinit
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbiterinit(hdb);
}


/* iternext */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_HDB_iternext
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  int vsiz;
  char *vbuf = tchdbiternext(hdb, &vsiz);
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
JNIEXPORT jobject JNICALL Java_tokyocabinet_HDB_fwmkeys
(JNIEnv *env, jobject self, jbyteArray prefix, jint max){
  if(!prefix){
    throwillarg(env);
    return NULL;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  jboolean icp;
  jbyte *pbuf = (*env)->GetByteArrayElements(env, prefix, &icp);
  if(!pbuf){
    throwoutmem(env);
    return NULL;
  }
  int psiz = (*env)->GetArrayLength(env, prefix);
  TCLIST *tkeys = tchdbfwmkeys(hdb, pbuf, psiz, max);
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
  if(icp) (*env)->ReleaseByteArrayElements(env, prefix, pbuf, JNI_ABORT);
  return keys;
}


/* addint */
JNIEXPORT jint JNICALL Java_tokyocabinet_HDB_addint
(JNIEnv *env, jobject self, jbyteArray key, jint num){
  if(!key){
    throwillarg(env);
    return 0;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  num = tchdbaddint(hdb, kbuf, ksiz, num);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return num;
}


/* adddouble */
JNIEXPORT jdouble JNICALL Java_tokyocabinet_HDB_adddouble
(JNIEnv *env, jobject self, jbyteArray key, jdouble num){
  if(!key){
    throwillarg(env);
    return 0.0;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0.0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  num = tchdbadddouble(hdb, kbuf, ksiz, num);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return num;
}


/* sync */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_sync
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbsync(hdb);
}


/* optimize */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_optimize
(JNIEnv *env, jobject self, jlong bnum, jint apow, jint fpow, jint opts){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdboptimize(hdb, bnum, apow, fpow, opts);
}


/* vanish */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_vanish
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbvanish(hdb);
}


/* copy */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_copy
(JNIEnv *env, jobject self, jstring path){
  if(!path){
    throwillarg(env);
    return false;
  }
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  bool rv = tchdbcopy(hdb, tpath);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* tranbegin */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_tranbegin
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbtranbegin(hdb);
}


/* trancommit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_trancommit
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbtrancommit(hdb);
}


/* tranabort */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_HDB_tranabort
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbtranabort(hdb);
}


/* path */
JNIEXPORT jstring JNICALL Java_tokyocabinet_HDB_path
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  const char *path = tchdbpath(hdb);
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
JNIEXPORT jlong JNICALL Java_tokyocabinet_HDB_rnum
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbrnum(hdb);
}


/* fsiz */
JNIEXPORT jlong JNICALL Java_tokyocabinet_HDB_fsiz
(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  return tchdbfsiz(hdb);
}


/* initialize */
JNIEXPORT void JNICALL
Java_tokyocabinet_HDB_initialize(JNIEnv *env, jobject self){
  TCHDB *hdb = tchdbnew();
  tchdbsetmutex(hdb);
  (*env)->SetLongField(env, self, hdb_fid_ptr, (intptr_t)hdb);
}


/* destruct */
JNIEXPORT void JNICALL
Java_tokyocabinet_HDB_destruct(JNIEnv *env, jobject self){
  TCHDB *hdb = (TCHDB *)(intptr_t)(*env)->GetLongField(env, self, hdb_fid_ptr);
  tchdbdel(hdb);
}



/* END OF FILE */
