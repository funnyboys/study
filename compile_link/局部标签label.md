# 在内核中的使用

```
#define _THIS_IP_  ({ __label__ __here; __here: (unsigned long)&&__here; })
```

# 作用

c语言中的goto标签，但是只是一个局部标签，防止一个宏多次引用导致多处定义相同的标签。

# 参考资料

https://www.cnblogs.com/hbt19860104/archive/2013/07/26/3216725.html