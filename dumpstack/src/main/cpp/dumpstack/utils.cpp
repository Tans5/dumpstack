//
// Created by pengcheng.tan on 2024/3/13.
//
#include <unistd.h>
#include <string.h>
#include <asm-generic/fcntl.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/time.h>
#include "android_log.h"

// 8 KB
#define MAX_BUFFER_SIZE 1024 * 8


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
