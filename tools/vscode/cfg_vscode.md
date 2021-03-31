# 显示空格和tab
https://www.jianshu.com/p/99ffaf2e4e8b

# 环境变量配置
https://zhuanlan.zhihu.com/p/96819625

# 左下角显示大纲
https://blog.csdn.net/shuipaopaojin/article/details/95939081

# 支持搜索符号
## 参考
https://blog.csdn.net/u010230055/article/details/113182496

## 步骤
### 安装GNU Global
```
1. 下载源码https://ftp.gnu.org/pub/gnu/global/
2. ./configure && make && make install
```

### 修改GNU Global设置
修改GNU Global配置文件，增加gtags和global目录
```
"gnuGlobal.globalExecutable": "/usr/local/bin/global",
"gnuGlobal.gtagsExecutable": "/usr/local/bin/gtags",
```

### 生成tags
有两种方法生成：
1. 使用vscode快捷键F1生成
2. 在命令行下生成
```
// 在当前工作区目录下执行gtags生成tag
$ gtags optee_os_3.11.0/
```

### 使用
都设置成功后，使用快捷键 F10

## 遇到的问题
### 缺少curses库
```
curses library is required but not found
```
1. 寻找对应的包
```
$ yum list | grep curse
```
2. 安装包
```
$ sudo yum -y install ncurses-devel.x86_64
```

# 快捷键
## 跳转回上个位置
ctrl + alt + -

## 跳转回下个位置
ctrl + shift + -

## 放到或缩小字体
ctrl + `+`
ctrl + `-`

## 搜索文件
ctrl + p

## 预览markdown
ctrl + k + v

## 搜索设置项
F1