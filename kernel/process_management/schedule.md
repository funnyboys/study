## task_struct->mm

如果是用户进程，指向当前的进程地址空间。

如果是内核线程，为空(内核线程没有进程地址空间)。

## task_struct->active_mm

如果是用户进程，mm与active_mm相同，都指向进程的地址空间。

如果是内核线程，指向被借用的用户进程的地址空间(mm)。

## mm_struct->mm_users

How many users with user space?

## mm_struct->mm_count

How many references to "struct mm_struct" (users count as 1)

如果该进程被内核线程借用，那么这个计数会加一。

## context_switch

```
/*
 * linux-4.1.18.y/kernel/sched/core.c
 */
/*
 * context_switch - switch to the new MM and the new thread's register state.
 */
static inline struct rq *
context_switch(struct rq *rq, struct task_struct *prev,
           struct task_struct *next)
{
    struct mm_struct *mm, *oldmm;

    prepare_task_switch(rq, prev, next);

    mm = next->mm;
    oldmm = prev->active_mm;
    /*
     * For paravirt, this is coupled with an exit in switch_to to
     * combine the page table reload and the switch backend into
     * one hypercall.
     */
    arch_start_context_switch(prev);

	/* mm为空，要切换到内核线程，不需要刷新TLB */
    if (!mm) {
        next->active_mm = oldmm;		//更新内核线程借用的进程地址空间
        atomic_inc(&oldmm->mm_count);	//增加被借用的进程地址空间的mm_count引用计数
        enter_lazy_tlb(oldmm, next);	//下一个为内核线程，不需要刷新tlb
    } else
        switch_mm(oldmm, mm, next);

	/* 从内核线程切出 */
	/* 这里为什么没有减去之前oldmm的mm_count
	 * 这个时候还在之前的地址空间，还没有切出，因此还不能减去
	 */
    if (!prev->mm) {
        prev->active_mm = NULL;
        rq->prev_mm = oldmm;
    }
    /*
     * Since the runqueue lock will be released by the next
     * task (which is an invalid locking op but in the case
     * of the scheduler it's an obvious special-case), so we
     * do an early lockdep release here:
     */
    spin_release(&rq->lock.dep_map, 1, _THIS_IP_);

    context_tracking_task_switch(prev, next);
    /* Here we just switch the register state and the stack. */
    switch_to(prev, next, prev);
    barrier();

    return finish_task_switch(prev);
}

```



## Arm64下的__switch_to

```
#define switch_to(prev, next, last)                 \
    do {                                \
        ((last) = __switch_to((prev), (next)));         \
    } while (0)

#endif /* __ASM_GENERIC_SWITCH_TO_H */

/*
 * Thread switching.
 */
struct task_struct *__switch_to(struct task_struct *prev,
                struct task_struct *next)
{
    struct task_struct *last;

    fpsimd_thread_switch(next);
    tls_thread_switch(next);
    hw_breakpoint_thread_switch(next);
    contextidr_thread_switch(next);

    /*
     * Complete any pending TLB or cache maintenance on this CPU in case
     * the thread migrates to a different CPU.
     */
    dsb(ish);

    /* the actual thread switch */
    last = cpu_switch_to(prev, next);

    return last;
}

/*
 * Register switch for AArch64. The callee-saved registers need to be saved
 * and restored. On entry:
 *   x0 = previous task_struct (must be preserved across the switch)
 *   x1 = next task_struct
 * Previous and next are guaranteed not to be the same.
 *
 */
/* x19 - x28
 * x29 
 * callee-saved registers
 */
ENTRY(cpu_switch_to)
    add x8, x0, #THREAD_CPU_CONTEXT
    mov x9, sp
    stp x19, x20, [x8], #16     // store callee-saved registers
    stp x21, x22, [x8], #16
    stp x23, x24, [x8], #16
    stp x25, x26, [x8], #16
    stp x27, x28, [x8], #16
    stp x29, x9, [x8], #16
    str lr, [x8]
    add x8, x1, #THREAD_CPU_CONTEXT
    ldp x19, x20, [x8], #16     // restore callee-saved registers
    ldp x21, x22, [x8], #16
    ldp x23, x24, [x8], #16
    ldp x25, x26, [x8], #16
    ldp x27, x28, [x8], #16
    ldp x29, x9, [x8], #16
    ldr lr, [x8]
    mov sp, x9
    ret
ENDPROC(cpu_switch_to)
```
## task_struct->mm

