# Linux下三个特殊的进程(idle, init, kthreadd)
## idle进程
idle进程由内核创建，运行在内核态。

idle进程的pid为0，是系统创建的第一个进程，也是唯一一个没有通过fork或者kernel_thread产生的进程。完成加载系统后，演变为进程调度、交换。

## init进程
由0进程创建，完成系统的初始化，是系统中所有其他用户进程的祖先进程。

Linux中的所有进程都由init进程创建并运行的。首先Linux内核启动，然后在用户空间中启动init进程，再启动其它系统进程。

系统启动完成后，init进程变为守护进程监控系统其它进程。

## kthreadd进程
kthreadd进程由idle通过kernel_thread创建，始终运行在内核态，负责所有内核线程的调度和管理。

它的任务是管理和调度其它内核线程kernel_thread，会循环执行一个kthread的函数，该函数的作用就是运行kthread_create_list全局链表中维护的kthread。

当我们调用kernel_thread创建的内核线程会加入到该链表中，因此所有的内核线程都直接或间接的以kthreadd为父进程。