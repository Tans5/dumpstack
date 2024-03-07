package com.tans.dumpstack

import android.content.Context
import androidx.annotation.Keep
import com.bytedance.android.bytehook.ByteHook
import java.io.File
import java.util.concurrent.atomic.AtomicBoolean

object DumpStack {

    private val isInit: AtomicBoolean = AtomicBoolean(false)

    fun init(context: Context) {
        val cacheDir = context.cacheDir
        val defaultAnrDir = File(cacheDir, "anr")
        val defaultStackDir = File(cacheDir, "stack")
        init(anrTraceDir = defaultAnrDir, stackTraceDir = defaultStackDir)
    }

    fun init(anrTraceDir: File, stackTraceDir: File) {
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
            System.loadLibrary("dumpstack")
            setDirs(anrTraceDir.canonicalPath, stackTraceDir.canonicalPath)
            val byteHookResult = ByteHook.init()
            DumpStackLog.d("ByteHook init result: $byteHookResult")
        } else {
            DumpStackLog.e("Already invoke init.")
        }

    }

    @Keep
    private external fun setDirs(anrTraceDir: String, stackTraceDir: String)

}