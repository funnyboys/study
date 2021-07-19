# ����ID��
LinuxΪÿ�����̷���һ��ID��Ϊ�������ռ��еı�ʶ��ID�ж�������:

1. ���̴���ĳ���߳���ʱ��ӵ���߳���ID(TGID)(���û���߳��飬��PID��TGID��ͬ)��
2. �������̿��Ժϲ��ɽ����飬�������Ա��task_struct��pgrp����ֵ��ͬ(Ϊ�������鳤PID)(�ùܵ����ӵĽ��̰�����ͬһ����������)��
3. ������������Ժϲ���һ���Ự���Ự�����н��̶��лỰID(SID)��������task_struct�е�session�С�

# ����ID����
����ID������include/linux/pid.h�ж������£�
```
enum pid_type
{
	PIDTYPE_PID,
	PIDTYPE_PGID,
	PIDTYPE_SID,
	PIDTYPE_MAX
};
```

# PS����鿴�߳�
```
huangchangwei@ubuntu:~/code_test/thread$ ps -T
   PID   SPID TTY          TIME CMD
 99855  99855 pts/15   00:00:00 bash
163829 163829 pts/15   00:00:00 a.out
163829 163830 pts/15   00:00:00 a.out
164038 164038 pts/15   00:00:00 ps
```

## PID�Ľ���
pid         PID       a number representing the process ID (alias tgid).
�߳���ID����task_struct->tgid

## SPID�Ľ���
spid        SPID      see lwp.  (alias lwp, tid).
lwp         LWP       light weight process (thread) ID of the dispatchable entity (alias spid, tid).  See tid for additional information.
�������̱߳�ʶ����task_struct->pid


# PID
pid��Linux�������ռ���Ψһ��ʶ���̶������ID����������ID�ţ���PID����ʹ��fork����cloneϵͳ����ʱ�����Ľ��̣��ں˻����һ���µ�Ψһ��PIDֵ��

pid�����ں�Ψһ����ÿ�����̡�

# TGID(thread group id)
��һ�������У������CLONE_THREAD��־����clone�����Ľ��̾��Ǹý��̵�һ���߳�(����������)������λ��һ���߳��顣

�߳����е������̵߳�ID�����߳���ID��
λ����ͬ�߳����е��߳�����ͬ��TGID���������ǵ�PID������ͬ��

# PGID(Process group id)
�����Ľ��̿�����ɽ�����(setpgrpϵͳ����)����������Լ����������ڽ��̷����źŵĲ�����

�����ùܵ����ӵĽ��̴���ͬһ�������ڣ�������ID����PGID���������ڵ����н�������ͬ��PGID�������鳤��PID��

# SID(Session id)
������������Ժϲ���һ���Ự��(setsidϵͳ����)��
�Ự�������н��̶�����ͬ��SID��������task_struct->session�С