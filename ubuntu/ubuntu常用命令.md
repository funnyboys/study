[toc]

# 1. 环境变量
## 1.1. 查看环境变量
```
$ echo $PATH
```

## 1.2. 暂时修改环境变量
```
$ export PATH="new_path/bin:$PATH"
```

## 1.3. 永久修改环境变量
修改 ~/.bashrc ，增加对应路径

# 2. 包管理
## 2.1. 查找可用的包
```
yum list | grep 7z
```

## 2.2. 安装deb包
```
sudo rpm -ivh TrueMeet.deb
```

## 2.3. 更新deb包
```
sudo rpm -Uvh
```

## 2.4. 卸载包
```
sudo yum remove krdc.x86_64
```

## 2.5. 查找已经安装的包
```
yum list installed | grep krdc
```

# 3. terminal快捷键
## 3.1. 新建tab
ctrl + shift + t

## 3.2. 关闭当前tab
ctrl + d

# 4. 常用技巧或工具
## 4.1. ssh设置心跳防止被关闭
https://blog.csdn.net/earl_yuan/article/details/50454032
修改 /etc/

## 4.2. ssh命令带密码操作
使用sshpass工具，在ssh命令前通过sshpass参数。
```
先安装一个软件包
yum install -y sshpass

在ssh命令的前面带上sshpass
sshpass -p 'redhat' ssh root@192.168.1.1
```

# 5. 压缩和解压
## 5.1. 压缩

## 5.2. 解压
### 5.2.1. 解压xz文件
tar -Jxf

### 5.2.2. 解压bz2文件
tar -jxvf

# 6. 安装beyondcompare
1. 下载rpm包
https://www.scootersoftware.com/download.php

2. 安装依赖-pdftotext
` sudo yum install poppler-utils `

3. 安装rpm包
` sudo rpm -ivh bcompare-4.3.7.25118.x86_64.rpm `

# 7. 查找包或者动态库等文件
## 7.1. locate
通过**数据库**(/var/lib/locatedb)，查找**本地**所有**文件**信息。

这个数据库中含有本地所有文件信息。Linux系统自动创建这个数据库，并且每天自动更新一次。

可以在使用locate之前，先使用updatedb命令，手动更新数据库。

## 7.2. whereis
用于搜索**程序名**，而且只搜索**二进制文件**（参数-b）、**man说明文件**（参数-m）和**源代码**文件（参数-s）。如果省略参数，则返回所有信息。

whereis命令的使用实例：
`whereis vim`

## 7.3. apt-cache search
通过 apt-cache 搜索，可以使用与其名称或描述相关的关键字来搜索任何**软件包**。

在输出中，它将显示所有符合搜索条件的软件包，例如：

```
$ apt-cache search ntfs
isolinux - collection of bootloaders (ISO 9660 bootloader)
libntfs-3g883 - read/write NTFS driver for FUSE (runtime library)
ntfs-3g - read/write NTFS driver for FUSE
ntfs-3g-dev - read/write NTFS driver for FUSE (development)
samba-vfs-modules - Samba Virtual FileSystem plugins
syslinux - collection of bootloaders (DOS FAT and NTFS bootloader)
syslinux-common - collection of bootloaders (common)
autopsy - graphical interface to SleuthKit
ddrutility - set of data recovery utilities for use with GNU ddrescue
```