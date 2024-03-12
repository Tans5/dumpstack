//
// Created by pengcheng.tan on 2024/3/7.
//

#ifndef DUMPSTACK_DUMPSTACK_H
#define DUMPSTACK_DUMPSTACK_H

void setDirs(const char* anrTraceDir, int anrTraceDirLength, const char* stackTraceDir, int stackTraceDirLength, JavaVM* jvm);

void monitorAnr();

void obtainCurrentStacks(bool fromAnr);

#endif //DUMPSTACK_DUMPSTACK_H
