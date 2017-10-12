#!bin/bash

WORK_PATH="$PWD"
echo $WORK_PATH

cd app/build/intermediates/classes/debug

javah -classpath ./:/Users/micker/Documents/\=\=tools/android-sdk-macosx/platforms/android-L/android.jar -jni cn.micker.jnisafeimage.JNISafe

cp cn_micker_jnisafeimage_JNISafe.h ../../../../src/main/jni/

echo "done"
