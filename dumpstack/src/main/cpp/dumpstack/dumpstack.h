//
// Created by pengcheng.tan on 2024/3/7.
//

#ifndef DUMPSTACK_DUMPSTACK_H
#define DUMPSTACK_DUMPSTACK_H

int initDumpStack(const char* anrTraceDir, int anrTraceDirLength, const char* stackTraceDir, int stackTraceDirLength, JavaVM* jvm);

int monitorAnr();

int obtainCurrentStacks();

#endif //DUMPSTACK_DUMPSTACK_H
