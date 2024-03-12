#include <jni.h>
#include <string>
#include <android/api-level.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include "dumpstack.h"
#include "android_log.h"
#include "bytehook.h"

// 8 KB
#define MAX_BUFFER_SIZE 1024 * 8

const static char* gAnrTraceDir = nullptr;
const static char* gStackTraceDir = nullptr;

long get_time_millis() {
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

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

static int gSignalCatcherTid = -1;

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

static int stackNotifyFd = -1;

void* stackHandleRoutine(void* args) {
    JavaVM *jvm = (JavaVM *) args;
    JavaVMAttachArgs jvmAttachArgs {
            .version = JNI_VERSION_1_6,
            .name = "CrashHandleThread",
            .group = nullptr
    };
    JNIEnv *jniEnv;
    int ret = jvm->AttachCurrentThread(&jniEnv, &jvmAttachArgs);
    if (ret == 0) {
        long data;
        while (true) {
            read(stackNotifyFd, &data, sizeof(data));
            if (data == 233L) {
                // Notify stack.
                LOGD("New stack...");
                // TODO:
            }
        }
    } else {
        LOGE("Attach thread to Jvm fail: %d", ret);
    }
    return nullptr;
}

void setDirs(const char* anrTraceDir,
             int anrTraceDirLength,
             const char* stackTraceDir,
             int stackTraceDirLength,
             JavaVM *jvm) {
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

    stackNotifyFd = eventfd(0, EFD_CLOEXEC);
    if (stackNotifyFd > 0) {
        pthread_t stackHandleThread;
        int ret = pthread_create(&stackHandleThread, nullptr, stackHandleRoutine, jvm);
        LOGD("Create stack handle thread: %ld, result: %d", stackHandleThread, ret);
    } else {
        LOGE("Create stack notify fd fail.");
    }
}

void monitorAnr() {
    // TODO:
}

// write hook.
ssize_t (*origin_write)(int fd, const void *const buf, size_t count);
bytehook_stub_t gWriteStub = nullptr;
static bool waitingWriteStackFileFromAnr = false;
ssize_t my_write(int fd, const void *const buf, size_t count) {
    if (gSignalCatcherTid == gettid()) {
        bytehook_unhook(gWriteStub);
        gWriteStub = nullptr;
        LOGD("SignalCatcher write count: %d", count);
        long time = get_time_millis();
        char * stackFileName = new char[MAX_BUFFER_SIZE];
        const char * dir;
        if (waitingWriteStackFileFromAnr) {
            dir = gAnrTraceDir;
        } else {
            dir = gStackTraceDir;
        }
        sprintf(stackFileName, "%s/%ld.text", dir, time);
        LOGD("Create stack file: %s", stackFileName);
        FILE *file = fopen(stackFileName, "w");
        fwrite(buf, 1, count, file);
        fclose(file);
        if (stackNotifyFd > 0) {
            long data = 233L;
            write(stackNotifyFd, &data, sizeof(data));
        }
    }
    return origin_write(fd, buf, count);
}
void my_write_hook_callback(bytehook_stub_t task_stub, int status_code, const char *caller_path_name,
                           const char *sym_name, void *new_func, void *prev_func, void *arg) {
    gWriteStub = task_stub;
    if (prev_func != nullptr) {
        origin_write = reinterpret_cast<ssize_t (*)(int fd, const void *const buf, size_t count)>(prev_func);
    }
    LOGD("Hook write method result: %d", status_code);
}

void obtainCurrentStacks(bool fromAnr) {
    int apiLevel = android_get_device_api_level();
    int signalCatcherTid = getSignalCatcherTid();
    gSignalCatcherTid = signalCatcherTid;
    LOGD("ApiLevel: %d, SignalCatcherTid: %d", apiLevel, signalCatcherTid);
    if (signalCatcherTid <= 0) {
        LOGE("Get Signal Catcher tid fail.");
        return;
    }

    if (gWriteStub != nullptr) {
        bytehook_unhook(gWriteStub);
        gWriteStub = nullptr;
    }
    char *writeLibName;
    if (apiLevel >= 30 || apiLevel == 25 || apiLevel == 24) {
        writeLibName = "libc.so";
    } else if (apiLevel == 29) {
        writeLibName = "libbase.so";
    } else {
        writeLibName = "libart.so";
    }
    bytehook_hook_single(
            writeLibName,
            nullptr,
            "write",
            (void *)my_write,
            my_write_hook_callback,
            nullptr);
    waitingWriteStackFileFromAnr = fromAnr;
    if (!fromAnr) {
        kill(getpid(), SIGQUIT);
    }
}