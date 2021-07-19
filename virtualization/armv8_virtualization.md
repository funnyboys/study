# 1. Overview
## 1.1. 目的
文档主要描述了Armv8 AArch64架构下的虚拟化支持。
* 该文档包括：
` stage 2 translation, virtual exceptions, trpping `
* 该文档不包括：
    ```
    1. 一个具体的hypervisor是如何运行的
    2. 如何写一个自己的hypvervisor
    ```

# 2. Introduction to virtualization
## 2.1. 虚拟化的重要性
1. 隔离
在一个cpu核上，虚拟化提供不同虚拟机在单个物理系统上的运行隔离。

2. 高可用性
虚拟化允许在物理机之间进行无感知和透明的切换，这种技术通常用于从需要维护或替换的故障物理机中迁移出来。

3. 负载均衡
虚拟机迁移或者物理机上托管合适的负载，可以优化数据中心的硬件和电源预算。

4. 沙箱
虚拟机提供了一个测试沙箱，可以为某些测试程序提供验证环境，防止漏洞或恶意攻击。

## 2.2. standalone and hosted hypervisor
### 2.2.1. 什么是hypervisor
hypvervisor，也被叫做VMM(virtual machine monitor)，是一个用来创建和运行虚拟机的软件。

通过共享资源(cpu, memory, io)，hypervisor允许在一个宿主机上，同时运行多个guest os。

### 2.2.2. hypervisor的种类
hypervisor可以被分为两种类型：
1. standalone(type1 hypervisors)
type1 hypervisor又被称为bare-mental hypervisor，它类似一个轻量级操作系统，直接运行在硬件之上。

2. hosted(type2 hypervisors)
type2 hypervisor运行在host-os之上。

### 2.2.3. 全虚拟化和半虚拟化
* 全虚拟化：guest-os无需做任何改动即可运行。有两种实现方式：软件辅助实现和硬件辅助实现。

    * 软件辅助实现的全虚拟化：
    一般采用优先级压缩(Ring Compression)和二进制翻译(Binary Translation)实现。
    由于一些特权指令必须运行于特权级Ring0，如LGDT，通常OS kernel运行Ring0中，这样虚拟化环境中，VMM就没有Ring可用了。
    优先级压缩大致原理为：使VMM运行于Ring0，Guest OS kernel运行于Ring1，而Guest OS user运行于Ring3。优先级压缩能正确处理大部分的特权指令，但是由于x86架构存在虚拟化漏洞(部分敏感指令不能被捕获)，采用二进制翻译方法来处理这些虚拟化漏洞：VMM扫描并修改Guest的二进制代码，将无法捕获的敏感指令转换为支持虚拟化的指令。

    * 硬件辅助实现的全虚拟化
    依赖硬件虚拟化特性(VT-x/AMD-V)，基本思想是：在硬件架构上加入了足够的虚拟化功能，使其可以截获Guest OS中所有敏感指令，并进行模拟。

* 半虚拟化：guest-os需要进行修改，对不能“陷入”的敏感指令进行替换，以解决敏感指令的捕获问题。


# 3. Virtualization in AArch64
运行在EL2以及更高特权级别的软件，可以修改如下的几个虚拟化选项：
* stage2 translation(页表两级翻译)
* EL1/0 instruction and register access trapping
* Virtual exception generation

Armv8还支持其它的几个虚拟化特性：
* Secure virtualization(TEE中的虚拟化)
* Hosted, or Type 2, hypervisors
* Nested virtualization(虚拟化嵌套)

# 4. Stage 2 translation
## 4.1. 什么是stage 2 translation
* Stage-1: Virtual Address (VA) -> Intermediate Physical Address (IPA)
* Stage-2: Intermediate Physical Address (IPA) -> Physical Address (PA)

stage-1翻译在guest-os中进行，不需要trap到hypervisor中，就可以自行修改内存映射和页表信息。

stage-2翻译在hypervisor中进行(EL2)，stage-2页表寄存器只能由hypervisor进行访问修改。因此hypervisor可以控制guest-os映射哪些内存，访问哪部分地址空间。

