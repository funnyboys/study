# 启用gdb
```
窗口1:
make qemu-gdb

窗口2：
riscv64-unknown-elf-gdb kernel/kernel
```

# gdb设置
## 取消max-value-size
提示
```
(gdb) value requires 130388 bytes, which is more than max-value-sizeQuit
```
使用如下的命令取消限制
```
(gdb) set max-value-size unlimited
```

## gdb不优化宏
修改 Makefile ，增加 -g3 和 -gdwarf-2 选项。
```
-CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb
+CFLAGS = -Wall -Werror -g3 -gdwarf-2 -O -fno-omit-frame-pointer -ggdb
```

# 快捷键
## 显示所有断点信息
```
info breakpoints
```
```
(gdb) info breakpoints
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000003ffffff10e
```
## 删除断点
```
delete 断点编号
```
```
(gdb) info breakpoints
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000003ffffff10e
        breakpoint already hit 2 times
2       breakpoint     keep y   0x0000000080000efa in main at kernel/main.c:13
(gdb) delete 1
(gdb) info breakpoints
Num     Type           Disp Enb Address            What
2       breakpoint     keep y   0x0000000080000efa in main at kernel/main.c:13
(gdb)
```
## 分屏模式同时显示汇编
```
layout split
```
分屏模式下切换窗口：
1. 切换到下一窗口
```
(gdb) fs next
Focus set to cmd window.
```
2. 指定切换窗口
```
(gdb) info win
Name       Lines Focus
src           38
status         1
cmd           20 (has focus)
(gdb) fs src
Focus set to src window.
```
## 打印结构体中的内容
```
set print pretty on
```

## 显示所有寄存器信息
```
info registers
```

## 打印指定寄存器信息
```
p $x0
在指定的寄存器名前加$
```

## 添加地址断点
```
b *0x44444444
地址前面必须要加星号
```

## 添加断点
```
b 文件名：行号
```

## 打印地址
```
p *argv
```
## 打印数组的前n个元素
```
p *argv@2
```
# 条件断点
```
watch sum               //sum有变化时停止
b sum_to if i==5        //当i=5时，在sum_to设置断点
```

# gui
## 查看当前窗口
```
(gdb) info win
Name       Lines Focus
src           32 (has focus)
status         1
cmd           16
```

## 切换到下个窗口
```
(gdb) fs next
Focus set to cmd window.
```

## 切换到指定窗口
```
(gdb) fs src
Focus set to src window.
```