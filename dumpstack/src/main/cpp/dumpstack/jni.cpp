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
    jboolean copy = true;
    setDirs((const char *)env->GetStringChars(anrTraceDir, &copy),
            (const char *)env->GetStringChars(stackTraceDir, &copy));
}