如果是用户进程，指向当前的进程地址空间。

如果是内核线程，为空(内核线程没有进程地址空间)。

## task_struct->active_mm

如果是用户进程，mm与active_mm相同，都指向进程的地址空间。

如果是内核线程，指向被借用的用户进程的地址空间(mm)。

## mm_struct->mm_users

How many users with user space?

## mm_struct->mm_count

How many references to "struct mm_struct" (users count as 1)

如果该进程被内核线程借用，那么这个计数会加一。

## context_switch

```
/*
 * linux-4.1.18.y/kernel/sched/core.c
 */
/*
 * context_switch - switch to the new MM and the new thread's register state.
 */
static inline struct rq *
context_switch(struct rq *rq, struct task_struct *prev,
           struct task_struct *next)
{
    struct mm_struct *mm, *oldmm;

    prepare_task_switch(rq, prev, next);

    mm = next->mm;
    oldmm = prev->active_mm;
    /*
     * For paravirt, this is coupled with an exit in switch_to to
     * combine the page table reload and the switch backend into
     * one hypercall.
     */
    arch_start_context_switch(prev);

	/* mm为空，要切换到内核线程，不需要刷新TLB */
    if (!mm) {
        next->active_mm = oldmm;		//更新内核线程借用的进程地址空间
        atomic_inc(&oldmm->mm_count);	//增加被借用的进程地址空间的mm_count引用计数
        enter_lazy_tlb(oldmm, next);	//下一个为内核线程，不需要刷新tlb
    } else
        switch_mm(oldmm, mm, next);

	/* 从内核线程切出 */
	/* 这里为什么没有减去之前oldmm的mm_count
	 * 这个时候还在之前的地址空间，还没有切出，因此还不能减去
	 */
    if (!prev->mm) {
        prev->active_mm = NULL;
        rq->prev_mm = oldmm;
    }
    /*
     * Since the runqueue lock will be released by the next
     * task (which is an invalid locking op but in the case
     * of the scheduler it's an obvious special-case), so we
     * do an early lockdep release here:
     */
    spin_release(&rq->lock.dep_map, 1, _THIS_IP_);

    context_tracking_task_switch(prev, next);
    /* Here we just switch the register state and the stack. */
    switch_to(prev, next, prev);
    barrier();

    return finish_task_switch(prev);
}

```



## Arm64下的__switch_to

```
#define switch_to(prev, next, last)                 \
    do {                                \
        ((last) = __switch_to((prev), (next)));         \
    } while (0)

#endif /* __ASM_GENERIC_SWITCH_TO_H */

/*
 * Thread switching.
 */
struct task_struct *__switch_to(struct task_struct *prev,
                struct task_struct *next)
{
    struct task_struct *last;

    fpsimd_thread_switch(next);
    tls_thread_switch(next);
    hw_breakpoint_thread_switch(next);
    contextidr_thread_switch(next);

    /*
     * Complete any pending TLB or cache maintenance on this CPU in case
     * the thread migrates to a different CPU.
     */
    dsb(ish);

    /* the actual thread switch */
    last = cpu_switch_to(prev, next);

    return last;
}

/*
 * Register switch for AArch64. The callee-saved registers need to be saved
 * and restored. On entry:
 *   x0 = previous task_struct (must be preserved across the switch)
 *   x1 = next task_struct
 * Previous and next are guaranteed not to be the same.
 *
 */
/* x19 - x28
 * x29 
 * callee-saved registers
 */
ENTRY(cpu_switch_to)
    add x8, x0, #THREAD_CPU_CONTEXT
    mov x9, sp
    stp x19, x20, [x8], #16     // store callee-saved registers
    stp x21, x22, [x8], #16
    stp x23, x24, [x8], #16
    stp x25, x26, [x8], #16
    stp x27, x28, [x8], #16
    stp x29, x9, [x8], #16
    str lr, [x8]
    add x8, x1, #THREAD_CPU_CONTEXT
    ldp x19, x20, [x8], #16     // restore callee-saved registers
    ldp x21, x22, [x8], #16
    ldp x23, x24, [x8], #16
    ldp x25, x26, [x8], #16
    ldp x27, x28, [x8], #16
    ldp x29, x9, [x8], #16
    ldr lr, [x8]
    mov sp, x9
    ret
ENDPROC(cpu_switch_to)
```