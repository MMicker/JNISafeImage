//
// Created by Micker on 2017/10/9.
//

#include "cn_micker_jnisafeimage_JNISafe.h"
#include "common.h"
#include "encrypt.hpp"

JNIEXPORT jobject JNICALL Java_cn_micker_jnisafeimage_JNISafe_saveTextsToImage
        (JNIEnv *env, jclass classObject, jobject jobject1, jobjectArray jobjectArray1) {

    return saveTextsToImage(env,classObject,jobject1,jobjectArray1);
}

JNIEXPORT jobjectArray JNICALL Java_cn_micker_jnisafeimage_JNISafe_getDataFromImage
        (JNIEnv *env, jclass classObject, jobject jobject1) {

    return getSafeData(env,classObject,jobject1);
}