#include <jni.h>
#include <string>
#include <android/api-level.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <sys/syscall.h>
#include "dumpstack.h"
#include "android_log.h"
#include "xhook.h"
#include "utils.h"

// 8 KB
#define MAX_BUFFER_SIZE 1024 * 8

const static char* gAnrTraceDir = nullptr;
const static char* gStackTraceDir = nullptr;
static int gSignalCatcherTid = -1;
static int gStackNotifyFd = -1;
static bool isInited = false;
static pthread_mutex_t *lock = nullptr;
static bool isMonitorAnrSig = false;
static struct sigaction originAnrSigaction {};

enum DumpStackState {
    NO_DUMP,
    WAITING_STACK_DUMP,
    WAITING_ANR_DUMP
};
static DumpStackState dumpState = NO_DUMP;

void* stackHandleRoutine(void* args) {
    DumpStackParams *params = (DumpStackParams *) args;
    JavaVMAttachArgs jvmAttachArgs {
            .version = JNI_VERSION_1_6,
            .name = "StackHandleThread",
            .group = nullptr
    };
    JavaVM *jvm = params->jvm;
    auto callback = params->stackCallback;
    JNIEnv *jniEnv;
    int ret = jvm->AttachCurrentThread(&jniEnv, &jvmAttachArgs);
    params->jvm = nullptr;
    params->stackCallback = nullptr;
    free(params);
    if (ret == 0) {
        int epollFd = epoll_create(1);
        if (epollFd > 0) {
            struct epoll_event epollEvent;
            memset(&epollEvent, 0, sizeof(epollEvent));
            epollEvent.events = EPOLLIN; // Can read.
            epollEvent.data.fd = gStackNotifyFd;
            ret = epoll_ctl(epollFd, EPOLL_CTL_ADD, gStackNotifyFd, &epollEvent);
            LOGD("StackHandleThread launched: %d", ret);
            long data;
            while (true) {
                // wait infinity
                int eventCount = epoll_wait(epollFd, &epollEvent, 1, -1);
                if (eventCount > 0) {
                    pthread_mutex_lock(lock);
                    int readSize = read(epollEvent.data.fd, &data, sizeof(data));
                    LOGD("StackHandleThread read event: %ld, size: %d", data, readSize);
                    if (data > 0L) {
                        // Notify stack.
                        callback(jniEnv, data, dumpState == WAITING_ANR_DUMP);
                    }
                    goto end;
                    end:
                    dumpState = NO_DUMP;
                    pthread_mutex_unlock(lock);
                }
            }
        } else {
            LOGE("Create epoll fail.");
        }
    } else {
        LOGE("Attach thread to Jvm fail: %d", ret);
    }
    jvm->DetachCurrentThread();
    return nullptr;
}

ssize_t (*origin_write)(int fd, const void *const buf, size_t count);

ssize_t my_write(int fd, const void *const buf, size_t count) {
    if (gSignalCatcherTid == gettid()) {
        pthread_mutex_lock(lock);
        if (dumpState != NO_DUMP) {
            LOGD("SignalCatcher write count: %d", count);
            long time = get_time_millis();
            char *stackFileName = new char[MAX_BUFFER_SIZE];
            const char * dir;
            if (dumpState == WAITING_STACK_DUMP) {
                dir = gStackTraceDir;
                LOGD("Start stack dump.");
            } else {
                dir = gAnrTraceDir;
                LOGD("Start anr dump.");
            }
            sprintf(stackFileName, "%s/%ld.text", dir, time);
            LOGD("Create stack file: %s", stackFileName);
            int fileFd = open(stackFileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            if (fileFd < 0) {
                LOGE("Create file fail: %d", fd);
                goto end;
            }
            LOGD("Start write stack file.");
            int writeSize = origin_write(fileFd, buf, count);
            close(fileFd);
            LOGD("Write stack file success, size: %d", writeSize);
            int notifyWriteSize = origin_write(gStackNotifyFd, &time, sizeof(time));
            LOGD("Notify write size: %d", notifyWriteSize);
            goto end;
        } else {
            goto end;
        }
       end:
        pthread_mutex_unlock(lock);
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
                  JavaVM *jvm,
                  void (*stackCallback)(JNIEnv* jni, long timestamp, bool isAnr)) {
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
        gStackNotifyFd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
        if (gStackNotifyFd > 0) {
            pthread_t stackHandleThread;
            DumpStackParams* params = new DumpStackParams;
            params->jvm = jvm;
            params->stackCallback = stackCallback;
            ret = pthread_create(&stackHandleThread, nullptr, stackHandleRoutine, params);
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

static void anrSignalHandler(int sig, siginfo_t *sig_info, void *uc) {
    LOGD("Receive anr signal.");
    int fromPid1 = sig_info->_si_pad[3];
    int fromPid2 = sig_info->_si_pad[4];
    int myPid = getpid();
    if (fromPid1 != myPid && fromPid2 != myPid) {
        pthread_mutex_lock(lock);
        if (dumpState == NO_DUMP) {
            dumpState = WAITING_ANR_DUMP;
        } else {
            LOGE("Skip dump anr, because state: %d", dumpState);
        }
        pthread_mutex_unlock(lock);
    }
    syscall(SYS_tgkill, myPid, gSignalCatcherTid, SIGQUIT);
    // originAnrSigaction.sa_sigaction(sig, sig_info, uc);
}

int monitorAnr() {
    if (lock != nullptr) {
        pthread_mutex_lock(lock);
        int ret;
        if (isInited) {
            if (!isMonitorAnrSig) {
                sigset_t sig_sets;
                sigemptyset(&sig_sets);
                sigaddset(&sig_sets, SIGQUIT);
                pthread_sigmask(SIG_UNBLOCK, &sig_sets, nullptr);

                struct sigaction sigAction{};
                sigfillset(&sigAction.sa_mask);
                sigAction.sa_flags = SA_RESTART | SA_ONSTACK | SA_SIGINFO;
                sigAction.sa_sigaction = anrSignalHandler;
                ret = sigaction(SIGQUIT, &sigAction, &originAnrSigaction);
                if (ret == 0) {
                    LOGD("Monitor anr signal success.");
                } else {
                    LOGE("Monitor anr signal fail: %d", ret);
                }
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
            if (dumpState == NO_DUMP) {
                dumpState = WAITING_STACK_DUMP;
                syscall(SYS_tgkill, getpid(), gSignalCatcherTid, SIGQUIT);
                ret = 0;
            } else {
                LOGE("Contain no finish stack dump.");
                ret = -1;
            }
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