package com.tans.dumpstack.demo

import android.app.Application
import com.tans.dumpstack.DumpStack

class MyApplication : Application() {

    override fun onCreate() {
        super.onCreate()
        DumpStack.init(this, true)
    }
}