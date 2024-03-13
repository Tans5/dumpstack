#include <jni.h>
#include <string>
#include <android/api-level.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/eventfd.h>
#include "dumpstack.h"
#include "android_log.h"
#include "xhook.h"
#include "utils.h"

const static char* gAnrTraceDir = nullptr;
const static char* gStackTraceDir = nullptr;
static int gSignalCatcherTid = -1;
static int gStackNotifyFd = -1;
static bool isInited = false;
static pthread_mutex_t *lock = nullptr;
static bool isMonitorAnrSig = false;

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
            read(gStackNotifyFd, &data, sizeof(data));
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

ssize_t (*origin_write)(int fd, const void *const buf, size_t count);

ssize_t my_write(int fd, const void *const buf, size_t count) {
    if (gSignalCatcherTid == gettid()) {
//        bytehook_unhook(gWriteStub);
//        gWriteStub = nullptr;
        LOGD("SignalCatcher write count: %d", count);
//        long time = get_time_millis();
//        char * stackFileName = new char[MAX_BUFFER_SIZE];
//        const char * dir;
//        if (waitingWriteStackFileFromAnr) {
//            dir = gAnrTraceDir;
//        } else {
//            dir = gStackTraceDir;
//        }
//        sprintf(stackFileName, "%s/%ld.text", dir, time);
//        LOGD("Create stack file: %s", stackFileName);
//        FILE *file = fopen(stackFileName, "w");
//        fwrite(buf, 1, count, file);
//        fclose(file);
//        if (gStackNotifyFd > 0) {
//            long data = 233L;
//            write(gStackNotifyFd, &data, sizeof(data));
//        }
    }
    return origin_write(fd, buf, count);
}

int hookSignalCatcherWrite() {
    int apiLevel = android_get_device_api_level();
    int signalCatcherTid = gSignalCatcherTid;
    if (signalCatcherTid <= 0) {
        signalCatcherTid = getSignalCatcherTid();
        gSignalCatcherTid = signalCatcherTid;
    }
    LOGD("ApiLevel: %d, SignalCatcherTid: %d", apiLevel, signalCatcherTid);
    if (signalCatcherTid <= 0) {
        LOGE("Get Signal Catcher tid fail.");
        return -1;
    }
    char *writeLibName;
    if (apiLevel >= 30 || apiLevel == 25 || apiLevel == 24) {
        writeLibName = ".*/libc\\.so$";
    } else if (apiLevel == 29) {
        writeLibName = ".*/libbase\\.so$";
    } else {
        writeLibName = ".*/libart\\.so$";
    }
    int ret = xhook_register(writeLibName,
                   "write",
                   (void *) my_write,
                             nullptr);
    LOGD("xhook hook write register result: %d", ret);
    if (ret == 0) {
        ret = xhook_refresh(1);
        LOGD("xhook hook write refresh result: %d", ret);
        return ret;
    } else {
        return ret;
    }
}

int initDumpStack(const char* anrTraceDir,
                   int anrTraceDirLength,
                   const char* stackTraceDir,
                   int stackTraceDirLength,
                   JavaVM *jvm) {
    if (lock == nullptr) {
        pthread_mutex_t *llock = new pthread_mutex_t;
        pthread_mutex_init(llock, nullptr);
        lock = llock;
    }
    pthread_mutex_lock(lock);
    int ret = 0;

    if (isInited) {
        ret = -1;
        pthread_mutex_unlock(lock);
        return ret;
    }

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

    origin_write = write;
    ret = hookSignalCatcherWrite();
    if (ret == 0) {
        gStackNotifyFd = eventfd(0, EFD_CLOEXEC);
        if (gStackNotifyFd > 0) {
            pthread_t stackHandleThread;
            ret = pthread_create(&stackHandleThread, nullptr, stackHandleRoutine, jvm);
            LOGD("Create stack handle thread: %ld, result: %d", stackHandleThread, ret);
            isInited = (ret == 0);
            goto end;
        } else {
            LOGE("Create stack notify fd fail.");
            goto end;
        }
    } else {
        goto end;
    }

    end:
        pthread_mutex_unlock(lock);
        return ret;
}

int monitorAnr() {
    if (lock != nullptr) {
        pthread_mutex_lock(lock);
        int ret;
        if (isInited) {
            if (!isMonitorAnrSig) {
                // TODO:

                ret = 0;
                isMonitorAnrSig = true;
            } else {
                LOGE("Do not invoke monitor anr multiple times.");
                ret = -1;
            }
            goto end;
        } else {
            ret = -1;
            LOGE("Not init.");
            goto end;
        }

        end:
           pthread_mutex_unlock(lock);
           return ret;
    } else {
        LOGE("Not init.");
        return -1;
    }
}



int obtainCurrentStacks() {
    if (lock != nullptr) {
        pthread_mutex_lock(lock);
        int ret;
        if (isInited) {
            // TODO:

            kill(getpid(), SIGQUIT);
            ret = 0;
            goto end;
        } else {
            ret = -1;
            goto end;
        }
        end:
           pthread_mutex_unlock(lock);
           return ret;
    } else {
        LOGE("Not init.");
        return -1;
    }
}