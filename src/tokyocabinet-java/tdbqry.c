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


#include "tokyocabinet_TDBQRY.h"
#include "myconf.h"


typedef struct {
  JNIEnv *env;
  jobject obj;
  jmethodID mid;
  jobject map;
  jmethodID midmapclear;
  jmethodID midmapput;
  jmethodID midmapentryset;
  jmethodID midsetiterator;
  jmethodID miditerhasnext;
  jmethodID miditernext;
  jmethodID midentrygk;
  jmethodID midentrygv;
  jmethodID midstringgb;
  jclass clsbyteary;
  jclass clsstring;
} TCPROCOP;


/* private global variables */
static jclass cls_tdbqry;
static jfieldID tdbqry_fid_ptr;


/* private function prototypes */
static int tcprocrec(const void *pkbuf, int pksiz, TCMAP *cols, TCPROCOP *procop);



/*************************************************************************************************
 * public objects
 *************************************************************************************************/


/* init */
JNIEXPORT void JNICALL Java_tokyocabinet_TDBQRY_init
(JNIEnv *env, jclass cls){
  cls_tdbqry = cls;
  tdbqry_fid_ptr = (*env)->GetFieldID(env, cls, "ptr", "J");
}


/* addcond */
JNIEXPORT void JNICALL Java_tokyocabinet_TDBQRY_addcond
(JNIEnv *env, jobject self, jstring name, jint op, jstring expr){
  if(!name || !expr){
    throwillarg(env);
    return;
  }
  TDBQRY *qry = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tdbqry_fid_ptr);
  jboolean icn;
  const char *tname = (*env)->GetStringUTFChars(env, name, &icn);
  jboolean ice;
  const char *texpr = (*env)->GetStringUTFChars(env, expr, &ice);
  tctdbqryaddcond(qry, tname, op, texpr);
  if(ice) (*env)->ReleaseStringUTFChars(env, expr, texpr);
  if(icn) (*env)->ReleaseStringUTFChars(env, name, tname);
}


/* setorder */
JNIEXPORT void JNICALL Java_tokyocabinet_TDBQRY_setorder
(JNIEnv *env, jobject self, jstring name, jint type){
  if(!name){
    throwillarg(env);
    return;
  }
  TDBQRY *qry = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tdbqry_fid_ptr);
  jboolean icn;
  const char *tname = (*env)->GetStringUTFChars(env, name, &icn);
  tctdbqrysetorder(qry, tname, type);
  if(icn) (*env)->ReleaseStringUTFChars(env, name, tname);
}


/* setlimit */
JNIEXPORT void JNICALL Java_tokyocabinet_TDBQRY_setlimit
(JNIEnv *env, jobject self, jint max, jint skip){
  TDBQRY *qry = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tdbqry_fid_ptr);
  tctdbqrysetlimit(qry, max, skip);
}


/* search */
JNIEXPORT jobject JNICALL Java_tokyocabinet_TDBQRY_search
(JNIEnv *env, jobject self){
  TDBQRY *qry = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tdbqry_fid_ptr);
  TCLIST *tkeys = tctdbqrysearch(qry);
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
  return pkeys;
}


/* searchout */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDBQRY_searchout
(JNIEnv *env, jobject self){
  TDBQRY *qry = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tdbqry_fid_ptr);
  return tctdbqrysearchout(qry);
}


/* proc */
JNIEXPORT jboolean JNICALL Java_tokyocabinet_TDBQRY_proc
(JNIEnv *env, jobject self, jobject qp){
  if(!qp){
    throwillarg(env);
    return false;
  }
  TDBQRY *qry = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tdbqry_fid_ptr);
  jclass clsqp = (*env)->GetObjectClass(env, qp);
  jclass clsmap = (*env)->FindClass(env, CLSHASHMAP);
  jclass clsset = (*env)->FindClass(env, CLSSET);
  jclass clsiter = (*env)->FindClass(env, CLSITERATOR);
  jclass clsentry = (*env)->FindClass(env, CLSMAPENTRY);
  jclass clsbyteary = (*env)->FindClass(env, "[B");
  jclass clsstring = (*env)->FindClass(env, "L" CLSSTRING ";");
  jmethodID midmapinit = (*env)->GetMethodID(env, clsmap, "<init>", "()V");
  jobject map = (*env)->NewObject(env, clsmap, midmapinit);
  TCPROCOP procop;
  procop.env = env;
  procop.obj = qp;
  procop.mid = (*env)->GetMethodID(env, clsqp, "proc", "([BL" CLSMAP ";)I");
  procop.map = map;
  procop.midmapclear = (*env)->GetMethodID(env, clsmap, "clear", "()V");
  procop.midmapput = (*env)->GetMethodID(env, clsmap, "put",
                                         "(L" CLSOBJECT ";L" CLSOBJECT ";)L" CLSOBJECT ";");
  procop.midmapentryset = (*env)->GetMethodID(env, clsmap, "entrySet", "()L" CLSSET ";");
  procop.midsetiterator = (*env)->GetMethodID(env, clsset, "iterator", "()L" CLSITERATOR ";");
  procop.miditerhasnext = (*env)->GetMethodID(env, clsiter, "hasNext", "()Z");
  procop.miditernext = (*env)->GetMethodID(env, clsiter, "next", "()L" CLSOBJECT ";");
  procop.midentrygk = (*env)->GetMethodID(env, clsentry, "getKey", "()L" CLSOBJECT ";");
  procop.midentrygv = (*env)->GetMethodID(env, clsentry, "getValue", "()L" CLSOBJECT ";");
  procop.midstringgb = (*env)->GetMethodID(env, clsstring, "getBytes", "()[B");
  procop.clsbyteary = clsbyteary;
  procop.clsstring = clsstring;
  return tctdbqryproc(qry, (TDBQRYPROC)tcprocrec, &procop);
}


