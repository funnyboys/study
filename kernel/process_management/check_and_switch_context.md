static inline void check_and_switch_context(struct mm_struct *mm,
                        struct task_struct *tsk)
{
    /*
     * Required during context switch to avoid speculative page table
     * walking with the wrong TTBR.
     */
    cpu_set_reserved_ttbr0();

    if (!((mm->context.id ^ cpu_last_asid) >> MAX_ASID_BITS))
        /*
         * The ASID is from the current generation, just switch to the
         * new pgd. This condition is only true for calls from
         * context_switch() and interrupts are already disabled.
         */
        cpu_switch_mm(mm->pgd, mm);
    else if (irqs_disabled())
        /*
         * Defer the new ASID allocation until after the context
         * switch critical region since __new_context() cannot be
         * called with interrupts disabled.
         */
        set_ti_thread_flag(task_thread_info(tsk), TIF_SWITCH_MM);
    else
        /*
         * That is a direct call to switch_mm() or activate_mm() with
         * interrupts enabled and a new context.
         */
        switch_new_context(mm);
}

TTBR0 TTBR1 TTBCR
TTBCR: Translation Table Base Control Register
一级转换页表控制相关：
划分虚拟地址范围，哪一部分使用TTBR0，哪一部分使用TTBR1。
控制地址翻译时的转换页表格式(translation table format)。
configuration：
