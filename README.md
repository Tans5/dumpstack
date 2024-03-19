
## Summary

Monitor APP ANR and get all thread stack. 

## Usage

### Step1

Add jitpack maven in project gradle file:  

```Groovy
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        maven {
            url "https://www.jitpack.io"
        }
    }
}
```

Add dependency in module gradle file:  

```Groovy
depencencies {
    // ...
    implementation "com.github.Tans5:dumpstack:1.0.0"
    // ...
}
```

### Step2

Init library when app launch: 

```Kotlin

class MyApplication : Application() {
    
    override fun onCreate() {
        super.onCreate()
        DumpStack.init(this, true)
    }
}
```

### Step3

Listen ANR and stack request result: 

```Kotlin
DumpStack.addStackListener(object : DumpStackListener {
    override fun onAnr(timestamp: Long, stackFile: File) {
        println("Receive anr stack file: ${stackFile.canonicalPath}")
    }

    override fun onStackRequest(timestamp: Long, stackFile: File) {
        println("Receive request stack file: ${stackFile.canonicalPath}")
    }
})
```
If you want to get all thread stack, you could invoke blow method: 

```Kotlin
DumpStack.obtainCurrentStacks()
```


## Stack File Simple

```text

----- pid 15609 at 2024-03-08 18:47:04.992981711+0800 -----
Cmd line: com.tans.dumpstack.demo
Build fingerprint: 'OPPO/PGCM10/OPD2A0:13/TP1A.220905.001/R.162610b-1:user/release-keys'
ABI: 'arm64'
Build type: optimized
Zygote loaded classes=24325 post zygote classes=1280
Dumping registered class loaders
#0 dalvik.system.PathClassLoader: [], parent #1
#1 java.lang.BootClassLoader: [], no parent
#2 dalvik.system.PathClassLoader: [], parent #1
#3 dalvik.system.PathClassLoader: [/data/app/~~0gDcLHhiRd8lvd1pdFqxnw==/com.tans.dumpstack.demo-1lP5dA_73I2uDI9UULr3Mg==/base.apk!classes5.dex:/data/app/~~0gDcLHhiRd8lvd1pdFqxnw==/com.tans.dumpstack.demo-1lP5dA_73I2uDI9UULr3Mg==/base.apk!classes2.dex:/data/app/~~0gDcLHhiRd8lvd1pdFqxnw==/com.tans.dumpstack.demo-1lP5dA_73I2uDI9UULr3Mg==/base.apk:/data/app/~~0gDcLHhiRd8lvd1pdFqxnw==/com.tans.dumpstack.demo-1lP5dA_73I2uDI9UULr3Mg==/base.apk!classes4.dex:/data/app/~~0gDcLHhiRd8lvd1pdFqxnw==/com.tans.dumpstack.demo-1lP5dA_73I2uDI9UULr3Mg==/base.apk!classes3.dex], parent #1
Done dumping class loaders
Classes initialized: 0 in 0
Intern table: 41423 strong; 1205 weak
JNI: CheckJNI is on; globals=397 (plus 74 weak)
Libraries: /data/app/~~0gDcLHhiRd8lvd1pdFqxnw==/com.tans.dumpstack.demo-1lP5dA_73I2uDI9UULr3Mg==/base.apk!/lib/arm64-v8a/libbytehook.so /data/app/~~0gDcLHhiRd8lvd1pdFqxnw==/com.tans.dumpstack.demo-1lP5dA_73I2uDI9UULr3Mg==/base.apk!/lib/arm64-v8a/libdumpstack.so libSchedAssistJni.so libandroid.so libaudioeffect_jni.so libcompiler_rt.so libframework-connectivity-jni.so libframework-connectivity-tiramisu-jni.so libicu_jni.so libjavacore.so libjavacrypto.so libjnigraphics.so libmedia_jni.so libonetrace_jni.so libopenjdk.so liboplusextzawgyi.so liboplushwui_jni.so libphoenix_jni.so libpowerhalwrap_jni.so librs_jni.so librtp_jni.so libsoundpool.so libstats_jni.so libwebviewchromium_loader.so (24)
Heap: 6% free, 14MB/16MB; 434546 objects
Dumping cumulative Gc timings
Average major GC reclaim bytes ratio inf over 0 GC cycles
Average major GC copied live bytes ratio 0.838573 over 4 major GCs
Cumulative bytes moved 29931016
Cumulative objects moved 551127
Peak regions allocated 72 (18MB) / 1536 (384MB)
Total madvise time 32.223ms
Average minor GC reclaim bytes ratio inf over 0 GC cycles
Average minor GC copied live bytes ratio 0.0435016 over 5 minor GCs
Cumulative bytes moved 1976536
Cumulative objects moved 50067
Peak regions allocated 72 (18MB) / 1536 (384MB)
Total number of allocations 434546
Total bytes allocated 14MB
Total bytes freed 0B
Free memory 1066KB
Free memory until GC 1066KB
Free memory until OOME 369MB
Total memory 16MB
Max memory 384MB
Zygote space size 7680KB
Total mutator paused time: 0
Total time waiting for GC to complete: 2.691us
Total GC count: 0
Total GC time: 0
Total blocking GC count: 0
Total blocking GC time: 0
Total pre-OOME GC count: 0
Native bytes total: 26556212 registered: 861540
Total native bytes at last GC: 5816240
/system/framework/oat/arm64/android.hidl.base-V1.0-java.odex: verify
/system/framework/oat/arm64/android.hidl.manager-V1.0-java.odex: verify
/system/framework/oat/arm64/android.test.base.odex: verify
Current JIT code cache size (used / resident): 23KB / 32KB
Current JIT data cache size (used / resident): 43KB / 48KB
Zygote JIT code cache size (at point of fork): 19KB / 32KB
Zygote JIT data cache size (at point of fork): 8KB / 32KB
Current JIT mini-debug-info size: 41KB
Current JIT capacity: 64KB
Current number of JIT JNI stub entries: 0
Current number of JIT code cache entries: 57
Total number of JIT baseline compilations: 38
Total number of JIT optimized compilations: 4
Total number of JIT compilations for on stack replacement: 0
Total number of JIT code cache collections: 0
Memory used for stack maps: Avg: 100B Max: 432B Min: 32B
Memory used for compiled code: Avg: 569B Max: 2528B Min: 164B
Memory used for profiling info: Avg: 72B Max: 408B Min: 24B
Start Dumping Averages for 57 iterations for JIT timings
Compiling optimized:	Sum: 47.400ms Avg: 831.578us
Compiling baseline:	Sum: 10.020ms Avg: 175.789us
TrimMaps:	Sum: 2.143ms Avg: 37.596us
Done Dumping Averages
Memory used for compilation: Avg: 77KB Max: 1693KB Min: 16KB
ProfileSaver total_bytes_written=4203
ProfileSaver total_number_of_writes=1
ProfileSaver total_number_of_code_cache_queries=1
ProfileSaver total_number_of_skipped_writes=0
ProfileSaver total_number_of_failed_writes=0
ProfileSaver total_ms_of_sleep=5000
ProfileSaver total_ms_of_work=3
ProfileSaver total_number_of_hot_spikes=0
ProfileSaver total_number_of_wake_ups=0

*** ART internal metrics ***
  Metadata:
    timestamp_since_start_ms: 16684
  Metrics:
    ClassLoadingTotalTime: count = 56978
    ClassVerificationTotalTime: count = 138292
    ClassVerificationCount: count = 539
    WorldStopTimeDuringGCAvg: count = 0
    YoungGcCount: count = 0
    FullGcCount: count = 0
    TotalBytesAllocated: count = 3769568
    TotalGcCollectionTime: count = 0
    YoungGcThroughputAvg: count = 0
    FullGcThroughputAvg: count = 0
    YoungGcTracingThroughputAvg: count = 0
    FullGcTracingThroughputAvg: count = 0
    JitMethodCompileTotalTime: count = 17762
    JitMethodCompileCount: count = 42
    YoungGcCollectionTime: range = 0...60000, buckets: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    FullGcCollectionTime: range = 0...60000, buckets: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    YoungGcThroughput: range = 0...10000, buckets: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    FullGcThroughput: range = 0...10000, buckets: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    YoungGcTracingThroughput: range = 0...10000, buckets: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    FullGcTracingThroughput: range = 0...10000, buckets: 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
*** Done dumping ART internal metrics ***

suspend all histogram:	Sum: 165us 99% C.I. 1us-21us Avg: 7.173us Max: 21us
DALVIK THREADS (23):
"Signal Catcher" daemon prio=10 tid=2 Runnable
  | group="system" sCount=0 ucsCount=0 flags=0 obj=0x13600338 self=0xb400007bf3a26000
  | sysTid=5041 nice=-20 cgrp=default sched=0/0 handle=0x7bf4ffbcb0
  | state=R schedstat=( 28127001 5785385 10 ) utm=2 stm=0 core=5 HZ=100
  | stack=0x7bf4f04000-0x7bf4f06000 stackSize=991KB
  | held mutexes= "mutator lock"(shared held)
  native: #00 pc 0000000000570ec4  /apex/com.android.art/lib64/libart.so (art::DumpNativeStack(std::__1::basic_ostream<char, std::__1::char_traits<char> >&, int, BacktraceMap*, char const*, art::ArtMethod*, void*, bool)+148) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #01 pc 0000000000675a24  /apex/com.android.art/lib64/libart.so (art::Thread::DumpStack(std::__1::basic_ostream<char, std::__1::char_traits<char> >&, bool, BacktraceMap*, bool) const+340) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #02 pc 000000000069310c  /apex/com.android.art/lib64/libart.so (art::DumpCheckpoint::Run(art::Thread*)+908) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #03 pc 000000000068ccac  /apex/com.android.art/lib64/libart.so (art::ThreadList::RunCheckpoint(art::Closure*, art::Closure*)+508) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #04 pc 000000000068bf54  /apex/com.android.art/lib64/libart.so (art::ThreadList::Dump(std::__1::basic_ostream<char, std::__1::char_traits<char> >&, bool)+1796) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #05 pc 000000000068b70c  /apex/com.android.art/lib64/libart.so (art::ThreadList::DumpForSigQuit(std::__1::basic_ostream<char, std::__1::char_traits<char> >&)+1340) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #06 pc 000000000063d300  /apex/com.android.art/lib64/libart.so (art::Runtime::DumpForSigQuit(std::__1::basic_ostream<char, std::__1::char_traits<char> >&)+208) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #07 pc 0000000000651dc0  /apex/com.android.art/lib64/libart.so (art::SignalCatcher::HandleSigQuit()+1376) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #08 pc 0000000000650e54  /apex/com.android.art/lib64/libart.so (art::SignalCatcher::Run(void*)+340) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #09 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #10 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"main" prio=5 tid=1 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x73869160 self=0xb400007c11e10800
  | sysTid=15609 nice=-10 cgrp=default sched=1073741824/0 handle=0x7cbd635500
  | state=S schedstat=( 1086854706 330699698 4068 ) utm=63 stm=45 core=6 HZ=100
  | stack=0x7fd3027000-0x7fd3029000 stackSize=8188KB
  | held mutexes=
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 00000000002833dc  /apex/com.android.art/lib64/libart.so (art::ConditionVariable::WaitHoldingLocks(art::Thread*)+140) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #02 pc 000000000043bf3c  /apex/com.android.art/lib64/libart.so (art::(anonymous namespace)::CheckJNI::FindClass(_JNIEnv*, char const*) (.llvm.11132044689082360456)+460) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #03 pc 0000000000128ebc  /system/lib64/libandroid_runtime.so (android::NativeDisplayEventReceiver::dispatchVsync(long, android::PhysicalDisplayId, unsigned int, android::gui::VsyncEventData)+92) (BuildId: 4da95a3e8bdc1b6a6682b67c10bdc47e)
  native: #04 pc 00000000000c1820  /system/lib64/libgui.so (android::DisplayEventDispatcher::handleEvent(int, int, void*)+272) (BuildId: 1d69b7a57862392ad7b7712ed6197e18)
  native: #05 pc 000000000001836c  /system/lib64/libutils.so (android::Looper::pollInner(int)+1068) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #06 pc 0000000000017ee0  /system/lib64/libutils.so (android::Looper::pollOnce(int, int*, int*, void**)+112) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #07 pc 000000000016410c  /system/lib64/libandroid_runtime.so (android::android_os_MessageQueue_nativePollOnce(_JNIEnv*, _jobject*, long, int)+44) (BuildId: 4da95a3e8bdc1b6a6682b67c10bdc47e)
  at android.os.MessageQueue.nativePollOnce(Native method)
  at android.os.MessageQueue.next(MessageQueue.java:339)
  at android.os.Looper.loopOnce(Looper.java:186)
  at android.os.Looper.loop(Looper.java:351)
  at android.app.ActivityThread.main(ActivityThread.java:8377)
  at java.lang.reflect.Method.invoke(Native method)
  at com.android.internal.os.RuntimeInit$MethodAndArgsCaller.run(RuntimeInit.java:584)
  at com.android.internal.os.ZygoteInit.main(ZygoteInit.java:1013)

"Jit thread pool worker thread 0" daemon prio=5 tid=4 Native
  | group="system" sCount=1 ucsCount=0 flags=1 obj=0x135c0720 self=0xb400007bf3a47800
  | sysTid=5046 nice=9 cgrp=default sched=0/0 handle=0x7bf4d01cb0
  | state=S schedstat=( 12650002 4618461 48 ) utm=0 stm=0 core=1 HZ=100
  | stack=0x7bf4c02000-0x7bf4c04000 stackSize=1023KB
  | held mutexes=
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 00000000002833dc  /apex/com.android.art/lib64/libart.so (art::ConditionVariable::WaitHoldingLocks(art::Thread*)+140) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #02 pc 0000000000694b78  /apex/com.android.art/lib64/libart.so (art::ThreadPool::GetTask(art::Thread*)+120) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #03 pc 0000000000693f50  /apex/com.android.art/lib64/libart.so (art::ThreadPoolWorker::Run()+144) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #04 pc 00000000006939cc  /apex/com.android.art/lib64/libart.so (art::ThreadPoolWorker::Callback(void*)+172) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #05 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #06 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"perfetto_hprof_listener" prio=10 tid=8 Native (still starting up)
  | group="" sCount=1 ucsCount=0 flags=1 obj=0x0 self=0xb400007bf3a6f800
  | sysTid=5044 nice=-20 cgrp=default sched=0/0 handle=0x7bf4efdcb0
  | state=S schedstat=( 119385 21461461 4 ) utm=0 stm=0 core=6 HZ=100
  | stack=0x7bf4e06000-0x7bf4e08000 stackSize=991KB
  | held mutexes=
  native: #00 pc 00000000000d5774  /apex/com.android.runtime/lib64/bionic/libc.so (read+4) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000001dee4  /apex/com.android.art/lib64/libperfetto_hprof.so (void* std::__1::__thread_proxy<std::__1::tuple<std::__1::unique_ptr<std::__1::__thread_struct, std::__1::default_delete<std::__1::__thread_struct> >, ArtPlugin_Initialize::$_34> >(void*)+260) (BuildId: 13ee3b989b35c4e1d3ac372e558e2961)
  native: #02 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"binder:15609_1" prio=5 tid=9 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x13640020 self=0xb400007bf4867400
  | sysTid=5054 nice=-20 cgrp=default sched=0/0 handle=0x7bf42dfcb0
  | state=S schedstat=( 333385 370462 3 ) utm=0 stm=0 core=4 HZ=100
  | stack=0x7bf41e8000-0x7bf41ea000 stackSize=991KB
  | held mutexes=
  native: #00 pc 00000000000d5a54  /apex/com.android.runtime/lib64/bionic/libc.so (__ioctl+4) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 00000000000873bc  /apex/com.android.runtime/lib64/bionic/libc.so (ioctl+156) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000005f48c  /system/lib64/libbinder.so (android::IPCThreadState::talkWithDriver(bool)+284) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #03 pc 000000000005f788  /system/lib64/libbinder.so (android::IPCThreadState::getAndExecuteCommand()+24) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #04 pc 00000000000600a4  /system/lib64/libbinder.so (android::IPCThreadState::joinThreadPool(bool)+68) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #05 pc 0000000000090048  /system/lib64/libbinder.so (android::PoolThread::threadLoop()+24) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #06 pc 0000000000013550  /system/lib64/libutils.so (android::Thread::_threadLoop(void*)+416) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #07 pc 00000000000cc59c  /system/lib64/libandroid_runtime.so (android::AndroidRuntime::javaThreadShell(void*)+140) (BuildId: 4da95a3e8bdc1b6a6682b67c10bdc47e)
  native: #08 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #09 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"binder:15609_2" prio=5 tid=10 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x13680020 self=0xb400007bf3a85000
  | sysTid=5057 nice=-20 cgrp=default sched=0/0 handle=0x7bf41e1cb0
  | state=S schedstat=( 10795768 8014306 36 ) utm=0 stm=0 core=0 HZ=100
  | stack=0x7bf40ea000-0x7bf40ec000 stackSize=991KB
  | held mutexes=
  native: #00 pc 00000000000d5a54  /apex/com.android.runtime/lib64/bionic/libc.so (__ioctl+4) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 00000000000873bc  /apex/com.android.runtime/lib64/bionic/libc.so (ioctl+156) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000005f48c  /system/lib64/libbinder.so (android::IPCThreadState::talkWithDriver(bool)+284) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #03 pc 000000000005f788  /system/lib64/libbinder.so (android::IPCThreadState::getAndExecuteCommand()+24) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #04 pc 00000000000600a4  /system/lib64/libbinder.so (android::IPCThreadState::joinThreadPool(bool)+68) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #05 pc 0000000000090048  /system/lib64/libbinder.so (android::PoolThread::threadLoop()+24) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #06 pc 0000000000013550  /system/lib64/libutils.so (android::Thread::_threadLoop(void*)+416) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #07 pc 00000000000cc59c  /system/lib64/libandroid_runtime.so (android::AndroidRuntime::javaThreadShell(void*)+140) (BuildId: 4da95a3e8bdc1b6a6682b67c10bdc47e)
  native: #08 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #09 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"ADB-JDWP Connection Control Thread" daemon prio=0 tid=11 WaitingInMainDebuggerLoop
  | group="system" sCount=1 ucsCount=0 flags=1 obj=0x13587c48 self=0xb400007bf487c400
  | sysTid=5045 nice=-20 cgrp=default sched=0/0 handle=0x7bf4dffcb0
  | state=S schedstat=( 1337923 29081384 10 ) utm=0 stm=0 core=7 HZ=100
  | stack=0x7bf4d08000-0x7bf4d0a000 stackSize=991KB
  | held mutexes=
  native: #00 pc 00000000000d6b38  /apex/com.android.runtime/lib64/bionic/libc.so (__ppoll+8) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 0000000000089cec  /apex/com.android.runtime/lib64/bionic/libc.so (poll+92) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 0000000000009b78  /apex/com.android.art/lib64/libadbconnection.so (adbconnection::AdbConnectionState::RunPollLoop(art::Thread*)+728) (BuildId: a61da70931be5f5bc49350ef9b19cea4)
  native: #03 pc 00000000000081d0  /apex/com.android.art/lib64/libadbconnection.so (adbconnection::CallbackFunction(void*)+1344) (BuildId: a61da70931be5f5bc49350ef9b19cea4)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"HeapTaskDaemon" daemon prio=5 tid=12 WaitingForTaskProcessor
  | group="system" sCount=1 ucsCount=0 flags=1 obj=0x13587d10 self=0xb400007be44a8000
  | sysTid=5047 nice=4 cgrp=default sched=0/0 handle=0x7bf4bfbcb0
  | state=S schedstat=( 535231 20968384 6 ) utm=0 stm=0 core=1 HZ=100
  | stack=0x7bf4af8000-0x7bf4afa000 stackSize=1039KB
  | held mutexes=
  native: #00 pc 0000000000078df0  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+32) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 00000000002837a4  /apex/com.android.art/lib64/libart.so (art::ConditionVariable::TimedWait(art::Thread*, long, int)+164) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #02 pc 00000000003c2bfc  /apex/com.android.art/lib64/libart.so (art::gc::TaskProcessor::GetTask(art::Thread*)+444) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #03 pc 00000000003c34ec  /apex/com.android.art/lib64/libart.so (art::gc::TaskProcessor::RunAllTasks(art::Thread*)+92) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  at dalvik.system.VMRuntime.runHeapTasks(Native method)
  at java.lang.Daemons$HeapTaskDaemon.runInternal(Daemons.java:620)
  at java.lang.Daemons$Daemon.run(Daemons.java:145)
  at java.lang.Thread.run(Thread.java:1012)

"ReferenceQueueDaemon" daemon prio=5 tid=13 Waiting
  | group="system" sCount=1 ucsCount=0 flags=1 obj=0x13587da0 self=0xb400007be44a9c00
  | sysTid=5048 nice=4 cgrp=default sched=0/0 handle=0x7bf47fbcb0
  | state=S schedstat=( 110538 20121847 2 ) utm=0 stm=0 core=6 HZ=100
  | stack=0x7bf46f8000-0x7bf46fa000 stackSize=1039KB
  | held mutexes=
  at java.lang.Object.wait(Native method)
  - waiting on <0x0c105f5b> (a java.lang.Class<java.lang.ref.ReferenceQueue>)
  at java.lang.Object.wait(Object.java:442)
  at java.lang.Object.wait(Object.java:568)
  at java.lang.Daemons$ReferenceQueueDaemon.runInternal(Daemons.java:237)
  - locked <0x0c105f5b> (a java.lang.Class<java.lang.ref.ReferenceQueue>)
  at java.lang.Daemons$Daemon.run(Daemons.java:145)
  at java.lang.Thread.run(Thread.java:1012)

"FinalizerDaemon" daemon prio=5 tid=14 Waiting
  | group="system" sCount=1 ucsCount=0 flags=1 obj=0x13587e30 self=0xb400007be44ab800
  | sysTid=5049 nice=4 cgrp=default sched=0/0 handle=0x7bf46f1cb0
  | state=S schedstat=( 189462 19911384 2 ) utm=0 stm=0 core=6 HZ=100
  | stack=0x7bf45ee000-0x7bf45f0000 stackSize=1039KB
  | held mutexes=
  at java.lang.Object.wait(Native method)
  - waiting on <0x0b6883f8> (a java.lang.Object)
  at java.lang.Object.wait(Object.java:442)
  at java.lang.ref.ReferenceQueue.remove(ReferenceQueue.java:203)
  - locked <0x0b6883f8> (a java.lang.Object)
  at java.lang.ref.ReferenceQueue.remove(ReferenceQueue.java:224)
  at java.lang.Daemons$FinalizerDaemon.runInternal(Daemons.java:305)
  at java.lang.Daemons$Daemon.run(Daemons.java:145)
  at java.lang.Thread.run(Thread.java:1012)

"FinalizerWatchdogDaemon" daemon prio=5 tid=15 Waiting
  | group="system" sCount=1 ucsCount=0 flags=1 obj=0x13587ec0 self=0xb400007be44ad400
  | sysTid=5051 nice=4 cgrp=default sched=0/0 handle=0x7bf45e7cb0
  | state=S schedstat=( 84692 22283385 3 ) utm=0 stm=0 core=7 HZ=100
  | stack=0x7bf44e4000-0x7bf44e6000 stackSize=1039KB
  | held mutexes=
  at java.lang.Object.wait(Native method)
  - waiting on <0x0d85f0d1> (a java.lang.Daemons$FinalizerWatchdogDaemon)
  at java.lang.Object.wait(Object.java:442)
  at java.lang.Object.wait(Object.java:568)
  at java.lang.Daemons$FinalizerWatchdogDaemon.sleepUntilNeeded(Daemons.java:390)
  - locked <0x0d85f0d1> (a java.lang.Daemons$FinalizerWatchdogDaemon)
  at java.lang.Daemons$FinalizerWatchdogDaemon.runInternal(Daemons.java:370)
  at java.lang.Daemons$Daemon.run(Daemons.java:145)
  at java.lang.Thread.run(Thread.java:1012)

"binder:15609_3" prio=5 tid=16 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x13740020 self=0xb400007bf3a96800
  | sysTid=5062 nice=-20 cgrp=default sched=0/0 handle=0x7bf40e3cb0
  | state=S schedstat=( 637308 4772769 6 ) utm=0 stm=0 core=1 HZ=100
  | stack=0x7bf3fec000-0x7bf3fee000 stackSize=991KB
  | held mutexes=
  native: #00 pc 00000000000d5a54  /apex/com.android.runtime/lib64/bionic/libc.so (__ioctl+4) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 00000000000873bc  /apex/com.android.runtime/lib64/bionic/libc.so (ioctl+156) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000005f48c  /system/lib64/libbinder.so (android::IPCThreadState::talkWithDriver(bool)+284) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #03 pc 000000000005f788  /system/lib64/libbinder.so (android::IPCThreadState::getAndExecuteCommand()+24) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #04 pc 00000000000600a4  /system/lib64/libbinder.so (android::IPCThreadState::joinThreadPool(bool)+68) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #05 pc 0000000000090048  /system/lib64/libbinder.so (android::PoolThread::threadLoop()+24) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #06 pc 0000000000013550  /system/lib64/libutils.so (android::Thread::_threadLoop(void*)+416) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #07 pc 00000000000cc59c  /system/lib64/libandroid_runtime.so (android::AndroidRuntime::javaThreadShell(void*)+140) (BuildId: 4da95a3e8bdc1b6a6682b67c10bdc47e)
  native: #08 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #09 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"Profile Saver" daemon prio=5 tid=17 Native
  | group="system" sCount=1 ucsCount=0 flags=1 obj=0x137c0020 self=0xb400007bf48b2400
  | sysTid=5091 nice=9 cgrp=default sched=0/0 handle=0x7bf3ee6cb0
  | state=S schedstat=( 8479770 659615 12 ) utm=0 stm=0 core=7 HZ=100
  | stack=0x7bf3def000-0x7bf3df1000 stackSize=991KB
  | held mutexes=
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 00000000002833dc  /apex/com.android.art/lib64/libart.so (art::ConditionVariable::WaitHoldingLocks(art::Thread*)+140) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #02 pc 0000000000432b1c  /apex/com.android.art/lib64/libart.so (art::ProfileSaver::Run()+540) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #03 pc 0000000000437f80  /apex/com.android.art/lib64/libart.so (art::ProfileSaver::RunProfileSaverThread(void*)+176) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"oplus.app.bg" prio=5 tid=18 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x1380fae0 self=0xb400007bf480ec00
  | sysTid=5099 nice=10 cgrp=default sched=0/0 handle=0x7b89cbbcb0
  | state=S schedstat=( 556077 0 1 ) utm=0 stm=0 core=4 HZ=100
  | stack=0x7b89bb8000-0x7b89bba000 stackSize=1039KB
  | held mutexes=
  native: #00 pc 00000000000d6a38  /apex/com.android.runtime/lib64/bionic/libc.so (__epoll_pwait+8) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 0000000000017ffc  /system/lib64/libutils.so (android::Looper::pollInner(int)+188) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #02 pc 0000000000017ee0  /system/lib64/libutils.so (android::Looper::pollOnce(int, int*, int*, void**)+112) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #03 pc 000000000016410c  /system/lib64/libandroid_runtime.so (android::android_os_MessageQueue_nativePollOnce(_JNIEnv*, _jobject*, long, int)+44) (BuildId: 4da95a3e8bdc1b6a6682b67c10bdc47e)
  at android.os.MessageQueue.nativePollOnce(Native method)
  at android.os.MessageQueue.next(MessageQueue.java:339)
  at android.os.Looper.loopOnce(Looper.java:186)
  at android.os.Looper.loop(Looper.java:351)
  at android.os.HandlerThread.run(HandlerThread.java:67)

"FramePolicy" prio=5 tid=19 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x1380ff98 self=0xb400007bf4810800
  | sysTid=5100 nice=0 cgrp=default sched=0/0 handle=0x7b88bb1cb0
  | state=S schedstat=( 4354768 37308 4 ) utm=0 stm=0 core=7 HZ=100
  | stack=0x7b88aae000-0x7b88ab0000 stackSize=1039KB
  | held mutexes=
  native: #00 pc 00000000000d6a38  /apex/com.android.runtime/lib64/bionic/libc.so (__epoll_pwait+8) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 0000000000017ffc  /system/lib64/libutils.so (android::Looper::pollInner(int)+188) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #02 pc 0000000000017ee0  /system/lib64/libutils.so (android::Looper::pollOnce(int, int*, int*, void**)+112) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #03 pc 000000000016410c  /system/lib64/libandroid_runtime.so (android::android_os_MessageQueue_nativePollOnce(_JNIEnv*, _jobject*, long, int)+44) (BuildId: 4da95a3e8bdc1b6a6682b67c10bdc47e)
  at android.os.MessageQueue.nativePollOnce(Native method)
  at android.os.MessageQueue.next(MessageQueue.java:339)
  at android.os.Looper.loopOnce(Looper.java:186)
  at android.os.Looper.loop(Looper.java:351)
  at android.os.HandlerThread.run(HandlerThread.java:67)

"FramePolicy" prio=5 tid=20 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x138100a8 self=0xb400007bf4812400
  | sysTid=5101 nice=-4 cgrp=default sched=0/0 handle=0x7b87aa7cb0
  | state=S schedstat=( 449538 0 1 ) utm=0 stm=0 core=5 HZ=100
  | stack=0x7b879a4000-0x7b879a6000 stackSize=1039KB
  | held mutexes=
  native: #00 pc 00000000000d6a38  /apex/com.android.runtime/lib64/bionic/libc.so (__epoll_pwait+8) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 0000000000017ffc  /system/lib64/libutils.so (android::Looper::pollInner(int)+188) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #02 pc 0000000000017ee0  /system/lib64/libutils.so (android::Looper::pollOnce(int, int*, int*, void**)+112) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #03 pc 000000000016410c  /system/lib64/libandroid_runtime.so (android::android_os_MessageQueue_nativePollOnce(_JNIEnv*, _jobject*, long, int)+44) (BuildId: 4da95a3e8bdc1b6a6682b67c10bdc47e)
  at android.os.MessageQueue.nativePollOnce(Native method)
  at android.os.MessageQueue.next(MessageQueue.java:339)
  at android.os.Looper.loopOnce(Looper.java:186)
  at android.os.Looper.loop(Looper.java:351)
  at android.os.HandlerThread.run(HandlerThread.java:67)

"launch" prio=5 tid=21 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x138103a8 self=0xb400007bf4823000
  | sysTid=5102 nice=0 cgrp=default sched=0/0 handle=0x7b86848cb0
  | state=S schedstat=( 396616 579538 2 ) utm=0 stm=0 core=5 HZ=100
  | stack=0x7b86745000-0x7b86747000 stackSize=1039KB
  | held mutexes=
  native: #00 pc 00000000000d6a38  /apex/com.android.runtime/lib64/bionic/libc.so (__epoll_pwait+8) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 0000000000017ffc  /system/lib64/libutils.so (android::Looper::pollInner(int)+188) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #02 pc 0000000000017ee0  /system/lib64/libutils.so (android::Looper::pollOnce(int, int*, int*, void**)+112) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #03 pc 000000000016410c  /system/lib64/libandroid_runtime.so (android::android_os_MessageQueue_nativePollOnce(_JNIEnv*, _jobject*, long, int)+44) (BuildId: 4da95a3e8bdc1b6a6682b67c10bdc47e)
  at android.os.MessageQueue.nativePollOnce(Native method)
  at android.os.MessageQueue.next(MessageQueue.java:339)
  at android.os.Looper.loopOnce(Looper.java:186)
  at android.os.Looper.loop(Looper.java:351)
  at android.os.HandlerThread.run(HandlerThread.java:67)

"UIMonitorThread" prio=5 tid=22 Sleeping
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x13810560 self=0xb400007bf4824c00
  | sysTid=5103 nice=0 cgrp=default sched=0/0 handle=0x7b8573ecb0
  | state=S schedstat=( 2590537 323538 11 ) utm=0 stm=0 core=5 HZ=100
  | stack=0x7b8563b000-0x7b8563d000 stackSize=1039KB
  | held mutexes=
  at java.lang.Thread.sleep(Native method)
  - sleeping on <0x040d6736> (a java.lang.Object)
  at java.lang.Thread.sleep(Thread.java:450)
  - locked <0x040d6736> (a java.lang.Object)
  at java.lang.Thread.sleep(Thread.java:355)
  at android.os.SystemClock.sleep(SystemClock.java:133)
  at android.os.OplusTheiaUIMonitor$MonitorThread.run(OplusTheiaUIMonitor.java:218)

"RenderThread" daemon prio=10 tid=23 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x13940020 self=0xb400007bf49db800
  | sysTid=5104 nice=-10 cgrp=default sched=1073741824/0 handle=0x7b84634cb0
  | state=S schedstat=( 112480010 7714463 175 ) utm=8 stm=2 core=7 HZ=100
  | stack=0x7b8453d000-0x7b8453f000 stackSize=991KB
  | held mutexes=
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 00000000002833dc  /apex/com.android.art/lib64/libart.so (art::ConditionVariable::WaitHoldingLocks(art::Thread*)+140) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #02 pc 0000000000457428  /apex/com.android.art/lib64/libart.so (art::(anonymous namespace)::CheckJNI::CallMethodV(char const*, _JNIEnv*, _jobject*, _jclass*, _jmethodID*, std::__va_list, art::Primitive::Type, art::InvokeType)+472) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #03 pc 0000000000447cac  /apex/com.android.art/lib64/libart.so (art::(anonymous namespace)::CheckJNI::CallStaticBooleanMethodV(_JNIEnv*, _jclass*, _jmethodID*, std::__va_list) (.llvm.11132044689082360456)+76) (BuildId: f9461dad2df8cf4e9114de5c4ff5caf5)
  native: #04 pc 0000000000207678  /system/lib64/libhwui.so (_JNIEnv::CallStaticBooleanMethod(_jclass*, _jmethodID*, ...)+120) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #05 pc 000000000020759c  /system/lib64/libhwui.so (android::HardwareRendererObserver::notify(long const*)+284) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #06 pc 00000000002a4f98  /system/lib64/libhwui.so (android::uirenderer::FrameMetricsReporter::reportFrameMetrics(long const*, bool, unsigned long, int)+744) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #07 pc 0000000000287510  /system/lib64/libhwui.so (android::uirenderer::renderthread::CanvasContext::draw()+2176) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #08 pc 0000000000287ea4  /system/lib64/libhwui.so (android::uirenderer::renderthread::CanvasContext::prepareAndDraw(android::uirenderer::RenderNode*)+196) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #09 pc 000000000029b30c  /system/lib64/libhwui.so (std::__1::__function::__func<android::uirenderer::renderthread::RenderThread::frameCallback(long, long, long, long)::$_0, std::__1::allocator<android::uirenderer::renderthread::RenderThread::frameCallback(long, long, long, long)::$_0>, void ()>::operator()() (.8ef779c993792426461f62d071c69435)+172) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #10 pc 00000000002785bc  /system/lib64/libhwui.so (android::uirenderer::WorkQueue::process()+588) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #11 pc 000000000029a6f8  /system/lib64/libhwui.so (android::uirenderer::renderthread::RenderThread::threadLoop()+424) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #12 pc 0000000000013550  /system/lib64/libutils.so (android::Thread::_threadLoop(void*)+416) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #13 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #14 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"AsyncTask #1" prio=5 tid=24 Waiting
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x13b03428 self=0xb400007bf49f3000
  | sysTid=5119 nice=0 cgrp=default sched=0/0 handle=0x7b83536cb0
  | state=S schedstat=( 2542156 670075 5 ) utm=0 stm=0 core=1 HZ=100
  | stack=0x7b83433000-0x7b83435000 stackSize=1039KB
  | held mutexes=
  at jdk.internal.misc.Unsafe.park(Native method)
  - waiting on an unknown object
  at java.util.concurrent.locks.LockSupport.park(LockSupport.java:194)
  at java.util.concurrent.SynchronousQueue$TransferStack.awaitFulfill(SynchronousQueue.java:461)
  at java.util.concurrent.SynchronousQueue$TransferStack.transfer(SynchronousQueue.java:361)
  at java.util.concurrent.SynchronousQueue.take(SynchronousQueue.java:922)
  at java.util.concurrent.ThreadPoolExecutor.getTask(ThreadPoolExecutor.java:1063)
  at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1123)
  at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:637)
  at java.lang.Thread.run(Thread.java:1012)

"hwuiTask1" daemon prio=6 tid=25 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x13b80020 self=0xb400007bf3af0000
  | sysTid=5135 nice=-2 cgrp=default sched=0/0 handle=0x7b74fffcb0
  | state=S schedstat=( 456769 99923 4 ) utm=0 stm=0 core=7 HZ=100
  | stack=0x7b74f08000-0x7b74f0a000 stackSize=991KB
  | held mutexes=
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 00000000000eaa0c  /apex/com.android.runtime/lib64/bionic/libc.so (pthread_cond_wait+76) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 00000000000570d4  /system/lib64/libc++.so (std::__1::condition_variable::wait(std::__1::unique_lock<std::__1::mutex>&)+20) (BuildId: 81be9f9fc3ebe436bea9954bac713c94)
  native: #04 pc 000000000029e46c  /system/lib64/libhwui.so (android::uirenderer::CommonPool::workerLoop()+108) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #05 pc 000000000029e6e8  /system/lib64/libhwui.so (void* std::__1::__thread_proxy<std::__1::tuple<std::__1::unique_ptr<std::__1::__thread_struct, std::__1::default_delete<std::__1::__thread_struct> >, android::uirenderer::CommonPool::CommonPool()::$_0> >(void*)+264) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #06 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #07 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"hwuiTask0" daemon prio=6 tid=26 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x13bc0020 self=0xb400007b762af000
  | sysTid=5134 nice=-2 cgrp=default sched=0/0 handle=0x7b750fdcb0
  | state=S schedstat=( 693308 552077 7 ) utm=0 stm=0 core=0 HZ=100
  | stack=0x7b75006000-0x7b75008000 stackSize=991KB
  | held mutexes=
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 00000000000eaa0c  /apex/com.android.runtime/lib64/bionic/libc.so (pthread_cond_wait+76) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 00000000000570d4  /system/lib64/libc++.so (std::__1::condition_variable::wait(std::__1::unique_lock<std::__1::mutex>&)+20) (BuildId: 81be9f9fc3ebe436bea9954bac713c94)
  native: #04 pc 000000000029e46c  /system/lib64/libhwui.so (android::uirenderer::CommonPool::workerLoop()+108) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #05 pc 000000000029e6e8  /system/lib64/libhwui.so (void* std::__1::__thread_proxy<std::__1::tuple<std::__1::unique_ptr<std::__1::__thread_struct, std::__1::default_delete<std::__1::__thread_struct> >, android::uirenderer::CommonPool::CommonPool()::$_0> >(void*)+264) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #06 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #07 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"binder:15609_4" prio=5 tid=27 Native
  | group="main" sCount=1 ucsCount=0 flags=1 obj=0x13c00020 self=0xb400007b718e3800
  | sysTid=5141 nice=-20 cgrp=default sched=0/0 handle=0x7b707fbcb0
  | state=S schedstat=( 5449461 1428537 32 ) utm=0 stm=0 core=4 HZ=100
  | stack=0x7b70704000-0x7b70706000 stackSize=991KB
  | held mutexes=
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 00000000000eccec  /apex/com.android.runtime/lib64/bionic/libc.so (NonPI::MutexLockWithTimeout(pthread_mutex_internal_t*, bool, timespec const*)+348) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 00000000000ec9f8  /apex/com.android.runtime/lib64/bionic/libc.so (pthread_mutex_lock+216) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #04 pc 0000000000098d28  /system/lib64/libc++.so (std::__1::mutex::lock()+8) (BuildId: 81be9f9fc3ebe436bea9954bac713c94)
  native: #05 pc 00000000002865ec  /system/lib64/libhwui.so (android::uirenderer::renderthread::CanvasContext::onSurfaceStatsAvailable(void*, int, ASurfaceControlStats*)+236) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #06 pc 000000000001e2c4  /system/lib64/libandroid.so (std::__1::__function::__func<ASurfaceControl_registerSurfaceStatsListener::$_0, std::__1::allocator<ASurfaceControl_registerSurfaceStatsListener::$_0>, void (void*, long, android::sp<android::Fence> const&, android::SurfaceStats const&)>::operator()(void*&&, long&&, android::sp<android::Fence> const&, android::SurfaceStats const&)+228) (BuildId: ecaa24c0435a167c96238feeaebb5dfa)
  native: #07 pc 0000000000104bd0  /system/lib64/libgui.so (android::TransactionCompletedListener::onTransactionCompleted(android::ListenerStats)+3552) (BuildId: 1d69b7a57862392ad7b7712ed6197e18)
  native: #08 pc 00000000000e383c  /system/lib64/libgui.so (int android::SafeBnInterface<android::ITransactionCompletedListener>::callLocalAsync<void (android::ITransactionCompletedListener::*)(android::ListenerStats)>(android::Parcel const&, android::Parcel*, void (android::ITransactionCompletedListener::*)(android::ListenerStats))+300) (BuildId: 1d69b7a57862392ad7b7712ed6197e18)
  native: #09 pc 0000000000054a4c  /system/lib64/libbinder.so (android::BBinder::transact(unsigned int, android::Parcel const&, android::Parcel*, unsigned int)+236) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #10 pc 000000000005fcd4  /system/lib64/libbinder.so (android::IPCThreadState::executeCommand(int)+1012) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #11 pc 000000000005f810  /system/lib64/libbinder.so (android::IPCThreadState::getAndExecuteCommand()+160) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #12 pc 00000000000600a4  /system/lib64/libbinder.so (android::IPCThreadState::joinThreadPool(bool)+68) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #13 pc 0000000000090048  /system/lib64/libbinder.so (android::PoolThread::threadLoop()+24) (BuildId: 821d5191ea842f908c210c9c338b12f6)
  native: #14 pc 0000000000013550  /system/lib64/libutils.so (android::Thread::_threadLoop(void*)+416) (BuildId: 6038dbf95f76d91eaf842148f10f89ea)
  native: #15 pc 00000000000cc59c  /system/lib64/libandroid_runtime.so (android::AndroidRuntime::javaThreadShell(void*)+140) (BuildId: 4da95a3e8bdc1b6a6682b67c10bdc47e)
  native: #16 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #17 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  (no managed stack frames)

"mali-mem-purge" prio=10 (not attached)
  | sysTid=5121 nice=-10 cgrp=default
  | state=S schedstat=( 2100539 204999 17 ) utm=0 stm=0 core=1 HZ=100
  native: #00 pc 00000000000d61f8  /apex/com.android.runtime/lib64/bionic/libc.so (nanosleep+8) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000009125c  /apex/com.android.runtime/lib64/bionic/libc.so (usleep+76) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 0000000000873b18  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #03 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #04 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"mali-utility-wo" prio=10 (not attached)
  | sysTid=5123 nice=-10 cgrp=default
  | state=S schedstat=( 45539 0 1 ) utm=0 stm=0 core=0 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000008ba8c  /apex/com.android.runtime/lib64/bionic/libc.so (sem_wait+108) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 0000000000850e64  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"mali-utility-wo" prio=10 (not attached)
  | sysTid=5124 nice=-10 cgrp=default
  | state=S schedstat=( 124769 64154 4 ) utm=0 stm=0 core=7 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000008ba8c  /apex/com.android.runtime/lib64/bionic/libc.so (sem_wait+108) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 0000000000850e64  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"mali-utility-wo" prio=10 (not attached)
  | sysTid=5125 nice=-10 cgrp=default
  | state=S schedstat=( 125001 0 4 ) utm=0 stm=0 core=7 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000008ba8c  /apex/com.android.runtime/lib64/bionic/libc.so (sem_wait+108) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 0000000000850e64  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"mali-utility-wo" prio=10 (not attached)
  | sysTid=5126 nice=-10 cgrp=default
  | state=S schedstat=( 78768 0 2 ) utm=0 stm=0 core=0 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000008ba8c  /apex/com.android.runtime/lib64/bionic/libc.so (sem_wait+108) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 0000000000850e64  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"mali-utility-wo" prio=10 (not attached)
  | sysTid=5127 nice=-10 cgrp=default
  | state=S schedstat=( 48770 0 2 ) utm=0 stm=0 core=0 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000008ba8c  /apex/com.android.runtime/lib64/bionic/libc.so (sem_wait+108) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 0000000000850e64  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"mali-utility-wo" prio=10 (not attached)
  | sysTid=5128 nice=-10 cgrp=default
  | state=S schedstat=( 125076 48615 5 ) utm=0 stm=0 core=0 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000008ba8c  /apex/com.android.runtime/lib64/bionic/libc.so (sem_wait+108) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 0000000000850e64  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"mali-utility-wo" prio=10 (not attached)
  | sysTid=5129 nice=-10 cgrp=default
  | state=S schedstat=( 54000 0 2 ) utm=0 stm=0 core=0 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000008ba8c  /apex/com.android.runtime/lib64/bionic/libc.so (sem_wait+108) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 0000000000850e64  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"mali-utility-wo" prio=10 (not attached)
  | sysTid=5130 nice=-10 cgrp=default
  | state=S schedstat=( 99307 0 6 ) utm=0 stm=0 core=0 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000008ba8c  /apex/com.android.runtime/lib64/bionic/libc.so (sem_wait+108) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 0000000000850e64  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"mali-cmar-backe" prio=10 (not attached)
  | sysTid=5131 nice=-20 cgrp=default
  | state=S schedstat=( 33007377 13927075 147 ) utm=0 stm=2 core=0 HZ=100
  native: #00 pc 00000000000d6b38  /apex/com.android.runtime/lib64/bionic/libc.so (__ppoll+8) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 0000000000089cec  /apex/com.android.runtime/lib64/bionic/libc.so (poll+92) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 0000000000851124  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #03 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #04 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"ged-swd" prio=10 (not attached)
  | sysTid=5132 nice=-10 cgrp=default
  | state=S schedstat=( 5828460 3782231 78 ) utm=0 stm=0 core=1 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 00000000000eaa0c  /apex/com.android.runtime/lib64/bionic/libc.so (pthread_cond_wait+76) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 0000000000008adc  /vendor/lib64/libged.so (ged_cond_lock+44) (BuildId: fc0ae544d1fb627d799f44ff423063c5)
  native: #04 pc 0000000000008f68  /vendor/lib64/libged.so (ged_worker_thread+112) (BuildId: fc0ae544d1fb627d799f44ff423063c5)
  native: #05 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #06 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"RenderThread" prio=10 (not attached)
  | sysTid=5137 nice=-10 cgrp=default
  | state=S schedstat=( 67392842 8564309 77 ) utm=5 stm=1 core=0 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 00000000000eaa0c  /apex/com.android.runtime/lib64/bionic/libc.so (pthread_cond_wait+76) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 00000000008ab254  /vendor/lib64/egl/libGLES_mali.so (???) (BuildId: 937bebb06da90dc2)
  native: #04 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #05 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"binder:15609_4" prio=10 (not attached)
  | sysTid=5142 nice=-20 cgrp=default
  | state=S schedstat=( 1659386 797307 28 ) utm=0 stm=0 core=3 HZ=100
  native: #00 pc 0000000000078dec  /apex/com.android.runtime/lib64/bionic/libc.so (syscall+28) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000007d7e0  /apex/com.android.runtime/lib64/bionic/libc.so (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 00000000000eaa0c  /apex/com.android.runtime/lib64/bionic/libc.so (pthread_cond_wait+76) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #03 pc 00000000000570d4  /system/lib64/libc++.so (std::__1::condition_variable::wait(std::__1::unique_lock<std::__1::mutex>&)+20) (BuildId: 81be9f9fc3ebe436bea9954bac713c94)
  native: #04 pc 00000000000afd74  /system/lib64/libgui.so (android::AsyncWorker::run()+132) (BuildId: 1d69b7a57862392ad7b7712ed6197e18)
  native: #05 pc 00000000000b013c  /system/lib64/libgui.so (void* std::__1::__thread_proxy<std::__1::tuple<std::__1::unique_ptr<std::__1::__thread_struct, std::__1::default_delete<std::__1::__thread_struct> >, void (android::AsyncWorker::*)(), android::AsyncWorker*> >(void*)+60) (BuildId: 1d69b7a57862392ad7b7712ed6197e18)
  native: #06 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #07 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"RenderThread" prio=5 (not attached)
  | sysTid=5295 nice=0 cgrp=default
  | state=S schedstat=( 84923 0 1 ) utm=0 stm=0 core=0 HZ=100
  native: #00 pc 00000000000d61f8  /apex/com.android.runtime/lib64/bionic/libc.so (nanosleep+8) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000008cf6c  /apex/com.android.runtime/lib64/bionic/libc.so (sleep+44) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 0000000000014bd0  /system/lib64/libEGL.so (void* std::__1::__thread_proxy<std::__1::tuple<std::__1::unique_ptr<std::__1::__thread_struct, std::__1::default_delete<std::__1::__thread_struct> >, android::egl_cache_t::setBlob(void const*, long, void const*, long)::$_0> >(void*)+48) (BuildId: f9001722eb96e134648f27c0c283a2ec)
  native: #03 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #04 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

"RenderThread" prio=5 (not attached)
  | sysTid=5296 nice=0 cgrp=default
  | state=S schedstat=( 61615 54077 1 ) utm=0 stm=0 core=6 HZ=100
  native: #00 pc 00000000000d61f8  /apex/com.android.runtime/lib64/bionic/libc.so (nanosleep+8) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #01 pc 000000000008cf6c  /apex/com.android.runtime/lib64/bionic/libc.so (sleep+44) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #02 pc 000000000027c190  /system/lib64/libhwui.so (void* std::__1::__thread_proxy<std::__1::tuple<std::__1::unique_ptr<std::__1::__thread_struct, std::__1::default_delete<std::__1::__thread_struct> >, android::uirenderer::skiapipeline::ShaderCache::store(SkData const&, SkData const&, SkString const&)::$_0> >(void*)+64) (BuildId: 50f8903e6e159dd4a01f7fa47fdb31d7)
  native: #03 pc 00000000000eb720  /apex/com.android.runtime/lib64/bionic/libc.so (__pthread_start(void*)+208) (BuildId: cd953571180b7f5f8ae5570dad29595f)
  native: #04 pc 000000000007e2d0  /apex/com.android.runtime/lib64/bionic/libc.so (__start_thread+64) (BuildId: cd953571180b7f5f8ae5570dad29595f)

----- end 15609 -----

```