/* hint */
JNIEXPORT jstring JNICALL Java_tokyocabinet_TDBQRY_hint
(JNIEnv *env, jobject self){
  TDBQRY *qry = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tdbqry_fid_ptr);
  const char *hint = tctdbqryhint(qry);
  jstring jhint;
  if(hint){
    jhint = (*env)->NewStringUTF(env, hint);
    if(!hint){
      throwoutmem(env);
      return NULL;
    }
  } else {
    jhint = NULL;
  }
  return jhint;
}


/* metasearch */
JNIEXPORT jobject JNICALL Java_tokyocabinet_TDBQRY_metasearch
(JNIEnv *env, jobject self, jobjectArray others, jint type){
  TDBQRY *qry = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tdbqry_fid_ptr);
  int onum = (*env)->GetArrayLength(env, others);
  TDBQRY *qrys[onum+1];
  int qnum = 0;
  qrys[qnum++] = qry;
  jclass clsqry = (*env)->GetObjectClass(env, self);
  for(int i = 0; i < onum; i++){
    jobject oqry = (*env)->GetObjectArrayElement(env, others, i);
    if((*env)->IsInstanceOf(env, oqry, clsqry))
      qrys[qnum++] = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, oqry, tdbqry_fid_ptr);
  }
  TCLIST *tkeys = tctdbmetasearch(qrys, qnum, type);
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
  return pkeys;
}


/* initialize */
JNIEXPORT void JNICALL Java_tokyocabinet_TDBQRY_initialize
(JNIEnv *env, jobject self, jobject tdb){
  jclass clstdb = (*env)->GetObjectClass(env, tdb);
  jfieldID fidtdb = (*env)->GetFieldID(env, clstdb, "ptr", "J");
  TCTDB *tdbptr = (TCTDB *)(intptr_t)(*env)->GetLongField(env, tdb, fidtdb);
  TDBQRY *qry = tctdbqrynew(tdbptr);
  (*env)->SetLongField(env, self, tdbqry_fid_ptr, (intptr_t)qry);
}


/* destruct */
JNIEXPORT void JNICALL Java_tokyocabinet_TDBQRY_destruct
(JNIEnv *env, jobject self){
  TDBQRY *qry = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tdbqry_fid_ptr);
  tctdbqrydel(qry);
}


