//
// Created by pengcheng.tan on 2024/3/7.
//

#ifndef DUMPSTACK_DUMPSTACK_H
#define DUMPSTACK_DUMPSTACK_H

typedef struct DumpStackParams {
    JavaVM *jvm = nullptr;
    void (*stackCallback)(JNIEnv* jni, long timestamp, bool isAnr) = nullptr;
} DumpStackParams;

int initDumpStack(
        const char* anrTraceDir,
        int anrTraceDirLength,
        const char* stackTraceDir,
        int stackTraceDirLength,
        JavaVM *jvm,
        void (*stackCallback)(JNIEnv* jni, long timestamp, bool isAnr));

int monitorAnr();

int obtainCurrentStacks();

#endif //DUMPSTACK_DUMPSTACK_H
