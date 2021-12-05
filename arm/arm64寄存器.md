# 通用寄存器(general propose register)
通用寄存器用来存放一般性的数据。
64位cpu寄存器宽度为64bit，32位cpu寄存器宽度为32bit。
64位cpu兼容32位cpu，所以arm32下的寄存器就是64位对应寄存器的低32位。

```
arm64（64 bit）：
x0 - x30

arm32（32 bit）：
w0 - w30
```

特殊用法：
* x30
通常称x30为LR寄存器（link register），保存了子程序结束后需要执行的下一条指定的地址
* x0-x7
函数调用时作为入参, 超出的参数将入栈。假如在函数funcA中调用函数funcB, 传给funcB的参数超出8个的将保存在函数funcA函数栈的栈顶（具体需要看arm调用规范）
* x0
函数执行完毕后返回值保存在x0寄存器中

# 特殊寄存器
* CPSR
Current Program Status Register，描述了当前CPU的状态
* SPSR
Saved Program Status Register，描述了之前保存CPU的状态（一般用于状态的恢复，将CPSR的值保存在SPSR中，下次再从SPSR恢复到CPSR中）

# PC(Program Counter)
PC指向当前正在执行的指令地址。

# SP(Stack Pointer，堆栈寄存器)
SP指向当前函数的栈顶。

# FP(Frame Pointer)
FP指向上一个函数的栈顶。

# 参考资料
https://www.cnblogs.com/smartjourneys/p/6845180.html
DDI0487G_b_armv8_arm.pdf