/* kwicimpl */
JNIEXPORT jobjectArray JNICALL Java_tokyocabinet_TDBQRY_kwicimpl
(JNIEnv *env, jobject self, jobjectArray cols, jstring name, jint width, jint opts){
  if(!cols){
    throwillarg(env);
    return NULL;
  }
  jclass clsstring = name ?
    (*env)->GetObjectClass(env, name) : (*env)->FindClass(env, CLSSTRING);
  TDBQRY *qry = (TDBQRY *)(intptr_t)(*env)->GetLongField(env, self, tdbqry_fid_ptr);
  jsize cnum = (*env)->GetArrayLength(env, cols);
  TCMAP *tcols = tcmapnew2(cnum + 1);
  jboolean icc = false;
  const char *cbuf = NULL;
  int csiz = 0;
  if(name){
    cbuf = (*env)->GetStringUTFChars(env, name, &icc);
    if(!cbuf){
      throwoutmem(env);
      return NULL;
    }
    csiz = strlen(cbuf);
  }
  cnum--;
  for(int i = 0; i < cnum; i += 2){
    name = (*env)->GetObjectArrayElement(env, cols, i);
    jboolean icn;
    jbyte *nbuf = (*env)->GetByteArrayElements(env, name, &icn);
    if(!nbuf){
      throwoutmem(env);
      return NULL;
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
    if(!cbuf || (nsiz == csiz && !memcmp(nbuf, cbuf, nsiz)))
      tcmapputkeep(tcols, nbuf, nsiz, vbuf, vsiz);
    if(icv) (*env)->ReleaseByteArrayElements(env, val, vbuf, JNI_ABORT);
    if(icn) (*env)->ReleaseByteArrayElements(env, name, nbuf, JNI_ABORT);
    (*env)->DeleteLocalRef(env, val);
    (*env)->DeleteLocalRef(env, name);
  }
  if(width < 0){
    width = 1 << 30;
    opts |= TCKWNOOVER | TCKWPULEAD;
  }
  TCLIST *texts = tctdbqrykwic(qry, tcols, cbuf, width, opts);
  int tnum = tclistnum(texts);
  jobjectArray ary = (*env)->NewObjectArray(env, tnum, clsstring, NULL);
  for(int i = 0; i < tnum; i++){
    jobject text = (*env)->NewStringUTF(env, tclistval2(texts, i));
    (*env)->SetObjectArrayElement(env, ary, i, text);
    (*env)->DeleteLocalRef(env, text);
  }
  tclistdel(texts);
  if(icc) (*env)->ReleaseStringUTFChars(env, name, cbuf);
  tcmapdel(tcols);
  return ary;
}



/*************************************************************************************************
 * private objects
 *************************************************************************************************/


/* process a record in iteration of a query result set */
static int tcprocrec(const void *pkbuf, int pksiz, TCMAP *cols, TCPROCOP *procop){
  JNIEnv *env = procop->env;
  jbyteArray pkey = (*env)->NewByteArray(env, pksiz);
  (*env)->SetByteArrayRegion(env, pkey, 0, pksiz, (jbyte *)pkbuf);
  jobject map = procop->map;
  (*env)->CallVoidMethod(env, map, procop->midmapclear);
  tcmapiterinit(cols);
  const char *nbuf;
  int nsiz;
  while((nbuf = tcmapiternext(cols, &nsiz)) != NULL){
    int vsiz;
    const char *vbuf = tcmapiterval(nbuf, &vsiz);
    jstring name = (*env)->NewStringUTF(env, nbuf);
    if(!name){
      throwoutmem(env);
      return TDBQPSTOP;
    }
    jbyteArray value = (*env)->NewByteArray(env, vsiz);
    if(!value){
      throwoutmem(env);
      return TDBQPSTOP;
    }
    (*env)->SetByteArrayRegion(env, value, 0, vsiz, (jbyte *)vbuf);
    (*env)->CallObjectMethod(env, map, procop->midmapput, name, value);
    (*env)->DeleteLocalRef(env, value);
    (*env)->DeleteLocalRef(env, name);
  }
  int flags = (*env)->CallIntMethod(env, procop->obj, procop->mid, pkey, map);
  if((*env)->ExceptionOccurred(env) != NULL) flags = TDBQPSTOP;
  if(flags & TDBQPPUT){
    tcmapclear(cols);
    jclass clsstring = procop->clsstring;
    jclass clsbyteary = procop->clsbyteary;
    jobject set = (*env)->CallObjectMethod(env, map, procop->midmapentryset);
    jobject iter = (*env)->CallObjectMethod(env, set, procop->midsetiterator);
    while((*env)->CallBooleanMethod(env, iter, procop->miditerhasnext)){
      jobject entry = (*env)->CallObjectMethod(env, iter, procop->miditernext);
      jobject name = (*env)->CallObjectMethod(env, entry, procop->midentrygk);
      jobject value = (*env)->CallObjectMethod(env, entry, procop->midentrygv);
      if((*env)->IsInstanceOf(env, name, clsstring)){
        jobject nary = (*env)->CallObjectMethod(env, name, procop->midstringgb);
        (*env)->DeleteLocalRef(env, name);
        name = nary;
      }
      if((*env)->IsInstanceOf(env, value, clsstring)){
        jobject vary = (*env)->CallObjectMethod(env, value, procop->midstringgb);
        (*env)->DeleteLocalRef(env, value);
        value = vary;
      }
      if(!(*env)->IsInstanceOf(env, name, clsbyteary) ||
         !(*env)->IsInstanceOf(env, value, clsbyteary)){
        throwillarg(env);
        return TDBQPSTOP;
      }
      jboolean icn;
      jbyte *nbuf = (*env)->GetByteArrayElements(env, name, &icn);
      if(!nbuf){
        throwoutmem(env);
        return TDBQPSTOP;
      }
      int nsiz = (*env)->GetArrayLength(env, name);
      jboolean icv;
      jbyte *vbuf = (*env)->GetByteArrayElements(env, value, &icv);
      if(!vbuf){
        throwoutmem(env);
        return TDBQPSTOP;
      }
      int vsiz = (*env)->GetArrayLength(env, value);
      tcmapputkeep(cols, nbuf, nsiz, vbuf, vsiz);
      if(icv) (*env)->ReleaseByteArrayElements(env, value, vbuf, JNI_ABORT);
      if(icn) (*env)->ReleaseByteArrayElements(env, name, nbuf, JNI_ABORT);
      (*env)->DeleteLocalRef(env, value);
      (*env)->DeleteLocalRef(env, name);
      (*env)->DeleteLocalRef(env, entry);
    }
    (*env)->DeleteLocalRef(env, iter);
    (*env)->DeleteLocalRef(env, set);
  }
  (*env)->DeleteLocalRef(env, pkey);
  return flags;
}



/* END OF FILE */
