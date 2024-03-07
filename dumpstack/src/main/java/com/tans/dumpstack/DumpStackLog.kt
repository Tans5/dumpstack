package com.tans.dumpstack

import android.util.Log

internal object DumpStackLog {

    fun d(msg: String) {
        Log.d(TAG, msg)
    }

    fun e(msg: String, e: Throwable? = null) {
        Log.e(TAG, msg, e)
    }

    private const val TAG = "DumpStackJava"
}