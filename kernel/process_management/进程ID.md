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
   PID   SPID TTY          TIME CMD
 99855  99855 pts/15   00:00:00 bash
163829 163829 pts/15   00:00:00 a.out
163829 163830 pts/15   00:00:00 a.out
164038 164038 pts/15   00:00:00 ps
```

## 什么是PID
man ps之后，对于PID的解释如下：

pid         PID       a number representing the process ID (alias tgid).

PID的含义和tgid相同，即线程组ID，task_struct->tgid

## 什么是SPID
spid        SPID      see lwp.  (alias lwp, tid).

lwp         LWP       light weight process (thread) ID of the dispatchable entity (alias spid, tid).  See tid for additional information.

SPID是轻量级线程的标识，即task_struct->pid


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

# PID命名空间

命名空间是比KVM之类更加轻量级的虚拟化技术，是对全局资源的一种抽象。将资源放在不同的容器之中，各容器之间彼此隔离。

由于各个命名空间彼此隔离，相同的PID可以出现在不同的命名空间之内，因此每个命名空间中都有自己的1号进程。

PID命名空间具有层次关系，父命名空间知道子命名空间的存在，因此子命名空间需要映射到父命名空间中去。

## 局部ID和全局ID

由于PID命名空间具有层次关系。在建立一个新的命名空间时，该命名空间中的所有PID对父命名空间都是可见的，但子命名空间无法看到父命名空间的PID。这意味着某些进程有多个PID，凡是能看到该进程的命名空间，都需要为其分配一个对应的PID。

因此，必须要区分局部ID和全局ID。

* **全局ID** 在内核本身和初始化命名空间中惟一的一个ID，系统启动时启动的init进程属于该初始命名空间。系统中每个进程都对应了该命名空间中的一个PID，叫全局ID，保证在整个系统中唯一。

* **局部ID** 对于某个特定的命名空间之内，在其内部分配的ID叫做局部ID，该ID仅在内部有效。

全局PID和TGID保存在结构体`task_struct`中，分别是`pid`和 `tgid`。

```
pid_t pid;      //process id(unique)
pid_t tgid;     //thread group id
```

## PID命名空间的数据结构pid_namespace

pid_namespace保存在task_struct中，`task_struct->nsproxy->pid_namespace`，在include/linux/pid_namespace.h中进行定义。

```
struct pid_namespace {
	struct kref kref;                       //指向pid_namespace的引用个数
	struct pidmap pidmap[PIDMAP_ENTRIES];   //分配pid的位图
	struct rcu_head rcu;
	int last_pid;
	unsigned int nr_hashed;
	struct task_struct *child_reaper;   /*
                                         *托管进程，如果父进程先于子进程退出，托管进程会对孤儿进程调用wait4
                                         *每个命名空间都具有的进程，发挥的作用相当于全局的init进程
                                         */
	struct kmem_cache *pid_cachep;      //分配pid的slab地址
	unsigned int level;                 //当前命名空间的级别，父进程 level = n，则子进程 level = n+1
	struct pid_namespace *parent;       //父命名空间
#ifdef CONFIG_PROC_FS
	struct vfsmount *proc_mnt;
	struct dentry *proc_self;
	struct dentry *proc_thread_self;
#endif
#ifdef CONFIG_BSD_PROCESS_ACCT
	struct fs_pin *bacct;
#endif
	struct user_namespace *user_ns;
	struct work_struct proc_work;
	kgid_t pid_gid;
	int hide_pid;
	int reboot;	/* group exit code if this pidns was rebooted */
	struct ns_common ns;
};
```

## pid与upid

### pid

```
struct pid
{
	atomic_t count;                 //该pid被不同task引用的次数(不同的命名空间，可以存在相同的pid)
	unsigned int level;             
	/* lists of tasks that use this pid */
	struct hlist_head tasks[PIDTYPE_MAX];   //指向与该pid相连的task
	struct rcu_head rcu;
	struct upid numbers[1];         //变长数组，存储upid结构，upid是pid与pid_namespace相关联的结构
};
```

### upid

```
/*
 * struct upid is used to get the id of the struct pid, as it is
 * seen in particular namespace. Later the struct pid is found with
 * find_pid_ns() using the int nr and struct pid_namespace *ns.
 */

