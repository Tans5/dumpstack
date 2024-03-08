#include <jni.h>
#include <string>
#include <android/api-level.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include "dumpstack.h"
#include "android_log.h"

// 8 KB
#define MAX_BUFFER_SIZE 1024 * 8

const static char* gAnrTraceDir = nullptr;
const static char* gStackTraceDir = nullptr;

bool isNumberStr(char *str, int maxLen) {
    for (int i = 0; i < maxLen; i ++) {
        char c = str[i];
        if (c == '\0') {
            break;
        }
        if (c >= '0' && c <= '9') {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

int getSignalCatcherTid() {
    pid_t myPid = getpid();
    char *processPath = new char[MAX_BUFFER_SIZE];
    int size = sprintf(processPath, "/proc/%d/task", myPid);
    if (size >= MAX_BUFFER_SIZE) {
        LOGE("Read proc path fail, read buffer size: %d", size);
        return -1;
    }
    DIR *processDir = opendir(processPath);
    if (processDir) {
        int tid = -1;
        dirent * child = readdir(processDir);
        while (child != nullptr) {
            if (isNumberStr(child->d_name, 256)) {
                char *filePath = new char[MAX_BUFFER_SIZE];
                size = sprintf(filePath, "%s/%s/comm", processPath, child->d_name);
                if (size >= MAX_BUFFER_SIZE) {
                    continue;
                }
                char *threadName = new char[MAX_BUFFER_SIZE];
                int fd = open(filePath, O_RDONLY);
                size = read(fd, threadName, MAX_BUFFER_SIZE);
                close(fd);
                threadName[size - 1] = '\0';
                if (strcmp(threadName, "Signal Catcher") == 0) {
                    tid = atoi(child->d_name);
                    break;
                }
            }
            child = readdir(processDir);
        }
        closedir(processDir);
        return tid;
    } else {
        LOGE("Read process dir fail.");
    }
    return - 1;
}

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

void obtainCurrentStacks() {
    int signalCatcherTid = getSignalCatcherTid();
    LOGD("SignalCatcherTid: %d", signalCatcherTid);
}