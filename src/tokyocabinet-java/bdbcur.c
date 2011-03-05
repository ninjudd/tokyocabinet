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


#include "tokyocabinet_BDBCUR.h"
#include "myconf.h"


/* private global variables */
static jclass cls_bdbcur;
static jfieldID bdbcur_fid_ptr;



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


/* init */
JNIEXPORT void JNICALL Java_tokyocabinet_BDBCUR_init
(JNIEnv *env, jclass cls){
  cls_bdbcur = cls;
  bdbcur_fid_ptr = (*env)->GetFieldID(env, cls, "ptr", "J");
}


/* first */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDBCUR_first
(JNIEnv *env, jobject self){
  BDBCUR *cur = (BDBCUR *)(intptr_t)(*env)->GetLongField(env, self, bdbcur_fid_ptr);
  return tcbdbcurfirst(cur);
}


/* last */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDBCUR_last
(JNIEnv *env, jobject self){
  BDBCUR *cur = (BDBCUR *)(intptr_t)(*env)->GetLongField(env, self, bdbcur_fid_ptr);
  return tcbdbcurlast(cur);
}


/* jump */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDBCUR_jump
(JNIEnv *env, jobject self, jbyteArray key){
  if(!key){
    throwillarg(env);
    return false;
  }
  BDBCUR *cur = (BDBCUR *)(intptr_t)(*env)->GetLongField(env, self, bdbcur_fid_ptr);
  jboolean ick;
  jbyte *kbuf = (*env)->GetByteArrayElements(env, key, &ick);
  if(!kbuf){
    throwoutmem(env);
    return false;
  }
  int ksiz = (*env)->GetArrayLength(env, key);
  bool rv = tcbdbcurjump(cur, kbuf, ksiz);
  if(ick) (*env)->ReleaseByteArrayElements(env, key, kbuf, JNI_ABORT);
  return rv;
}


/* prev */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDBCUR_prev
(JNIEnv *env, jobject self){
  BDBCUR *cur = (BDBCUR *)(intptr_t)(*env)->GetLongField(env, self, bdbcur_fid_ptr);
  return tcbdbcurprev(cur);
}


/* next */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDBCUR_next
(JNIEnv *env, jobject self){
  BDBCUR *cur = (BDBCUR *)(intptr_t)(*env)->GetLongField(env, self, bdbcur_fid_ptr);
  return tcbdbcurnext(cur);
}


/* put */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDBCUR_put
(JNIEnv *env, jobject self, jbyteArray val, jint cpmode){
  if(!val){
    throwillarg(env);
    return false;
  }
  BDBCUR *cur = (BDBCUR *)(intptr_t)(*env)->GetLongField(env, self, bdbcur_fid_ptr);
  jboolean icv;
  jbyte *vbuf = (*env)->GetByteArrayElements(env, val, &icv);
  if(!vbuf){
    throwoutmem(env);
    return false;
  }
  int vsiz = (*env)->GetArrayLength(env, val);
  bool rv = tcbdbcurput(cur, vbuf, vsiz, cpmode);
  if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
  return rv;
}


/* out */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_BDBCUR_out
(JNIEnv *env, jobject self){
  BDBCUR *cur = (BDBCUR *)(intptr_t)(*env)->GetLongField(env, self, bdbcur_fid_ptr);
  return tcbdbcurout(cur);
}


/* key */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_BDBCUR_key
(JNIEnv *env, jobject self){
  BDBCUR *cur = (BDBCUR *)(intptr_t)(*env)->GetLongField(env, self, bdbcur_fid_ptr);
  int ksiz;
  char *kbuf = tcbdbcurkey(cur, &ksiz);
  jbyteArray key;
  if(kbuf){
    key = (*env)->NewByteArray(env, ksiz);
    if(!key){
      throwoutmem(env);
      return NULL;
    }
    (*env)->SetByteArrayRegion(env, key, 0, ksiz, (jbyte *)kbuf);
    tcfree(kbuf);
  } else {
    key = NULL;
  }
  return key;
}


/* val */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_BDBCUR_val
(JNIEnv *env, jobject self){
  BDBCUR *cur = (BDBCUR *)(intptr_t)(*env)->GetLongField(env, self, bdbcur_fid_ptr);
  int vsiz;
  char *vbuf = tcbdbcurval(cur, &vsiz);
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


/* initialize */
JNIEXPORT void JNICALL Java_tokyocabinet_BDBCUR_initialize
(JNIEnv *env, jobject self, jobject bdb){
  jclass clsbdb = (*env)->GetObjectClass(env, bdb);
  jfieldID fidbdb = (*env)->GetFieldID(env, clsbdb, "ptr", "J");
  TCBDB *bdbptr = (TCBDB *)(intptr_t)(*env)->GetLongField(env, bdb, fidbdb);
  BDBCUR *cur = tcbdbcurnew(bdbptr);
  (*env)->SetLongField(env, self, bdbcur_fid_ptr, (intptr_t)cur);
}


/* destruct */
JNIEXPORT void JNICALL Java_tokyocabinet_BDBCUR_destruct
(JNIEnv *env, jobject self){
  BDBCUR *cur = (BDBCUR *)(intptr_t)(*env)->GetLongField(env, self, bdbcur_fid_ptr);
  tcbdbcurdel(cur);
}



/* END OF FILE */
