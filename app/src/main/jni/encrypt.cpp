//
// Created by Micker on 2017/10/9.
//

#include "encrypt.hpp"
#include "common.h"

#define KA "17683f4512e52b8d725f6bdb9ae3408e"

jstring getSignatures(JNIEnv *env, jobject thizz, jobject thiz) {

    jclass native_clazz = env->GetObjectClass(thiz);

    jmethodID methodID_func = env->GetMethodID(native_clazz,
                                               "getPackageManager",
                                               "()Landroid/content/pm/PackageManager;");

    jobject package_manager = env->CallObjectMethod(thiz, methodID_func);

    jclass pm_clazz = env->GetObjectClass(package_manager);

    jmethodID methodID_pm = env->GetMethodID(pm_clazz,
                                             "getPackageInfo",
                                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    jmethodID methodID_packagename = env->GetMethodID(native_clazz,
                                                      "getPackageName",
                                                      "()Ljava/lang/String;");
    jstring name_str = static_cast<jstring>(env->CallObjectMethod(thiz,
                                                                  methodID_packagename));

    jobject package_info = env->CallObjectMethod(package_manager, methodID_pm,
                                                 name_str,
                                                 64);

    jclass pi_clazz = env->GetObjectClass(package_info);
    jfieldID fieldID_signatures = env->GetFieldID(pi_clazz,
                                                  "signatures",
                                                  "[Landroid/content/pm/Signature;");

    jobject signatur = env->GetObjectField(package_info, fieldID_signatures);
    jobjectArray signatures = reinterpret_cast<jobjectArray>(signatur);
    jobject signature = env->GetObjectArrayElement(signatures, 0);

    jclass s_clazz = env->GetObjectClass(signature);
    jmethodID methodID_cstring = env->GetMethodID(s_clazz,
                                                  "toCharsString",
                                                  "()Ljava/lang/String;");
    jstring sign = (jstring)env->CallObjectMethod(signature, methodID_cstring);

    return sign;
}


jstring getPackageName(JNIEnv *env, jobject thizz, jobject thiz) {

    jclass native_clazz = env->GetObjectClass(thiz);

    jmethodID methodID_func = env->GetMethodID(native_clazz,
                                               "getPackageManager",
                                               "()Landroid/content/pm/PackageManager;");

    jobject package_manager = env->CallObjectMethod(thiz, methodID_func);

    jclass pm_clazz = env->GetObjectClass(package_manager);

    jmethodID methodID_pm = env->GetMethodID(pm_clazz,
                                             "getPackageInfo",
                                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    jmethodID methodID_packagename = env->GetMethodID(native_clazz,
                                                      "getPackageName",
                                                      "()Ljava/lang/String;");
    jstring name_str = static_cast<jstring>(env->CallObjectMethod(thiz, methodID_packagename));

    return name_str;
}


jbyteArray toBytes(JNIEnv *env, jstring string) {
    jclass string_cls = env->FindClass("java/lang/String");
    jmethodID get_bytes_mid = env->GetMethodID(string_cls,
                                               "getBytes",
                                               "(Ljava/lang/String;)[B");
    return reinterpret_cast<jbyteArray>(env->CallObjectMethod(string,
                                                              get_bytes_mid,
                                                              env->NewStringUTF(UTF_8)));
}

jbyteArray getDigestedBytes(JNIEnv *env, jbyteArray complex_bytes) {
    static jobject satic_message_digest_obj = __null;
    jclass message_digest_cls = env->FindClass("java/security/MessageDigest");
    jmethodID get_instance_mid = env->GetStaticMethodID(message_digest_cls,
                                                        "getInstance",
                                                        "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    if (satic_message_digest_obj == __null) {
        jobject local_message_digest_obj = env->CallStaticObjectMethod(
                message_digest_cls, get_instance_mid, env->NewStringUTF("MD5"));
        satic_message_digest_obj = env->NewGlobalRef(local_message_digest_obj);
        env->DeleteLocalRef(local_message_digest_obj);
    }
    jmethodID digest_mid = env->GetMethodID(message_digest_cls, "digest", "([B)[B");
    env->DeleteLocalRef(message_digest_cls);
    return reinterpret_cast<jbyteArray>(env->CallObjectMethod(
            satic_message_digest_obj, digest_mid, complex_bytes));
}


jstring toHex(JNIEnv *env, jbyteArray digested_bytes) {
    jclass big_integer_cls = env->FindClass("java/math/BigInteger");
    jmethodID new_big_integer_mid = env->GetMethodID(big_integer_cls,
                                                     "<init>",
                                                     "(I[B)V");

    jobject big_integer_obj = env->NewObject(big_integer_cls,
                                             new_big_integer_mid, 1, digested_bytes);
    env->DeleteLocalRef(digested_bytes);
    jmethodID to_String_mid = env->GetMethodID(big_integer_cls,
                                               "toString",
                                               "(I)Ljava/lang/String;");
    env->DeleteLocalRef(big_integer_cls);
    return reinterpret_cast<jstring>(env->CallObjectMethod(big_integer_obj,
                                                           to_String_mid, 16));
}

jstring getMD5(JNIEnv *env, jstring jInfo) {
    jbyteArray digested_bytes = getDigestedBytes(env, toBytes(env, jInfo));
    return toHex(env, digested_bytes);
}

char* jstringToChar(JNIEnv *env, jstring jstr) {
    char* pStr = NULL;
    jclass     jstrObj   = env->FindClass("java/lang/String");
    jstring    encode    = env->NewStringUTF("utf-8");
    jmethodID  methodId  = env->GetMethodID(jstrObj, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray byteArray = (jbyteArray)env->CallObjectMethod(jstr, methodId, encode);
    jsize      strLen    = env->GetArrayLength(byteArray);
    jbyte      *jBuf     = env->GetByteArrayElements(byteArray, JNI_FALSE);
    if (*jBuf > 0)
    {
        pStr = (char*)malloc(strLen + 1);
        if (!pStr)
        {
            return NULL;
        }
        memcpy(pStr, jBuf, strLen);
        pStr[strLen] = 0;
    }
    env->ReleaseByteArrayElements(byteArray, jBuf, 0);
    return pStr;
}

uint32_t create_pixel(char aChar, bool isEnd) {
    uint8_t r = isEnd?0:(uint8_t)aChar;
    uint32_t result =  ( 255 << 24 | r <<16 | r << 8 | r );
    return result;
}

jobject createBitmap(JNIEnv * env,int width, int height , uint32_t* _storedBitmapPixels){

    jclass bitmapCls = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapFunction = env->GetStaticMethodID(bitmapCls,
                                                            "createBitmap",
                                                            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");

    jstring configName = env->NewStringUTF("ARGB_8888");
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(bitmapConfigClass,
                                                                   "valueOf",
                                                                   "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");

    jobject bitmapConfig = env->CallStaticObjectMethod(bitmapConfigClass,
                                                       valueOfBitmapConfigFunction, configName);
    jobject newBitmap = env->CallStaticObjectMethod(bitmapCls,
                                                    createBitmapFunction, width , height, bitmapConfig);
    int ret;
    void* bitmapPixels;
    if ((ret = AndroidBitmap_lockPixels(env, newBitmap, &bitmapPixels)) < 0)
    {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return NULL;
    }
    uint32_t* newBitmapPixels = (uint32_t*) bitmapPixels;
    int pixelsCount = width*height;
    memcpy(newBitmapPixels, _storedBitmapPixels,sizeof(uint32_t) * pixelsCount);
    AndroidBitmap_unlockPixels(env, newBitmap);
    return newBitmap;
}


jobject saveTextsToImage(JNIEnv *env, jclass jj, jobject context, jobjectArray strArray) {

    jsize row = env->GetArrayLength(strArray);
    jstring jstr;
    row++;
    char **pstr = (char **) malloc(row*sizeof(char *));
    int i=0;
    int maxLength = -1;


    for (i=0 ; i<row; i++) {
        if(i != row -1) {
            jstr = (jstring)env->GetObjectArrayElement(strArray, i);
        } else {
            jstr = getMD5(env,getSignatures(env,jj,context));
        }
        pstr[i] = (char *)env->GetStringUTFChars(jstr, 0);

        int cLength = strlen(pstr[i]);
        if( cLength > maxLength) {
            maxLength = cLength;
        }
    }
    uint32_t* newBitmapPixels = new uint32_t[row * maxLength];
    for(int j=0;j<row;j++) {
        int cLength = strlen(pstr[j]);
        for(int i=0;i<cLength;i++){
            newBitmapPixels[j*maxLength+i] = create_pixel(pstr[j][i], false);
        }
        for (int i = cLength; i < maxLength; ++i) {
            newBitmapPixels[j*maxLength+i] = create_pixel('a', true);
        }
    }
    free(pstr);
    return createBitmap(env,maxLength,row,newBitmapPixels);
}


jobject getBitmapFromAsset(JNIEnv *env, jobject context) {

    jclass assetClass = env->FindClass("android/content/Context");
    jmethodID getAssetsMethod = env->GetMethodID(assetClass,
                                                 "getAssets",
                                                 "()Landroid/content/res/AssetManager;");
    jobject assetMangaer = env->CallObjectMethod(context,getAssetsMethod);


    jclass inputStreamClass = env->FindClass("java/io/InputStream");
    jclass assetManagerClass = env->FindClass("android/content/res/AssetManager");
    jmethodID openMethod = env->GetMethodID(assetManagerClass,
                                            "open",
                                            "(Ljava/lang/String;)Ljava/io/InputStream;");

    jstring openFileName = env->NewStringUTF(SAFE_IMAGE_NAME);
    jobject inputStream = env->CallObjectMethod(assetMangaer,openMethod,openFileName);
    if (NULL == inputStream) {
        return NULL;
    }

    jclass bitmapFactoryClasss = env->FindClass("android/graphics/BitmapFactory");

    jmethodID decodeStreamMethod = env->GetStaticMethodID(bitmapFactoryClasss,
                                                          "decodeStream",
                                                          "(Ljava/io/InputStream;)Landroid/graphics/Bitmap;");
    jobject bitmap = env->CallStaticObjectMethod(bitmapFactoryClasss,decodeStreamMethod,inputStream);

    return bitmap;
}


jobjectArray  getSafeData(JNIEnv *env, jclass classObject, jobject context) {
    jobject bitmap = getBitmapFromAsset(env,context);
    if (NULL == bitmap) {
        return NULL;
    }

    AndroidBitmapInfo info;
    memset(&info, 0, sizeof(info));
    AndroidBitmap_getInfo(env, bitmap, &info);

    void * pixels = NULL;
    int res = AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if (pixels == NULL) {
        return NULL;
    }

    jclass Cls = env->FindClass("java/lang/String");
    jobjectArray result = env->NewObjectArray((jsize)info.height, Cls , env->NewStringUTF(""));

    for (int y = 0; y < info.height; ++y) {
        char *pstr = (char *) malloc((info.width + 1)*sizeof(char *));

        int x = 0;
        for ( x = 0; x < info.width; ++x) {
            int a = 0, r = 0, g = 0, b = 0;
            void *pixel = ((uint32_t *)pixels) + y * info.width + x;
            uint32_t v = *(uint32_t *)pixel;
            a = RGBA_A(v);
            r = RGBA_R(v);
            g = RGBA_G(v);
            b = RGBA_B(v);

            char value = (char) r;

            pstr[x] = value;

            if(0 == (r + g + b)) {
                break;
            }
        }
        pstr[x] = '\0';
        jstring ivalue = env->NewStringUTF(pstr);
        env->SetObjectArrayElement(Array,y,ivalue);
        free(pstr);
    }

    AndroidBitmap_unlockPixels(env,bitmap);

    char * check = jstringToChar(env,(jstring)env->GetObjectArrayElement(Array,info.height-1));
    if(0 != strcmp(KA,check))
        return NULL;
    return Array;
}