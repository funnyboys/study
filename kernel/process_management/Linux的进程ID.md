# 进程ID号
Linux为每个进程分配一个ID作为其命名空间中的标识，ID有多种类型:

1. 进程处于某个线程组时，拥有线程组ID(TGID)(如果没有线程组，则PID与TGID相同)。
2. 独立进程可以合并成进程组，进程组成员的task_struct的pgrp属性值相同(为进程组组长PID)(用管道连接的进程包含在同一个进程组中)。
3. 几个进程组可以合并成一个会话，会话中所有进程都有会话ID(SID)，保存在task_struct中的session中。

# 进程ID类型
进程ID类型在include/linux/pid.h中定义如下：
```
enum pid_type
{
	PIDTYPE_PID,
	PIDTYPE_PGID,
	PIDTYPE_SID,
	PIDTYPE_MAX
};
```

# PS命令查看线程
```
huangchangwei@ubuntu:~/code_test/thread$ ps -T
   PID   SPID TTY          TIME CMD
 99855  99855 pts/15   00:00:00 bash
163829 163829 pts/15   00:00:00 a.out
163829 163830 pts/15   00:00:00 a.out
164038 164038 pts/15   00:00:00 ps
```

## PID的解释
pid         PID       a number representing the process ID (alias tgid).
线程组ID，即task_struct->tgid

## SPID的解释
spid        SPID      see lwp.  (alias lwp, tid).
lwp         LWP       light weight process (thread) ID of the dispatchable entity (alias spid, tid).  See tid for additional information.
轻量级线程标识，即task_struct->pid


# PID
pid是Linux在命名空间中唯一标识进程而分配的ID，叫做进程ID号，即PID。在使用fork或者clone系统调用时产生的进程，内核会分配一个新的唯一的PID值。

pid用于内核唯一区分每个进程。

# TGID(thread group id)
在一个进程中，如果以CLONE_THREAD标志调用clone建立的进程就是该进程的一个线程(轻量级进程)，它们位于一个线程组。

线程组中的所有线程的ID叫做线程组ID。
位于相同线程组中的线程有相同的TGID，但是它们的PID各不相同。

# PGID(Process group id)
独立的进程可以组成进程组(setpgrp系统调用)，进程组可以简化向所有组内进程发送信号的操作。

例如用管道连接的进程处在同一进程组内，进程组ID叫做PGID，进程组内的所有进程有相同的PGID，等于组长的PID。

# SID(Session id)
几个进程组可以合并成一个会话组(setsid系统调用)。
会话组中所有进程都有相同的SID，保存在task_struct->session中。