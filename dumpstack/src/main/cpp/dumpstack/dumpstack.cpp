#include <jni.h>
#include <string>
#include "dumpstack.h"
#include "android_log.h"

const static char* gAnrTraceDir = nullptr;
const static char* gStackTraceDir = nullptr;

void setDirs(const char* anrTraceDir,
             int anrTraceDirLength,
             const char* stackTraceDir,
             int stackTraceDirLength) {
    if (gAnrTraceDir != nullptr) {
        free((void *)gAnrTraceDir);
    }
    if (gStackTraceDir != nullptr) {
        free((void *) gStackTraceDir);
    }
    void* anrTraceDirLocal = malloc(anrTraceDirLength + 1);
    void* stackTrackLocal = malloc(stackTraceDirLength + 1);
    memcpy(anrTraceDirLocal, anrTraceDir, anrTraceDirLength + 1);
    memcpy(stackTrackLocal, stackTraceDir, stackTraceDirLength + 1);
    gAnrTraceDir = (const char *) anrTraceDirLocal;
    gStackTraceDir = (const char *) stackTrackLocal;
    LOGD("AnrTraceDir: %s, StackTraceDir: %s", anrTraceDir, stackTraceDir);
}