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


#include "tokyocabinet_BDB.h"
#include "myconf.h"

typedef struct {
  JavaVM *vm;
  jobject obj;
  jmethodID mid;
} TCCMPOP;


/* private global variables */
static jclass cls_bdb;
static jfieldID bdb_fid_ptr;


/* private function prototypes */
static int tccmpobj(const char *aptr, int asiz, const char *bptr, int bsiz, TCCMPOP *cmpop);



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


/* ecode */
JNIEXPORT jstring JNICALL Java_tokyocabinet_BDB_errmsg
(JNIEnv *env, jclass cls, jint ecode){
  jstring jmsg = (*env)->NewStringUTF(env, tcbdberrmsg(ecode));
  if(!jmsg){
    throwoutmem(env);
    return NULL;
  }
  return jmsg;
}


/* init */
JNIEXPORT void JNICALL Java_tokyocabinet_BDB_init
(JNIEnv *env, jclass cls){
  cls_bdb = cls;
  bdb_fid_ptr = (*env)->GetFieldID(env, cls, "ptr", "J");
}


/* ecode */
JNIEXPORT jint JNICALL Java_tokyocabinet_BDB_ecode
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbecode(bdb);
}


/* setcomparator */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_setcomparator
(JNIEnv *env, jobject self, jobject cmp){
  if(!cmp){
    throwillarg(env);
    return false;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  TCCMPOP *cmpop = tcbdbcmpop(bdb);
  if(cmpop){
    (*env)->DeleteGlobalRef(env, cmpop->obj);
    tcfree(cmpop);
  }
  JavaVM *vm;
  (*env)->GetJavaVM(env, &vm);
  jclass clscmp = (*env)->GetObjectClass(env, cmp);
  cmpop = tcmalloc(sizeof(*cmpop));
  cmpop->vm = vm;
  cmpop->obj = (*env)->NewGlobalRef(env, cmp);
  cmpop->mid = (*env)->GetMethodID(env, clscmp, "compare", "([B[B)I");
  return tcbdbsetcmpfunc(bdb, (TCCMP)tccmpobj, cmpop);
}


/* setcmpfunc */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_setcmpfunc
(JNIEnv *env, jobject self, jint cmp){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  TCCMPOP *cmpop = tcbdbcmpop(bdb);
  if(cmpop){
    (*env)->DeleteGlobalRef(env, cmpop->obj);
    tcfree(cmpop);
  }
  switch(cmp){
  case tokyocabinet_BDB_CMPLEXICAL: return tcbdbsetcmpfunc(bdb, tccmplexical, NULL);
  case tokyocabinet_BDB_CMPDECIMAL: return tcbdbsetcmpfunc(bdb, tccmpdecimal, NULL);
  case tokyocabinet_BDB_CMPINT32: return tcbdbsetcmpfunc(bdb, tccmpint32, NULL);
  case tokyocabinet_BDB_CMPINT64: return tcbdbsetcmpfunc(bdb, tccmpint64, NULL);
  default: break;
  }
  tcbdbsetecode(bdb, TCEINVALID, __FILE__, __LINE__, __func__);
  return false;
}


/* tune */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_tune
(JNIEnv *env, jobject self, jint lmemb, jint nmemb, jlong bnum, jint apow, jint fpow, jint opts){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbtune(bdb, lmemb, nmemb, bnum, apow, fpow, opts);
}


/* setcache */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_setcache
(JNIEnv *env, jobject self, jint lcnum, jint ncnum){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbsetcache(bdb, lcnum, ncnum);
}


/* setxmsiz */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_setxmsiz
(JNIEnv *env, jobject self, jlong xmsiz){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbsetxmsiz(bdb, xmsiz);
}


/* setdfunit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_setdfunit
(JNIEnv *env, jobject self, jint dfunit){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbsetdfunit(bdb, dfunit);
}


/* open */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_open
(JNIEnv *env, jobject self, jstring path, jint omode){
  if(!path){
    throwillarg(env);
    return false;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  bool rv = tcbdbopen(bdb, tpath, omode);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* close */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_close
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbclose(bdb);
}


/* put */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_put
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
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
  bool rv = tcbdbput(bdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putkeey */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_putkeep
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
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
  bool rv = tcbdbputkeep(bdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putcat */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_putcat
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
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
  bool rv = tcbdbputcat(bdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putdup */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_putdup
(JNIEnv *env, jobject self, jbyteArray key, jbyteArray val){
  if(!key || !val){
    throwillarg(env);
    return false;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
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
  bool rv = tcbdbputdup(bdb, kbuf, ksiz, vbuf, vsiz);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* putlist */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_putlist
(JNIEnv *env, jobject self, jbyteArray key, jobject values){
  if(!key || !values){
    throwillarg(env);
    return false;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  jclass clslist = (*env)->GetObjectClass(env, values);
  jmethodID midit = (*env)->GetMethodID(env, clslist, "iterator", "()L" CLSITERATOR ";");
  jobject itobj = (*env)->CallObjectMethod(env, values, midit);
  jclass clsit = (*env)->GetObjectClass(env, itobj);
  jmethodID midhn = (*env)->GetMethodID(env, clsit, "hasNext", "()Z");
  jmethodID midnx = (*env)->GetMethodID(env, clsit, "next", "()L" CLSOBJECT ";");
  jclass clsbyteary = (*env)->FindClass(env, "[B");
  TCLIST *tvals = tclistnew();
  while((*env)->CallBooleanMethod(env, itobj, midhn)){
    jobject val = (*env)->CallObjectMethod(env, itobj, midnx);
    if(!(*env)->IsInstanceOf(env, val, clsbyteary)) continue;
    jboolean icv;
    jbyte *vbuf = (*env)->GetByteArrayElements(env, val, &icv);
    if(!vbuf){
      throwoutmem(env);
      return false;
    }
    int vsiz = (*env)->GetArrayLength(env, val);
    tclistpush(tvals, vbuf, vsiz);
    if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  }
  bool rv = tcbdbputdup3(bdb, kbuf, ksiz, tvals);
  tclistdel(tvals);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* out */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_out
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return false;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  bool rv = tcbdbout(bdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* outlist */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_outlist
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return false;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  bool rv = tcbdbout3(bdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* get */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_BDB_get
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return NULL;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return NULL;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  int vsiz;
  char *vbuf = tcbdbget(bdb, kbuf, ksiz, &vsiz);
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


/* getlist */
JNIEXPORT jobject JNICALL Java_tokyocabinet_BDB_getlist
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return NULL;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return NULL;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  TCLIST *tvals = tcbdbget4(bdb, kbuf, ksiz);
  jobject vals;
  if(tvals){
    jclass clslist = (*env)->FindClass(env, CLSARRAYLIST);
    jmethodID midinit = (*env)->GetMethodID(env, clslist, "<init>", "()V");
    vals = (*env)->NewObject(env, clslist, midinit);
    jmethodID midadd = (*env)->GetMethodID(env, clslist, "add", "(L" CLSOBJECT ";)Z");
    for(int i = 0; i < tclistnum(tvals); i++){
      int vsiz;
      const char *vbuf = tclistval(tvals, i, &vsiz);
      jbyteArray val = (*env)->NewByteArray(env, vsiz);
      (*env)->SetByteArrayRegion(env, val, 0, vsiz, (jbyte *)vbuf);
      (*env)->CallVoidMethod(env, vals, midadd, val);
      (*env)->DeleteLocalRef(env, val);
    }
    tclistdel(tvals);
  } else {
    vals = NULL;
  }
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return vals;
}


/* vnum */
JNIEXPORT jint JNICALL Java_tokyocabinet_BDB_vnum
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return 0;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  bool rv = tcbdbvnum(bdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* vsiz */
JNIEXPORT jint JNICALL Java_tokyocabinet_BDB_vsiz
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return 0;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  int rv = tcbdbvsiz(bdb, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* range */
JNIEXPORT jobject JNICALL Java_tokyocabinet_BDB_range
(JNIEnv *env, jobject self, jbyteArray bkey, jboolean binc,
 jbyteArray ekey, jboolean einc, jint max){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean icbk;
  jbyte *bkbuf;
  int bksiz;
  if(bkey){
    bkbuf = (*env)->GetByteArrayElements(env, bkey, &icbk);
    if(!bkbuf){
      throwoutmem(env);
      return NULL;
    }
    icbk = false;
    bksiz = (*env)->GetArrayLength(env, bkey);
  } else {
    bkbuf = NULL;
    bksiz = -1;
  }
  jboolean icek;
  jbyte *ekbuf;
  int eksiz;
  if(ekey){
    ekbuf = (*env)->GetByteArrayElements(env, ekey, &icek);
    if(!ekbuf){
      throwoutmem(env);
      return NULL;
    }
    icek = false;
    eksiz = (*env)->GetArrayLength(env, ekey);
  } else {
    ekbuf = NULL;
    eksiz = -1;
  }
  TCLIST *tkeys = tcbdbrange(bdb, bkbuf, bksiz, binc, ekbuf, eksiz, einc, max);
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
  if(icek) (*env)->ReleaseByteArrayElements(env, ekey, ekbuf, JNI_ABORT);
  if(icbk) (*env)->ReleaseByteArrayElements(env, bkey, bkbuf, JNI_ABORT);
  return keys;
}


/* fwmkeys */
JNIEXPORT jobject JNICALL Java_tokyocabinet_BDB_fwmkeys
(JNIEnv *env, jobject self, jbyteArray prefix, jint max){
  if(!prefix){
    throwillarg(env);
    return NULL;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean icp;
  jbyte *pbuf = (*env)->GetByteArrayElements(env, prefix, &icp);
  if(!pbuf){
    throwoutmem(env);
    return NULL;
  }
  int psiz = (*env)->GetArrayLength(env, prefix);
  TCLIST *tkeys = tcbdbfwmkeys(bdb, pbuf, psiz, max);
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
JNIEXPORT jint JNICALL Java_tokyocabinet_BDB_addint
(JNIEnv *env, jobject self, jbyteArray key, jint num){
  if(!key){
    throwillarg(env);
    return 0;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  num = tcbdbaddint(bdb, kbuf, ksiz, num);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return num;
}


/* adddouble */
JNIEXPORT jdouble JNICALL Java_tokyocabinet_BDB_adddouble
(JNIEnv *env, jobject self, jbyteArray key, jdouble num){
  if(!key){
    throwillarg(env);
    return 0.0;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return 0.0;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  num = tcbdbadddouble(bdb, kbuf, ksiz, num);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return num;
}


/* sync */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_sync
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbsync(bdb);
}


/* optimize */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_optimize
(JNIEnv *env, jobject self, jint lmemb, jint nmemb, jlong bnum, jint apow, jint fpow, jint opts){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdboptimize(bdb, lmemb, nmemb, bnum, apow, fpow, opts);
}


/* vanish */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_vanish
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbvanish(bdb);
}


/* copy */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_copy
(JNIEnv *env, jobject self, jstring path){
  if(!path){
    throwillarg(env);
    return false;
  }
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  bool rv = tcbdbcopy(bdb, tpath);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* tranbegin */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_tranbegin
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbtranbegin(bdb);
}


/* trancommit */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_trancommit
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbtrancommit(bdb);
}


/* tranabort */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDB_tranabort
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbtranabort(bdb);
}


/* path */
JNIEXPORT jstring JNICALL Java_tokyocabinet_BDB_path
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  const char *path = tcbdbpath(bdb);
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
JNIEXPORT jlong JNICALL Java_tokyocabinet_BDB_rnum
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbrnum(bdb);
}


/* fsiz */
JNIEXPORT jlong JNICALL Java_tokyocabinet_BDB_fsiz
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  return tcbdbfsiz(bdb);
}


/* initialize */
JNIEXPORT void JNICALL Java_tokyocabinet_BDB_initialize
(JNIEnv *env, jobject self){
  TCBDB *bdb = tcbdbnew();
  tcbdbsetmutex(bdb);
  (*env)->SetLongField(env, self, bdb_fid_ptr, (intptr_t)bdb);
}


/* destruct */
JNIEXPORT void JNICALL Java_tokyocabinet_BDB_destruct
(JNIEnv *env, jobject self){
  TCBDB *bdb = (TCBDB *)(intptr_t)(*env)->GetLongField(env, self, bdb_fid_ptr);
  TCCMPOP *cmpop = tcbdbcmpop(bdb);
  if(cmpop){
    (*env)->DeleteGlobalRef(env, cmpop->obj);
    tcfree(cmpop);
  }
  tcbdbdel(bdb);
}



/*************************************************************************************************
 * private objects
 *************************************************************************************************/


/* compare two keys by calling the custom comparator */
static int tccmpobj(const char *aptr, int asiz, const char *bptr, int bsiz, TCCMPOP *cmpop){
  JavaVM *vm = cmpop->vm;
  JNIEnv *env;
  (*vm)->AttachCurrentThread(vm, (void *)&env, NULL);
  jbyteArray a = (*env)->NewByteArray(env, asiz);
  (*env)->SetByteArrayRegion(env, a, 0, asiz, (jbyte *)aptr);
  jbyteArray b = (*env)->NewByteArray(env, bsiz);
  (*env)->SetByteArrayRegion(env, b, 0, bsiz, (jbyte *)bptr);
  int rv = (*env)->CallIntMethod(env, cmpop->obj, cmpop->mid, a, b);
  if((*env)->ExceptionOccurred(env) != NULL){
    (*env)->ExceptionClear(env);
    rv = 0;
  }
  (*env)->DeleteLocalRef(env, b);
  (*env)->DeleteLocalRef(env, a);
  return rv;
}



/* END OF FILE */
