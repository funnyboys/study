# -c(只编译不链接)
```
gcc -c test.c
```

# 机器码转汇编
```
riscv64-unknown-linux-gnu-objdump -b binary -m riscv:rv64 -D kernel1121.bin  > asm.txt
```
选项说明：
```
-b, --target=BFDNAME           Specify the target object format as BFDNAME
-m, --architecture=MACHINE     Specify the target architecture as MACHINE
```
-b和-m选项对应的参数可以通过`riscv64-unknown-linux-gnu-objdump --help`查看
-b支持的target类型：
```
riscv64-unknown-linux-gnu-objdump: supported targets: elf64-littleriscv elf32-littleriscv elf64-little elf64-big elf32-little elf32-big srec symbolsrec verilog tekhex binary ihex plugin
```
-m支持的arch类型：
```
riscv64-unknown-linux-gnu-objdump: supported architectures: riscv riscv:rv64 riscv:rv32
```

# -D(编译时增加宏定义)
```
gcc -c test.c -D DEBUG_ON
```
