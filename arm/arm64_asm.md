# 1. 寄存器
## 1.1. 寄存器
### 1.1.1. x29
elr寄存器。
### 1.1.2. x30
lr寄存器，保存函数的返回地址，当ret指令执行后，会寻找x30寄存器保存的地址。
# 2. 指令
## 2.1. 条件指令
### 2.1.1. cbz
condition branch zero
判断寄存器的值是否为0，如果是0，则跳转到对应的label。
```
cbz x0, 0f      // 如果x0 == 0，跳转到label为0的地址
```
### 2.1.2. ldr
参考链接：
https://www.jianshu.com/p/66d801c85ee9
```
/*
 * 1. 将地址为 x2 的值读取到 w4 中
 * 2. 将 x2 + 4 的值存入 x2
 * 即：
 *    w4 = addr[x2]
 *    x2 = x2 + 4
 */
ldr	w4, [x2], #4
```
```
/*
 * 将xs寄存器的值加上offset之后的值作为内存地址，
 * 将该内存地址的数据取出，放入寄存器xd中
 */
ldr xd, [xs, #offset]
```

# 3. QA
## 3.1. 什么是banked register
```
A banked register maps one-to-one onto a user mode register. If you change processor mode, a banked register from the new mode will replace an existing register. For example, when the processor is in the interrupt request mode, the instructions you execute still access registers named r13 and r14.
```
简单来说，就是在不同的cpu状态下，相同的寄存器有各自独立的值。

## 3.2. 什么是VMID

## 3.3. 什么是VA中的two VA ranges
类似linux中的用户态地址空间和内核地址空间，用户态占据低地址，内核高地址。

## 3.4. TLB中ASID和nG bit的关系
### 3.4.1. Arm spec中对nG的说明
In a VMSA implementation, system software can divide the virtual memory map used by a stage of translation that can support two VA ranges into global and non-global regions, indicated by the nG bit in the translation table descriptors:

* nG == 0 
    The translation is global, meaning the region is available for all processes.
* nG == 1 
    The translation is non-global, or process-specific, meaning it relates to the current ASID, as defined by:
    * TTBR0_ELx.ASID, if the value of TCR_ELx.A1 is 0.
    * TTBR1_ELx.ASID, if the value of TCR_ELx.A1 is 1.

As indicated by the nG field definitions, each non-global region has an associated ASID. These identifiers mean different translation table mappings can co-exist in a caching structure such as a TLB. This means that software can create a new mapping of a non-global memory region without removing previous mappings.

Note
* The selected ASID applies to the translation of any address for which the value of the nG bit is 1, regardless of whether the address is translated based on TTBR0_ELx or on TTBR1_ELx.
* In an implementation that does not include ARMv8.1-VHE, the only stage of translation that can support two VA ranges is stage 1 of the EL1&0 translation regime. In an implementation that includes ARMv8.1-VHEstage 1 of the EL2&0 translation regime also can support two VA ranges.

ASIDs are supported only when stage 1 translations can support two VA ranges. Stage 2 translations, and stage 1 translations that can support only a single VA range do not support ASIDs, and all descriptors in these regimes are treated as global.

In a translation regime that supports global and non-global translations, translation table entries from lookup levels other than the final level of lookup are treated as being non-global, regardless of the value of the nG bit.

When a PE is using the VMSAv8-64 translation table format, and is in Secure state, a translation must be treated as non-global, regardless of the value of the nG bit, if NSTable is set to 1 at any level of the translation table walk.

For more information, see Control of Secure or Non-secure memory access on page D5-2455

### 3.4.2. TLB的目的
TLB是硬件上的一个cache，用于提升访问页表的速度。MMU在将虚拟地址转换成物理地址时首先去TLB中找匹配的entry，如果能找到，则立即返回物理地址；如果TLB中找不到，则需访问内存中的页表，并加载相关entry到TLB中，以备下次MMU转换使用。

这个过程中存在一个问题，因为process的页表具有私密性，且kernel process和user process也需要做隔离，所以TLB中的entry需要保证只有本process可以访问。

一个最简单的办法来实现这种屏蔽就是在做context switch时将TLB中的内容全部刷掉重新载入新process的entry，但是这样做效率会降低。为减少这部分消耗时间，ARM实现了在TLB中同时存放多个process页表的功能。

### 3.4.3. ASID和nG bit
实现这个功能的硬件基础是，nG bit和ASID。

硬件页表中有一个bit来指明当前的entry是global(nG=0，所有process都可以访问)还是non-global(nG=1，only本process允许访问)。
如果是global类型，则TLB中不会tag ASID。
如果是non-global类型，则TLB会tag上ASID，且MMU在TLB中查询时需要判断这个ASID和当前进程的ASID是否一致，只有一致才证明这条entry当前process有权限访问。

ASID的赋值方法参考代码：
可以发现内核线程fork()时是不会初始化ASID这个值的，且低端内存在page初始化时的flag中nG=0，这表明内核空间是global的。而用户线程在fork()过程中会初始化asid = 0，且在做context switch，调度fork()的process进入RUNNING状态时会将这个asid赋为正式计算出的值，并且硬件页表中的nG有机会赋值1。

### 3.4.4. 参考
https://stackoverflow.com/questions/17590146/how-to-deal-the-circumstance-of-asids-are-used-up-in-linux-kernel
https://blog.csdn.net/lee_ham/article/details/103107135

### 3.4.5. todo
增加内核代码分析

# 4. MIDR_EL1 和 MPIDR_EL1
## 4.1. MIDR_EL1
main id register，标识了 PE 信息，包括 device 和 device ID，重要字段如：
* Implementer, bits [31:24]
    制造商标识，如Arm、NVIDIA、Broadcom等
* Architecture, bits [19:16]
    架构标识，如ARMv4、ARMv5、ARMv6等

## 4.2. MPIDR_EL1
Multiprocessor Affinity Register，保存了 processor ID。