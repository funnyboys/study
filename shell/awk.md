# NF 和 \$NF
```
NF 表示每一行字段的个数，即有多少个被空格或者TAB分隔开的字段
$NF 表示最后一个列，即每一行最后一个字段的内容
```

文件内容：
```
huangchangwei@ubuntu:~/temp$ cat tmp.txt
a   b   c   d
11  22  33  44
ff  gg
hh
```

awk '{print NF}'打印每一行有多少个字段：
```
huangchangwei@ubuntu:~/temp$ cat tmp.txt | awk '{print NF}'
4
4
3
1
```

awk '{print $NF}'打印每一行最后一个字段的内容：
```
huangchangwei@ubuntu:~/temp$ cat tmp.txt | awk '{print $NF}'
d
44

hh
```

# -F
```
-F fs or --field-separator fs
指定输入文件分隔符，fs是一个字符串或者是一个正则表达式，如-F：。
-F 选项用于对文件进行分割，-F后面跟的是分割关键词
分割后左边为 {print \$1} ，右边为 {print \$2}
```

文件内容
```
huangchangwei@ubuntu:~/temp$ cat tmp.txt
  1 0x0000000080000b1a cdef
```

分割后的左边内容
```
huangchangwei@ubuntu:~/temp$ cat tmp.txt | awk -F '000800' '{print $1}'
  1 0x00000
```

分割后的右边内容
```
huangchangwei@ubuntu:~/temp$ cat tmp.txt | awk -F '000800' '{print $2}'
00b1a cdef
```

# background
## awk中的\$0和$1
awk默认以行为单位处理文本，对输入文件中的每一行都执行后面 "{}" 中的语句。
awk中的两个术语：
* 记录（默认指文本的每一行）
* 字段（默认指每个记录中由空格或TAB分隔的字符串）
\$0表示一个记录，\$1表示记录中的第一个字段。
一般 print \$0 指打印整行内容（$0前面不需要反斜杠），print \$1表示只打印每行第一个字段。

文件内容：
```
huangchangwei@ubuntu:~/temp$ cat tmp.txt
a   b   c   d
11  22  33  44
ff  gg
hh
```

awk '{print $0}'打印所有内容：
```
huangchangwei@ubuntu:~/temp$ cat tmp.txt | awk '{print $0}'
a   b   c   d
11  22  33  44
ff  gg
hh
```

awk '{print $1}'打印第一列：
```
huangchangwei@ubuntu:~/temp$ cat tmp.txt | awk '{print $1}'
a
11
ff
hh
```