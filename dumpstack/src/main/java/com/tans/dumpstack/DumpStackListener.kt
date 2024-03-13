package com.tans.dumpstack

import java.io.File

interface DumpStackListener {
    fun onAnr(timestamp: Long, stackFile: File)

    fun onStackRequest(timestamp: Long, stackFile: File)
}