struct upid {
	/* Try to keep pid_chain in the same cacheline as nr for find_vpid */
	int nr;                         //PID具体的值
	struct pid_namespace *ns;       //指向命名空间的指针
	struct hlist_node pid_chain;    //指向PID哈希列表的指针，用于关联对应的PID
};
```

## pidmap，用于分配pid的位图

pidmap在pid_namespace结构体中使用，用来标记当前命名空间内pid的使用信息。

```
struct pid_namespace {
	...
	struct pidmap pidmap[PIDMAP_ENTRIES];   //分配pid的位图
	...
};
```

## pid_link

pid_link指向了和该task_struct结构体相关的pid结构体。

pid_link在task_struct中的使用：

```
struct task_struct {
	...
	/* PID/PID hash table linkage. */
	struct pid_link pids[PIDTYPE_MAX];
	...
};
enum pid_type
{
	PIDTYPE_PID,
	PIDTYPE_PGID,
	PIDTYPE_SID,
	PIDTYPE_MAX
};
```

pid_link的定义如下：

```
struct pid_link
{
	struct hlist_node node;
	struct pid *pid;
};
```

## task_struct中的PID信息

* 多个task_struct可以共用同一个PID
* 一个PID可以属于不同的namespace
* 当需要分配一个新的PID时，查找pid_namespace->pidmap即可

# 内核中的实现

Linux内核在进行管理时，需要解决以下几个问题：

1. 如何快速的根据task_struct、ID类型、namespace找到局部ID
2. 如何快速的根据局部ID、ID类型、namespace找到task_struct
3. 如何快速的给一个新进程在某个namespace中分配一个PID

## 如何快速的根据局部ID、ID类型、namespace找到task_struct
1. 首先通过 PID 计算 pid 挂接到哈希表 pid_hash[] 的表项

2. 遍历该表项，找到 pid 结构体中 nr 值与 PID 值相同的那个 pid

3. 再通过该 pid 结构体的 tasks 指针找到 node

4. 最后根据内核的 container_of 机制就能找到 task_struct 结构体

## 如何快速地根据task_struct、ID类型、namespace找到局部ID
已知 task_struct 结构体，根据其 pid_link 的 pid 指针找到 pid 结构体，取出其 nr 即为 PID 号。

## 如何快速的给一个新进程在某个namespace中分配一个PID
pid_map

# Linux中的 task_struct, pid, upid
```
enum pid_type
{
    PIDTYPE_PID,
    PIDTYPE_PGID,
    PIDTYPE_SID,
    PIDTYPE_MAX
};

struct task_struct
{
    //...
    pid_t pid; //PID
    pid_t tgid; //thread group id
    //..
    struct pid_link pids[PIDTYPE_MAX];    
    struct task_struct *group_leader; // threadgroup leader
    //...
    struct pid_link pids[PIDTYPE_MAX];  
    struct nsproxy *nsproxy;  
};

struct pid_link
{
    struct hlist_node node;
    struct pid *pid;
};

struct pid
{
	atomic_t count;                             //引用计数
	unsigned int level;                         //深度，即有几个namespace可以看到这个pid(root为0，因此一共有level+1个)
	/* lists of tasks that use this pid */
	struct hlist_head tasks[PIDTYPE_MAX];       //引用这个pid结构的tasks hashlist，指向pid_link中的node
	struct rcu_head rcu;
	struct upid numbers[1];                     /*可变长度
	                                             *一个pid可能属于不同的namespace, numbers[0]表示global namespace
	                                             *numbers[i]表示第i层namespace
												 *ns = pid->numbers[pid->level].ns
	                                             */
};

struct upid {
	/* Try to keep pid_chain in the same cacheline as nr for find_vpid */
	int nr;
	struct pid_namespace *ns;
	struct hlist_node pid_chain;
};
```