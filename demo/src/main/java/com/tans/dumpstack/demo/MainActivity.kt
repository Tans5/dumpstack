package com.tans.dumpstack.demo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.tans.dumpstack.DumpStack
import com.tans.dumpstack.DumpStackListener
import com.tans.dumpstack.demo.databinding.ActivityMainBinding
import java.io.File
import java.nio.charset.Charset
import java.text.SimpleDateFormat

class MainActivity : AppCompatActivity(), DumpStackListener {

    private val binding: ActivityMainBinding by lazy {
        ActivityMainBinding.inflate(layoutInflater)
    }

    private val timeFormat: SimpleDateFormat by lazy {
        SimpleDateFormat("yyyy-MM-dd HH:mm:ss")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(binding.root)
        binding.obtainStacksBt.setOnClickListener {
            DumpStack.obtainCurrentStacks()
        }
        DumpStack.addStackListener(this)
    }

    override fun onDestroy() {
        super.onDestroy()
        DumpStack.removeStackListener(this)
    }

    override fun onAnr(timestamp: Long, stackFile: File) {
        val timeStr = timeFormat.format(timestamp)
        val stackStr = stackFile.readText(Charset.defaultCharset())
        runOnUiThread {
            binding.typeAndTimeTv.text = "Anr: $timeStr"
            binding.stackTv.text = stackStr
        }
    }

    override fun onStackRequest(timestamp: Long, stackFile: File) {
        val timeStr = timeFormat.format(timestamp)
        val stackStr = stackFile.readText(Charset.defaultCharset())
        runOnUiThread {
            binding.typeAndTimeTv.text = "Request Stack: $timeStr"
            binding.stackTv.text = stackStr
        }
    }
}