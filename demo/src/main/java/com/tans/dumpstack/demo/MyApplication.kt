package com.tans.dumpstack.demo

import android.app.Application
import com.tans.dumpstack.DumpStack
import com.tans.dumpstack.DumpStackListener
import java.io.File

class MyApplication : Application() {

    override fun onCreate() {
        super.onCreate()
        DumpStack.init(this, true)

        DumpStack.addStackListener(object : DumpStackListener {
            override fun onAnr(timestamp: Long, stackFile: File) {
                println("Receive anr stack file: ${stackFile.canonicalPath}")
            }

            override fun onStackRequest(timestamp: Long, stackFile: File) {
                println("Receive request stack file: ${stackFile.canonicalPath}")
            }
        })
    }
}