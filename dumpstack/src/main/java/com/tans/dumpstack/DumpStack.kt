package com.tans.dumpstack

import android.content.Context
import android.os.SystemClock
import androidx.annotation.Keep
import com.bytedance.android.bytehook.ByteHook
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
            setDirsNative(anrTraceDir.canonicalPath, stackTraceDir.canonicalPath)
            val byteHookResult = ByteHook.init()
            DumpStackLog.d("ByteHook init result: $byteHookResult")
            if (monitorAnr) {
                DumpStackLog.d("Monitor anr.")
                monitorAnrNative()
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
                synchronized(this) {
                    lastObtainStackTime.set(now)
                    obtainCurrentStacksNative()
                }
            } else {
                DumpStackLog.e("Skip get stack, min interval is ${MIN_OBTAIN_STACK_INTERVAL}ms")
            }
        } else {
            DumpStackLog.e("ObtainCurrentStacks error, not init.")
        }
    }

    @Keep
    private external fun setDirsNative(anrTraceDir: String, stackTraceDir: String)

    @Keep
    private external fun monitorAnrNative()

    @Keep
    private external fun obtainCurrentStacksNative()

    private const val MIN_OBTAIN_STACK_INTERVAL = 1000L
}