# linux执行程序提示not found
## 参考
https://www.cnblogs.com/sciapex/p/9915962.html

## 解决动态库问题
```
// 1. readelf信息
readelf -a a.out

// 2. 查找需要的动态库(NEEDED)
0x00000001 (NEEDED) Shared library: [libpthread.so.0]

// 3. 下载到对应路径
```

## 解决链接器问题
```
// 1. readelf信息
readelf -a a.out

// 2. 查找interpreter
[Requesting program interpreter: /lib/ld-linux.so.3]

// 3. 下载到对应路径
注意必须是绝对路径
```