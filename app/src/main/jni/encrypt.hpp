//
// Created by Micker on 2017/10/9.
//

#ifndef JNISAFEIMAGE_ENCRYPT_HPP
#define JNISAFEIMAGE_ENCRYPT_HPP

#include "common.h"

jstring getSignatures(JNIEnv *env, jobject thizz, jobject thiz);

jobject saveTextsToImage(JNIEnv *env, jclass classObject, jobject context, jobjectArray strArray);

jobjectArray getSafeData(JNIEnv *env, jclass classObject, jobject context);

#endif //JNISAFEIMAGE_ENCRYPT_HPP
