//
// Created by Micker on 2017/10/9.
//

#ifndef JNISAFEIMAGE_COMMON_H
#define JNISAFEIMAGE_COMMON_H


#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>

#define TAG "MICKER_JNI_SAFE"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , TAG, __VA_ARGS__)

#define UTF_8 "UTF-8"
#define SAFE_IMAGE_NAME "jjjjffff.png"

#define RGBA_A(p) (((p) & 0xFF000000) >> 24)
#define RGBA_R(p) (((p) & 0x00FF0000) >> 16)
#define RGBA_G(p) (((p) & 0x0000FF00) >>  8)
#define RGBA_B(p)  ((p) & 0x000000FF)

#endif //JNISAFEIMAGE_COMMON_H
