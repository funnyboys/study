# ps�еĽ���״̬��
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

# ʹ��kill������ź�
## �鿴���õ��ź�ֵ
kill -l

ps�еļӺ�
+    is in the foreground process group
��ʾ������ǰ̨�������С�
���磺
./a.out &
./b.out
huangch+ 190640 99.2  0.0   4216   648 pts/23   R    11:57   0:18 ./a.out
huangch+ 190646  100  0.0   4216   780 pts/23   R+   11:57   0:05 ./b.out
a.out�ں�̨���У���b.out��ǰ̨���С�


# �ں��е�����״̬
## TASK_RUNNING - R
�������� ����
�Ѿ��ﵽ�������е�״̬�������뵽�����ж����У����ǻ�û�п�ʼ���С�

## TASK_INTERRUPTIBLE - S
���жϵ�˯��״̬
�ȴ�ĳ�����鷢��������ȴ�socket���ӣ��ȴ��ź����ȣ�����������
���̵�task_struct�������Ӧ�¼��ĵȴ������У�����Щ�¼�����ʱ����Ӧ�ĵȴ������е�һ���������̽��ᱻ���ѡ�

## TASK_UNINTERRUPTIBLE - D
�����жϵ�˯��״̬
�� TASK_INTERRUPTIBLE ���ƣ����̴���˯��״̬�����Ǵ�ʱ�����ǲ��ɱ��жϵġ�
���ɱ��ж���ָ���ý��̲���Ӧ�첽�źš���Ӳ���ж���Ȼ��Ч��
��״̬�����壺�ں˵�ĳЩ�����ǲ����Ա���ϵģ������Ӧ�첽�źţ������ִ�����̾ͻᱻ����һ�δ����첽�źŵ�����(��������̿���ֻ���ں�̬��Ҳ�������û�̬)������ԭ�е����̾ͱ�����ˡ�
��������ĳЩӲ������ʱ(�������ͨ��readϵͳ���ö��豸�ļ����ж�����)��������Ҫ TASK_UNINTERRUPTIBLE ���б�������ֹ��������ϣ������豸���벻�ɿص�״̬��

## TASK_STOPPED - T
## TASK_TRACED - t
��׷��״̬������ʹ��gdb�Խ��̽��е��ԣ��ͽ����˸�״̬��

## TASK_DEAD - Z
��ʬ����
����ռ�õ�������Դ��������