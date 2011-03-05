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


#include "tokyocabinet_Util.h"
#include "myconf.h"


/* private global variables */
static TCMAP *envmap;



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


/* version */
JNIEXPORT jstring JNICALL Java_tokyocabinet_Util_version
(JNIEnv *env, jclass cls){
  jstring jver = (*env)->NewStringUTF(env, tcversion);
  if(!jver){
    throwoutmem(env);
    return NULL;
  }
  return jver;
}


/* init */
JNIEXPORT void JNICALL Java_tokyocabinet_Util_init
(JNIEnv *env, jclass cls){
  TCMPOOL *mpool = tcmpoolglobal();
  envmap = tcmpoolmapnew(mpool);
}


/* packint */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_Util_packint
(JNIEnv *env, jclass cls, jint num){
  jbyteArray serial = (*env)->NewByteArray(env, sizeof(num));
  if(!serial){
    throwoutmem(env);
    return NULL;
  }
  (*env)->SetByteArrayRegion(env, serial, 0, sizeof(num), (jbyte *)&num);
  return serial;
}


/* unpackint */
JNIEXPORT jint JNICALL Java_tokyocabinet_Util_unpackint
(JNIEnv *env, jclass cls, jbyteArray serial){
  if(!serial){
    throwillarg(env);
    return 0;
  }
  jboolean ic;
  jbyte *buf = (*env)->GetByteArrayElements(env, serial, &ic);
  if(!buf){
    throwoutmem(env);
    return 0;
  }
  int size = (*env)->GetArrayLength(env, serial);
  int num;
  if(size == sizeof(num)){
    memcpy(&num, buf, sizeof(num));
  } else {
    num = 0;
  }
  if(ic) (*env)->ReleaseByteArrayElements(env, serial, buf, JNI_ABORT);
  return num;
}


/* packdouble */
JNIEXPORT jbyteArray JNICALL Java_tokyocabinet_Util_packdouble
(JNIEnv *env, jclass cls, jdouble num){
  jbyteArray serial = (*env)->NewByteArray(env, sizeof(num));
  if(!serial){
    throwoutmem(env);
    return NULL;
  }
  (*env)->SetByteArrayRegion(env, serial, 0, sizeof(num), (jbyte *)&num);
  return serial;
}


/* unpackdouble */
JNIEXPORT jdouble JNICALL Java_tokyocabinet_Util_unpackdouble
(JNIEnv *env, jclass cls, jbyteArray serial){
  if(!serial){
    throwillarg(env);
    return 0;
  }
  jboolean ic;
  jbyte *buf = (*env)->GetByteArrayElements(env, serial, &ic);
  if(!buf){
    throwoutmem(env);
    return 0;
  }
  int size = (*env)->GetArrayLength(env, serial);
  double num;
  if(size == sizeof(num)){
    memcpy(&num, buf, sizeof(num));
  } else {
    num = 0.0;
  }
  if(ic) (*env)->ReleaseByteArrayElements(env, serial, buf, JNI_ABORT);
  return num;
}


/* system */
JNIEXPORT jint JNICALL Java_tokyocabinet_Util_system
(JNIEnv *env, jclass cls, jstring cmd){
  jboolean iccmd;
  const char *tcmd = (*env)->GetStringUTFChars(env, cmd, &iccmd);
  if(!tcmd){
    throwoutmem(env);
    return false;
  }
  int rv = system(tcmd);
  if(iccmd) (*env)->ReleaseStringUTFChars(env, cmd, tcmd);
  return rv;
}


/* chdir */
JNIEXPORT jint JNICALL Java_tokyocabinet_Util_chdir
(JNIEnv *env, jclass cls, jstring path){
  jboolean icp;
  const char *tpath = (*env)->GetStringUTFChars(env, path, &icp);
  if(!tpath){
    throwoutmem(env);
    return false;
  }
  int rv = chdir(tpath);
  if(icp) (*env)->ReleaseStringUTFChars(env, path, tpath);
  return rv;
}


/* getcwd */
JNIEXPORT jstring JNICALL Java_tokyocabinet_Util_getcwd
(JNIEnv *env, jclass cls){
  char tpath[PATH_MAX];
  if(!getcwd(tpath, PATH_MAX)) return NULL;
  jstring path = (*env)->NewStringUTF(env, tpath);
  if(!path){
    throwoutmem(env);
    return false;
  }
  return path;
}


/* getpid */
JNIEXPORT jint JNICALL Java_tokyocabinet_Util_getpid
(JNIEnv *env, jclass cls){
  return getpid();
}


/* putenv */
JNIEXPORT jint JNICALL Java_tokyocabinet_Util_putenv
(JNIEnv *env, jclass cls, jstring name, jstring value){
  jboolean icn;
  const char *tname = (*env)->GetStringUTFChars(env, name, &icn);
  if(!tname){
    throwoutmem(env);
    return false;
  }
  jboolean icv;
  const char *tvalue = (*env)->GetStringUTFChars(env, value, &icv);
  if(!tvalue){
    throwoutmem(env);
    return false;
  }
  int len = strlen(tname) + strlen(tvalue) + 1;
  char expr[len+1];
  sprintf(expr, "%s=%s", tname, tvalue);
  tcmapput2(envmap, tname, expr);
  const char *cexpr = tcmapget2(envmap, tname);
  int rv = putenv((char *)cexpr);
  if(icv) (*env)->ReleaseStringUTFChars(env, value, tvalue);
  if(icn) (*env)->ReleaseStringUTFChars(env, name, tname);
  return rv;
}


/* getenv */
JNIEXPORT jstring JNICALL Java_tokyocabinet_Util_getenv
(JNIEnv *env, jclass cls, jstring name){
  jboolean icn;
  const char *tname = (*env)->GetStringUTFChars(env, name, &icn);
  if(!tname){
    throwoutmem(env);
    return false;
  }
  char *tval = getenv(tname);
  jstring value = tval ? (*env)->NewStringUTF(env, tval) : NULL;
  if(icn) (*env)->ReleaseStringUTFChars(env, name, tname);
  return value;
}



/* END OF FILE */
