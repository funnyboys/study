# 1. 显示空格和tab
https://www.jianshu.com/p/99ffaf2e4e8b

# 2. 环境变量配置
https://zhuanlan.zhihu.com/p/96819625

# 3. 左下角显示大纲
https://blog.csdn.net/shuipaopaojin/article/details/95939081

# 4. 支持搜索符号
## 4.1. 参考
https://blog.csdn.net/u010230055/article/details/113182496

## 4.2. 步骤
### 4.2.1. 安装GNU Global
```
1. 下载源码https://ftp.gnu.org/pub/gnu/global/
2. ./configure && make && make install
```

### 4.2.2. 修改GNU Global设置
修改GNU Global配置文件，增加gtags和global目录
```
"gnuGlobal.globalExecutable": "/usr/local/bin/global",
"gnuGlobal.gtagsExecutable": "/usr/local/bin/gtags",
```

### 4.2.3. 生成tags
有两种方法生成：
1. 使用vscode快捷键F1生成
2. 在命令行下生成
```
// 在当前工作区目录下执行gtags生成tag
$ gtags optee_os_3.11.0/
```

### 4.2.4. 使用
都设置成功后，使用快捷键 F10

## 4.3. 遇到的问题
### 4.3.1. 缺少curses库
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

# 5. 快捷键
## 5.1. 跳转回上个位置
ctrl + alt + -

## 5.2. 跳转回下个位置
ctrl + shift + -

## 5.3. 放到或缩小字体
ctrl + `+`
ctrl + `-`

## 5.4. 搜索文件
ctrl + p

## 5.5. 预览markdown
ctrl + k + v

## 5.6. 搜索设置项
F1

## 5.7. 搜索符号
前置条件：已经用 tags 生成了符号
F10

## 5.8. 开启分屏
ctrl + \

## 5.9. 合并所有分屏
通过快捷键实现
文件 -> 首选项 -> 键盘快捷方式 -> 视图：合并所有编辑器组
ctrl + alt + \

## 5.10. 开启新窗口来打开文件夹
ctrl + shift + n

## 5.11. 在编辑器和terminal之间跳转
参考：
https://blog.csdn.net/grllery/article/details/84452750

设置编辑器为 ctrl + 1
设置terminal 为 ctrl + 2
关闭terminal 为 ctrl + j

## 5.12. 使用 clang-format 格式化代码
快捷键：
Shift + Ctrl + I

clang-format 配置：
将 .clang-format 放到工程目录。

## 5.13. 注释当前行
Ctrl + /

# 6. vscode配置remote
https://zhuanlan.zhihu.com/p/68577071
https://blog.csdn.net/ibless/article/details/118610776

# 7. vscode 离线安装插件
## 7.1. 方法
1. 进入插件市场
市场网址：[marketplace](https://marketplace.visualstudio.com/vscode)
搜索并下载对应插件。

2. 离线安装插件
直接将插件拖到 vscode 中。

## 7.2. 参考链接
https://www.php.cn/tool/vscode/440725.html

# 8. vscode 为 remote 服务器安装插件
https://www.cnblogs.com/understandWorld/articles/12856323.html

# 9. vscode 中使用 gdb
1. 环境搭建
https://blog.csdn.net/zztiger123/article/details/105544640

2. 使用
主要需要注意参数的配置，这里以调试 openssl 为例：
```
{
    // 使用 IntelliSense 了解相关属性。
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "type": "lldb",
            "request": "launch",
            "name": "Debug",
            "program": "${workspaceFolder}/apps/openssl",   // 要调试的二进制
            "args": ["speed", "-evp", "des-ede3"],          // 传递给二进制的参数
            "cwd": "${workspaceFolder}"
        }
    ]
}
```