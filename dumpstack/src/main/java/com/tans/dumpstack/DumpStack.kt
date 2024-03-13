package com.tans.dumpstack

import android.content.Context
import android.os.SystemClock
import androidx.annotation.Keep
import java.io.File
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicLong

object DumpStack {

    private val isInit: AtomicBoolean = AtomicBoolean(false)
    private val lastObtainStackTime: AtomicLong = AtomicLong()

    fun init(context: Context, monitorAnr: Boolean) {
        val cacheDir = context.cacheDir
        val defaultAnrDir = File(cacheDir, "anr")
        val defaultStackDir = File(cacheDir, "stack")
        init(anrTraceDir = defaultAnrDir, stackTraceDir = defaultStackDir, monitorAnr = monitorAnr)
    }

    fun init(anrTraceDir: File, stackTraceDir: File, monitorAnr: Boolean) {
        if (isInit.compareAndSet(false, true)) {
            DumpStackLog.d("Start init.")
            if (anrTraceDir.isFile) {
                isInit.set(false)
                error("${anrTraceDir.canonicalPath} is a file, need a dir.")
            }
            if (stackTraceDir.isFile) {
                isInit.set(false)
                error("${stackTraceDir.canonicalPath} is a file, need a dir.")
            }
            if (!anrTraceDir.isDirectory) {
                anrTraceDir.mkdirs()
            }
            if (!stackTraceDir.isDirectory) {
                stackTraceDir.mkdirs()
            }
            DumpStackLog.d("AnrTraceDir: ${anrTraceDir.canonicalPath}, StackTraceDir: ${stackTraceDir.canonicalPath}")
            System.loadLibrary("dumpstack")
            var result = initDumpStackNative(anrTraceDir.canonicalPath, stackTraceDir.canonicalPath)
            if (result == 0) {
                if (monitorAnr) {
                    DumpStackLog.d("Monitor anr.")
                    result = monitorAnrNative()
                    if (result != 0) {
                        DumpStackLog.e("Monitor anr fail.")
                    }
                }
            } else {
                DumpStackLog.e("Init dump stack fail: $result")
            }
        } else {
            DumpStackLog.e("Already invoke init.")
        }
    }
    fun obtainCurrentStacks() {
        if (isInit.get()) {
            val now = SystemClock.uptimeMillis()
            val last = lastObtainStackTime.get()
            if ((now - last) > MIN_OBTAIN_STACK_INTERVAL) {
                lastObtainStackTime.set(now)
                if (obtainCurrentStacksNative() != 0) {
                    DumpStackLog.e("Obtain current stacks fail.")
                }
            } else {
                DumpStackLog.e("Skip get stack, min interval is ${MIN_OBTAIN_STACK_INTERVAL}ms")
            }
        } else {
            DumpStackLog.e("ObtainCurrentStacks error, not init.")
        }
    }

    @Keep
    private external fun initDumpStackNative(anrTraceDir: String, stackTraceDir: String): Int

    @Keep
    private external fun monitorAnrNative(): Int

    @Keep
    private external fun obtainCurrentStacksNative(): Int

    private const val MIN_OBTAIN_STACK_INTERVAL = 1000L
}