# ps中的进程状态码
PROCESS STATE CODES
Here are the different values that the s, stat and state output specifiers (header "STAT" or "S") will display to describe the state of a process:
               D    uninterruptible sleep (usually IO)
               R    running or runnable (on run queue)
               S    interruptible sleep (waiting for an event to complete)
               T    stopped by job control signal
               t    stopped by debugger during the tracing
               W    paging (not valid since the 2.6.xx kernel)
               X    dead (should never be seen)
               Z    defunct ("zombie") process, terminated but not reaped by its parent

For BSD formats and when the stat keyword is used, additional characters may be displayed:
               <    high-priority (not nice to other users)
               N    low-priority (nice to other users)
               L    has pages locked into memory (for real-time and custom IO)
               s    is a session leader
               l    is multi-threaded (using CLONE_THREAD, like NPTL pthreads do)
               +    is in the foreground process group

# 使用kill命令发送信号
## 查看可用的信号值
kill -l

ps中的加号
+    is in the foreground process group
表示进程在前台进程组中。
例如：
./a.out &
./b.out
huangch+ 190640 99.2  0.0   4216   648 pts/23   R    11:57   0:18 ./a.out
huangch+ 190646  100  0.0   4216   780 pts/23   R+   11:57   0:05 ./b.out
a.out在后台运行，而b.out在前台运行。


# 内核中的运行状态
## TASK_RUNNING - R
正在运行 或者
已经达到可以运行的状态，被放入到了运行队列中，但是还没有开始运行。

## TASK_INTERRUPTIBLE - S
可中断的睡眠状态
等待某种事情发生（比如等待socket连接，等待信号量等），而被挂起。
进程的task_struct被放入对应事件的等待队列中，当这些事件发生时，对应的等待队列中的一个或多个进程将会被唤醒。

## TASK_UNINTERRUPTIBLE - D
不可中断的睡眠状态
与 TASK_INTERRUPTIBLE 类似，进程处于睡眠状态，但是此时进程是不可被中断的。
不可被中断是指，该进程不响应异步信号。（硬件中断仍然有效）
该状态的意义：内核的某些流程是不可以被打断的，如果响应异步信号，程序的执行流程就会被插入一段处理异步信号的流程(插入的流程可能只在内核态，也可能在用户态)，于是原有的流程就被打断了。
当进程与某些硬件交互时(例如进程通过read系统调用对设备文件进行读操作)，可能需要 TASK_UNINTERRUPTIBLE 进行保护，防止交互被打断，导致设备陷入不可控的状态。

## TASK_STOPPED - T
## TASK_TRACED - t
被追踪状态，例如使用gdb对进程进行调试，就进入了该状态。

## TASK_DEAD - Z
僵尸进程
进程占用的所有资源都被回收