#include <jni.h>
#include <string>
#include "dumpstack.h"
#include "android_log.h"

const static char* gAnrTraceDir = nullptr;
const static char* gStackTraceDir = nullptr;

void setDirs(const char* anrTraceDir, const char* stackTraceDir) {
    gAnrTraceDir = anrTraceDir;
    gStackTraceDir = stackTraceDir;
    LOGD("AnrTraceDir: %s, StackTraceDir: %s", anrTraceDir, stackTraceDir);
}