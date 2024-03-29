//
// Created by pengcheng.tan on 2024/3/7.
//
#include <jni.h>
#include <string>
#include "android_log.h"
#include "dumpstack.h"

static jclass gJavaClass = nullptr;
static jmethodID gStackCallbackMethodId = nullptr;
void stackCallback(JNIEnv *jni, long timestamp, bool isAnr) {
    LOGD("Callback receive stack.");
    jni->CallStaticVoidMethod(gJavaClass, gStackCallbackMethodId, (jlong)timestamp, (jboolean)isAnr);
}

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
                  jvm,
                  stackCallback);
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

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    LOGD("Jni load.");
    JNIEnv *jni;
    jvm->GetEnv((void **)&jni, JNI_VERSION_1_6);
    auto javaClass = jni->FindClass("com/tans/dumpstack/DumpStack");
    gJavaClass = static_cast<jclass>(jni->NewGlobalRef(javaClass));
    auto methodId = jni->GetStaticMethodID(javaClass, "stackCallback", "(JZ)V");
    gStackCallbackMethodId = methodId;
    return JNI_VERSION_1_6;
}

