# 寄存器
## 寄存器
### x29
elr寄存器。
### x30
lr寄存器，保存函数的返回地址，当ret指令执行后，会寻找x30寄存器保存的地址。
# 指令
## 条件指令
### cbz
condition branch zero
判断寄存器的值是否为0，如果是0，则跳转到对应的label。
```
cbz x0, 0f      // 如果x0 == 0，跳转到label为0的地址
```
### ldr
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
1:	ldr	w4, [x2], #4
```