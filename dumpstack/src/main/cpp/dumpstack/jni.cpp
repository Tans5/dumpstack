//
// Created by pengcheng.tan on 2024/3/7.
//
#include <jni.h>
#include <string>
#include "dumpstack.h"

extern "C" JNIEXPORT jint JNICALL
Java_com_tans_dumpstack_DumpStack_initDumpStackNative(
        JNIEnv* env,
        jobject /* this */,
        jstring anrTraceDir,
        jstring stackTraceDir) {
    jboolean copy = JNI_FALSE;
    JavaVM *jvm;
    env->GetJavaVM(&jvm);
    return initDumpStack((const char *) env->GetStringUTFChars(anrTraceDir, &copy),
                  env->GetStringUTFLength(anrTraceDir),
                  (const char *) env->GetStringUTFChars(stackTraceDir, &copy),
                  env->GetStringUTFLength(stackTraceDir),
                  jvm);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_tans_dumpstack_DumpStack_monitorAnrNative(
        JNIEnv* env,
        jobject /* this */) {
    return monitorAnr();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_tans_dumpstack_DumpStack_obtainCurrentStacksNative(
        JNIEnv* env,
        jobject /* this */) {
    return obtainCurrentStacks();
}

