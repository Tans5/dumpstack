package com.tans.dumpstack

import android.content.Context
import android.os.SystemClock
import androidx.annotation.Keep
import java.io.File
import java.util.concurrent.Executor
import java.util.concurrent.Executors
import java.util.concurrent.LinkedBlockingDeque
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicLong

@Keep
object DumpStack {

    private val dispatchExecutor: Executor by lazy {
        Executors.newSingleThreadExecutor { r ->
            val t = Thread(r, "DumpStack-Dispatcher")
            t
        }
    }

    private val dumpStackListeners: LinkedBlockingDeque<DumpStackListener> by lazy {
        LinkedBlockingDeque()
    }

    private val isInit: AtomicBoolean = AtomicBoolean(false)
    private val lastObtainStackTime: AtomicLong = AtomicLong()

    private var anrTraceDir: File? = null
    private var stackTraceDir: File? = null

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
            this.anrTraceDir = anrTraceDir
            this.stackTraceDir = stackTraceDir
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

    fun addStackListener(l: DumpStackListener) {
        if (!dumpStackListeners.contains(l)) {
            dumpStackListeners.add(l)
        }
    }

    fun removeStackListener(l: DumpStackListener) {
        dumpStackListeners.remove(l)
    }

    private external fun initDumpStackNative(anrTraceDir: String, stackTraceDir: String): Int

    private external fun monitorAnrNative(): Int

    private external fun obtainCurrentStacksNative(): Int

    private const val MIN_OBTAIN_STACK_INTERVAL = 1000L

    /**
     * For native call.
     */
    @JvmStatic
    fun stackCallback(timestamp: Long, isAnr: Boolean) {
        val dir: File? = if (isAnr) {
            anrTraceDir
        } else {
            stackTraceDir
        }
        if (dir == null) {
            DumpStackLog.e("Dir is null, can't handle stack callback.")
            return
        }
        val file = File(dir, "${timestamp}.text")
        if (!file.isFile) {
            DumpStackLog.e("Wrong stack file: ${file.canonicalPath}")
            return
        }
        dispatchStack(timestamp, isAnr, file)
    }

    private fun dispatchStack(timestamp: Long, isAnr: Boolean, f: File) {
        dispatchExecutor.execute {
            for (l in dumpStackListeners) {
                if (isAnr) {
                    l.onAnr(timestamp, f)
                } else {
                    l.onStackRequest(timestamp, f)
                }
            }
        }
    }
}