# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

project("dumpstack")

add_library(${CMAKE_PROJECT_NAME} SHARED
        bytehook/bytehook.h
        dumpstack/dumpstack.cpp
        dumpstack/dumpstack.h
        dumpstack/android_log.h
        dumpstack/jni.cpp
        xhook/xhook.h)

add_library(xhook SHARED
        xhook/queue.h
        xhook/tree.h
        xhook/xh_core.c
        xhook/xh_core.h
        xhook/xh_elf.c
        xhook/xh_elf.h
        xhook/xh_errno.h
        xhook/xh_jni.c
        xhook/xh_log.c
        xhook/xh_log.h
        xhook/xh_util.c
        xhook/xh_util.h
        xhook/xh_version.c
        xhook/xh_version.h
        xhook/xhook.h
        xhook/xhook.c)

target_link_libraries(xhook
        android
        log)

# bytehook
find_package(bytehook REQUIRED CONFIG)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        bytehook::bytehook
        xhook
)