## 4.2. ASID
### 4.2.1. 概念
ASID在TLB中使用，用以区分不同进程的TLB entry，降低flush TLB所造成的性能损失。

TLB是硬件上的一个cache，用于提升访问页表的速度。

MMU在将虚拟地址转换成物理地址时首先去TLB中找合适entry，如果能找到，则立即返回物理地址；如果TLB中找不到，则需访问Memory中的页表，且会加载相关entry到TLB中，以备下次MMU转换使用。

这个过程中存在一个问题，因为process的页表具有私密性，且kernel process和user process也需要做隔离，所以TLB中的entry需要保证只有本process可以访问。一个最简单的办法来实现这种屏蔽就是在做context switch时将TLB中的内容全部刷掉重新载入新process的entry，但是这样做效率会降低。为减少这部分消耗时间，ARM实现了在TLB中同时存放多个process页表的功能。

实现这个功能的硬件基础是，nG bit和ASID。硬件页表中有一个bit来指明当前的entry是global(nG=0，所有process都可以访问)还是non-global(nG=1，only本process允许访问)。如果是global类型，则TLB中不会tag ASID；如果是non-global类型，则TLB会tag上ASID，且MMU在TLB中查询时需要判断这个ASID和当前进程的ASID是否一致，只有一致才证明这条entry当前process有权限访问。

ASID是Arm的叫法，X86与此类比的是PCIDS(process-context identifiers)。

ASID的赋值方法参考代码。可以发现内核线程fork()时是不会初始化ASID这个值的，且低端内存在page初始化时的flag中nG=0，这表明内核空间是global的。而用户线程在fork()过程中会初始化asid = 0，且在做context switch，调度fork()的process进入RUNNING状态时会将这个asid赋为正式计算出的值，并且硬件页表中的nG有机会赋值1。

### 4.2.2. 代码
todo：内核中对asid的应用和管理

## 4.3. VMID
### 4.3.1. 概念
VMID和ASID类似，也是在TLB中使用，用以区分不同虚拟机的TLB entry(区别相同虚拟地址的不同虚拟机)。

VMID存储在VTTBR_EL2中，8或者16bit。
* VMID size由VTCR_EL2.VS bit设置修改
* 16bit VMID在ArmV8.1-A中添加

### 4.3.2. 代码
to do

## 4.4. ASID和VMID的交互
TLB可以同时使用ASID和VMID，每一个VM都有自己独立的ASID命名空间，不同的VM可能使用相同的ASID，因此ASID和VMID通常结合使用。

## 4.5. Attribute combining and overriding
stage1和stage2两级地址翻译，都包含属性信息，例如memory类型、访问权限，MMU将两部分属性信息结合起来，得到一个最终的值。

这种组合属性的方法适合大部分情况，但有时候hypervisor想重写属性信息，比如虚拟机启动阶段，hypervisor提供了一个bit来重写这些行为：
* HCR_EL2.CD. This makes all stage 1 attributes Non-cacheable
* HCR_EL2.DC. This forces stage 1 attributes to be Normal, Write-Back Cacheable
* HCR_EL2.FWB. This allows stage 2 to override the stage 1 attribute, instead of regular attribute combining

## 4.6. Emulating Memory-mapped Input/Output (MMIO)

## System Memory Management Units (SMMUs)

# 中断虚拟化
两种不同的方式来产生虚拟中断：
1. 写 HCR_EL2 寄存器的 VI VF VSE bit
    * HCR_EL2.VI = raise vIRQ
    * HCR_EL2.VF = rasie vFIQ
    * HCR_EL2.VSE = raise vSError

2. GICv2 或更高版本的 GIC ，产生虚拟中断

# 5. todo
qemu    kvm     Hypervisor

虚拟化：
    cpu虚拟化
    内存虚拟化(页表、tlb、cache)
        intel扩展页表EPT、AMD扩展页表NPT
        影子页表
    io虚拟化(dma)

## arm smmu/iommu
https://kernel.taobao.org/2020/06/ARM-SMMU-and-IOMMU/