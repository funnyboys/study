# 错误类型
## E235000
svn up出现如下错误：
```
svn: E235000: In file '/build/subversion-aJrtQs/subversion-1.9.3/subversion/libsvn_subr/token.c' line 51: internal malfunction
Aborted (core dumped)
```

解决方法：
删除项目，重新checkout工程