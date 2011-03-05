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


#include "tokyocabinet_ADB.h"
#include "myconf.h"


/* private global variables */
static jclass cls_adb;
static jfieldID adb_fid_ptr;



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


/* version */
JNIEXPORT jstring JNICALL Java_tokyocabinet_ADB_version
(JNIEnv *env , jclass cls){
  jstring jver = (*env)->NewStringUTF(env, tcversion);
  if(!jver){
    throwoutmem(env);
    return NULL;
  }
  return jver;
}


/* init */
JNIEXPORT void JNICALL Java_tokyocabinet_ADB_init
(JNIEnv *env, jclass cls){
  cls_adb = cls;
  adb_fid_ptr = (*env)->GetFieldID(env, cls, "ptr", "J");
}


/* open */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_open
(JNIEnv *env, jobject self, jstring name){
  if(!name){
    throwillarg(env);
    return false;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  jboolean icn;
  const char *tname = (*env)->GetStringUTFChars(env, name, &icn);
  if(!tname){
    throwoutmem(env);
    return false;
  }
  bool rv = tcadbopen(adb, tname);
  if(icn) (*env)->ReleaseStringUTFChars(env, name, tname);
  return rv;
}


/* close */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_close
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  return tcadbclose(adb);
}


/* put */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_put
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
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
  bool rv = tcadbput(adb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putkeep */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_putkeep
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
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
  bool rv = tcadbputkeep(adb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putcat */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_putcat
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
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
  bool rv = tcadbputcat(adb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* out */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_out
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return false;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  bool rv = tcadbout(adb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* get */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_ADB_get
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return NULL;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return NULL;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  int vsiz;
  char *vbuf = tcadbget(adb, kbuf, ksiz, &vsiz);
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
JNIEXPORT jint JNICALL Java_tokyocabinet_ADB_vsiz
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return 0;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  int rv = tcadbvsiz(adb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* iterinit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_iterinit
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  return tcadbiterinit(adb);
}


/* iternext */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_ADB_iternext
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  int vsiz;
  char *vbuf = tcadbiternext(adb, &vsiz);
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
JNIEXPORT jobject JNICALL Java_tokyocabinet_ADB_fwmkeys
(JNIEnv *env, jobject self, jbyteArray prefix, jint max){
  if(!prefix){
    throwillarg(env);
    return NULL;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  jboolean icp;
  jbyte *pbuf = (*env)->GetByteArrayElements(env, prefix, &icp);
  if(!pbuf){
    throwoutmem(env);
    return NULL;
  }
  int psiz = (*env)->GetArrayLength(env, prefix);
  TCLIST *tkeys = tcadbfwmkeys(adb, pbuf, psiz, max);
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
JNIEXPORT jint JNICALL Java_tokyocabinet_ADB_addint
(JNIEnv *env, jobject self, jbyteArray key, jint num){
  if(!key){
    throwillarg(env);
    return 0;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  num = tcadbaddint(adb, kbuf, ksiz, num);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return num;
}


/* adddouble */
JNIEXPORT jdouble JNICALL Java_tokyocabinet_ADB_adddouble
(JNIEnv *env, jobject self, jbyteArray key, jdouble num){
  if(!key){
    throwillarg(env);
    return 0.0;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0.0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  num = tcadbadddouble(adb, kbuf, ksiz, num);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return num;
}


/* sync */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_sync
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  return tcadbsync(adb);
}


/* optimize */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_optimize
(JNIEnv *env, jobject self, jstring params){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  jboolean icp;
  const char *tparams;
  if(params){
    tparams = (*env)->GetStringUTFChars(env, params, &icp);
    if(!tparams){
      throwoutmem(env);
      return false;
    }
  } else {
    icp = false;
    tparams = NULL;
  }
  bool rv = tcadboptimize(adb, tparams);
  if(icp) (*env)->ReleaseStringUTFChars(env, params, tparams);
  return rv;
}


/* vanish */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_vanish
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  return tcadbvanish(adb);
}


/* copy */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_copy
(JNIEnv *env, jobject self, jstring path){
  if(!path){
    throwillarg(env);
    return false;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  bool rv = tcadbcopy(adb, tpath);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* tranbegin */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_tranbegin
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  return tcadbtranbegin(adb);
}


/* trancommit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_trancommit
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  return tcadbtrancommit(adb);
}


/* tranabort */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_ADB_tranabort
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  return tcadbtranabort(adb);
}


/* path */
JNIEXPORT jstring JNICALL Java_tokyocabinet_ADB_path
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  const char *path = tcadbpath(adb);
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
JNIEXPORT jlong JNICALL Java_tokyocabinet_ADB_rnum
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  return tcadbrnum(adb);
}


/* size */
JNIEXPORT jlong JNICALL Java_tokyocabinet_ADB_size
(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  return tcadbsize(adb);
}


/* initialize */
JNIEXPORT void JNICALL
Java_tokyocabinet_ADB_initialize(JNIEnv *env, jobject self){
  TCADB *adb = tcadbnew();
  (*env)->SetLongField(env, self, adb_fid_ptr, (intptr_t)adb);
}


/* destruct */
JNIEXPORT void JNICALL
Java_tokyocabinet_ADB_destruct(JNIEnv *env, jobject self){
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  tcadbdel(adb);
}


/* misc */
JNIEXPORT jobjectArray JNICALL Java_tokyocabinet_ADB_miscimpl
(JNIEnv *env, jobject self, jstring name, jobjectArray args){
  if(!name){
    throwillarg(env);
    return NULL;
  }
  TCADB *adb = (TCADB *)(intptr_t)(*env)->GetLongField(env, self, adb_fid_ptr);
  jboolean icn;
  const char *tname = (*env)->GetStringUTFChars(env, name, &icn);
  if(!tname){
    throwoutmem(env);
    return NULL;
  }
  int anum = (*env)->GetArrayLength(env, args);
  TCLIST *targs = tclistnew2(anum);
  for(int i = 0; i < anum; i++){
    jobject arg = (*env)->GetObjectArrayElement(env, args, i);
    jboolean ica;
    jbyte *abuf = (*env)->GetByteArrayElements(env, arg, &ica);
    if(!abuf){
      throwoutmem(env);
      return NULL;
    }
    int asiz = (*env)->GetArrayLength(env, arg);
    tclistpush(targs, abuf, asiz);
    if(ica) (*env)->ReleaseByteArrayElements(env, arg, abuf, JNI_ABORT);
    (*env)->DeleteLocalRef(env, arg);
  }
  jobject res = NULL;
  TCLIST *tres = tcadbmisc(adb, tname, targs);
  if(tres){
    int rnum = tclistnum(tres);
    res = (*env)->NewObjectArray(env, rnum, (*env)->FindClass(env, "[B"), NULL);
    for(int i = 0; i < rnum; i++){
      int rsiz;
      const char *rbuf = tclistval(tres, i, &rsiz);
      jbyteArray ary = (*env)->NewByteArray(env, rsiz);
      if(!ary){
        throwoutmem(env);
        return NULL;
      }
      (*env)->SetByteArrayRegion(env, ary, 0, rsiz, (jbyte *)rbuf);
      (*env)->SetObjectArrayElement(env, res, i, ary);
      (*env)->DeleteLocalRef(env, ary);
    }
    tclistdel(tres);
  }
  tclistdel(targs);
  if(icn) (*env)->ReleaseStringUTFChars(env, name, tname);
  return res;
}



/* END OF FILE */
