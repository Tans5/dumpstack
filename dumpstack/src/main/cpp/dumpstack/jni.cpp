//
// Created by pengcheng.tan on 2024/3/7.
//
#include <jni.h>
#include <string>
#include "dumpstack.h"

extern "C" JNIEXPORT void JNICALL
Java_com_tans_dumpstack_DumpStack_setDirs(
        JNIEnv* env,
        jobject /* this */,
        jstring anrTraceDir,
        jstring stackTraceDir) {
    jboolean copy = JNI_FALSE;
    setDirs((const char *)env->GetStringUTFChars(anrTraceDir, &copy),
            env->GetStringUTFLength(anrTraceDir),
            (const char *)env->GetStringUTFChars(stackTraceDir, &copy),
            env->GetStringUTFLength(stackTraceDir));
}

