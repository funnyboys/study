curren、内核栈、current_thread_info

内核栈：
为什么要有内核栈？
内核栈用于存放一些内核的栈信息，例如临时变量，函数调用信息。
内核栈存放在哪里？
task_struct->stack
内核栈和thread_info一起存放在一个联合体thread_union中，thread_union的大小为THREAD_SIZE。

thread_info
线程描述符，和体系结构有关的信息。

current宏
栈指针->thread_info
	sp的低字节部分改为0（起始地址THREAD_SIZE对齐）
thread_info->task

#define get_current() (current_thread_info()->task)
#define current get_current()


/*
 * how to get the thread information struct from C
 */
static inline struct thread_info *current_thread_info(void) __attribute_const__;

static inline struct thread_info *current_thread_info(void)
{
    return (struct thread_info *)
        (current_stack_pointer & ~(THREAD_SIZE - 1));
}

/*
 * low level task data that entry.S needs immediate access to.
 * __switch_to() assumes cpu_context follows immediately after cpu_domain.
 */
struct thread_info {
    unsigned long       flags;      /* low level flags */
    mm_segment_t        addr_limit; /* address limit */
    struct task_struct  *task;      /* main task structure */
    int         preempt_count;  /* 0 => preemptable, <0 => bug */
    int         cpu;        /* cpu */
};

#define THREAD_SIZE     16384
#define THREAD_START_SP     (THREAD_SIZE - 16)

union thread_union {
    struct thread_info thread_info;
    unsigned long stack[THREAD_SIZE/sizeof(long)];
};


static void print_arg(void)
{
    struct task_struct *task = current;

    printk("[%s %d] task = 0x%lx\n", __func__, __LINE__, task);
    printk("[%s %d] task->statck = 0x%lx\n", __func__, __LINE__, task->stack);
    printk("[%s %d] current_stack_pointer = 0x%lx\n", __func__, __LINE__, current_stack_pointer);
    printk("[%s %d] ~(THREAD_SIZE - 1) = 0x%lx\n", __func__, __LINE__, ~(THREAD_SIZE - 1));
    printk("[%s %d] (current_stack_pointer & ~(THREAD_SIZE - 1)) = 0x%lx\n", __func__, __LINE__, (current_stack_pointer & ~(THREAD_SIZE - 1)));
    printk("[%s %d] current_thread_info = 0x%lx\n", __func__, __LINE__, current_thread_info());
    printk("[%s %d] task->stack->task = 0x%lx\n", __func__, __LINE__, ((struct thread_info *)task->stack)->task);
    return;
}

static int __init test_module_init(void)
{
    printk("[%s %d] init!\n", __func__, __LINE__);
    print_arg();
    return 0;
}

# insmod test_module.ko
[ 5903.536202] [test_module_init 28] init!
[ 5903.540313] [print_arg 16] task = 0xffffffc00d060000
[ 5903.545394] [print_arg 17] task->statck = 0xffffffc00c220000
[ 5903.551181] [print_arg 18] current_stack_pointer = 0xffffffc00c223c20
[ 5903.557643] [print_arg 19] ~(THREAD_SIZE - 1) = 0xffffc000
[ 5903.563207] [print_arg 20] (current_stack_pointer & ~(THREAD_SIZE - 1)) = 0xffffffc00c220000
[ 5903.571755] [print_arg 21] current_thread_info = 0xffffffc00c220000
[ 5903.578033] [print_arg 22] task->stack->task = 0xffffffc00d060000