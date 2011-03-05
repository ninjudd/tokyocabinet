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


#ifndef _MYCONF_H                        /* duplication check */
#define _MYCONF_H



/*************************************************************************************************
 * common settings
 *************************************************************************************************/


#include <jni.h>
#include <tcutil.h>
#include <tchdb.h>
#include <tcbdb.h>
#include <tcfdb.h>
#include <tctdb.h>
#include <tcadb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <unistd.h>

#define CLSOBJECT      "java/lang/Object"
#define CLSSTRING      "java/lang/String"
#define CLSLIST        "java/util/List"
#define CLSARRAYLIST   "java/util/ArrayList"
#define CLSMAP         "java/util/Map"
#define CLSHASHMAP     "java/util/HashMap"
#define CLSMAPENTRY    "java/util/Map$Entry"
#define CLSSET         "java/util/Set"
#define CLSITERATOR    "java/util/Iterator"
#define CLSEILLARG     "java/lang/IllegalArgumentException"
#define CLSEOUTMEM     "java/lang/OutOfMemoryError"
#define CLSHDB         "tokyocabinet/HDB"
#define CLSBDB         "tokyocabinet/BDB"
#define CLSBDBCUR      "tokyocabinet/BDBCUR"
#define CLSBDBCMP      "tokyocabinet/BDBCMP"


int isinstanceof(JNIEnv *env, jobject obj, const char *name);


void throwexception(JNIEnv *env, const char *name, const char *message);


void throwoutmem(JNIEnv *env);


void throwillarg(JNIEnv *env);



#endif                                   /* duplication check */


/* END OF FILE */
