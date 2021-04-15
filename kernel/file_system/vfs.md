# 什么是vfs
vfs是virtual file system，它提供了对文件系统的抽象，使得上层应用读写文件系统时可以调用相同的接口。

# vfs写操作流程
```
write() ----> sys_write() ----> 文件系统的写方法 ----> 物理介质
用户空间      vfs虚拟文件系统
```

# unix文件系统
## 概念
unix系统将文件的相关信息和文件本身这两个概念进行区分，例如访问控制权限、大小、拥有者、创建时间等信息。
文件相关信息，称为文件的元数据，被存储在inode中。

## inode
### 参考
https://www.jianshu.com/p/d60a2b44e78e

### inode中的数据
unix使用inode唯一标识一个文件。
inode保存了文件的元数据，不包括具体的数据和文件名，具体内容为：
* inode编号
* 文件大小
* 占用的块数目与块大小
* 文件类型（普通文件、目录、管道等）
* 存储该文件的设备号
* 链接数目
* 读、写、执行权限
* 用户ID和组ID
* 文件的最近访问、数据最近修改时间
* inode最近修改时间

### 查看inode
#### stat
```
huangchangwei@ubuntu:~$ stat diff.txt 
  File: 'diff.txt'
  Size: 6954            Blocks: 16         IO Block: 4096   regular file
Device: 811h/2065d      Inode: 127139847   Links: 1
Access: (0644/-rw-r--r--)  Uid: ( 1011/huangchangwei)   Gid: ( 1011/huangchangwei)
Access: 2021-03-30 09:07:57.046211740 +0800
Modify: 2021-03-24 17:23:08.066307967 +0800
Change: 2021-03-24 17:23:08.066307967 +0800
 Birth: -
```

#### ls -i
```
huangchangwei@ubuntu:~$ ls -i diff.txt 
127139847 diff.txt
```

### Linux中的inode
inode会占用固定大小的空间，Linux中每个分区的inode数目是固定的。
因此，有可能出现存储空间没有占满，但因为小文件太多而占满了inode的情况。
可以利用 `df -i` 查看inode数量信息：
```
huangchangwei@ubuntu:~$ df -i
Filesystem                     Inodes    IUsed     IFree IUse% Mounted on
udev                         66029307      474  66028833    1% /dev
tmpfs                        66034744     1581  66033163    1% /run
/dev/mapper/ubuntu--vg-root  36487168   256084  36231084    1% /
tmpfs                        66034744        1  66034743    1% /dev/shm
tmpfs                        66034744        4  66034740    1% /run/lock
tmpfs                        66034744       16  66034728    1% /sys/fs/cgroup
/dev/sda1                       46848      304     46544    1% /boot
/dev/sdb1                   182968320 46958009 136010311   26% /home
tmpfs                        66034744        5  66034739    1% /run/user/1002
tmpfs                        66034744        5  66034739    1% /run/user/1005
tmpfs                        66034744        1  66034743    1% /run/user/1007
tmpfs                        66034744        5  66034739    1% /run/user/1009
tmpfs                        66034744       18  66034726    1% /run/user/1008
tmpfs                        66034744        5  66034739    1% /run/user/1000
tmpfs                        66034744        5  66034739    1% /run/user/1019
tmpfs                        66034744        5  66034739    1% /run/user/1011
tmpfs                        66034744        5  66034739    1% /run/user/1003
tmpfs                        66034744        5  66034739    1% /run/user/1001
tmpfs                        66034744        5  66034739    1% /run/user/1014
```

# Linux中常见的操作
## 复制
复制文件时，会创建一个包含新inode的新文件。
```
huangchangwei@ubuntu:~$ cp diff.txt diff_new.txt
huangchangwei@ubuntu:~$ ls -li diff.txt diff_new.txt 
127139847 -rw-r--r-- 1 huangchangwei huangchangwei 6954 Mar 24 17:23 diff.txt
127264718 -rw-r--r-- 1 huangchangwei huangchangwei 6954 Mar 30 15:32 diff_new.txt
```

## 移动
移动文件时，只是inode指向的路径发生了变化，inode编号与实际数据存储的块的位置不会变化。
```
huangchangwei@ubuntu:~$ ls -li diff.txt diff_new.txt 
127139847 -rw-r--r-- 1 huangchangwei huangchangwei 6954 Mar 24 17:23 diff.txt
127264718 -rw-r--r-- 1 huangchangwei huangchangwei 6954 Mar 30 15:32 diff_new.txt
huangchangwei@ubuntu:~$ mv diff_new.txt diff_mv.txt
huangchangwei@ubuntu:~$ ls -li diff*
127139847 -rw-r--r-- 1 huangchangwei huangchangwei 6954 Mar 24 17:23 diff.txt
127264718 -rw-r--r-- 1 huangchangwei huangchangwei 6954 Mar 30 15:32 diff_mv.txt
```

## 硬链接
硬链接就是将多个文件名指向同一个inode，可以用不同的文件名访问相同的内容。
硬链接不会创建新的inode，只会增加对应inode的链接数。
```
huangchangwei@ubuntu:~$ stat diff_mv.txt
  File: 'diff_mv.txt'
  Size: 6954            Blocks: 16         IO Block: 4096   regular file
Device: 811h/2065d      Inode: 127264718   Links: 1
Access: (0644/-rw-r--r--)  Uid: ( 1011/huangchangwei)   Gid: ( 1011/huangchangwei)
Access: 2021-03-30 15:32:00.266584428 +0800
Modify: 2021-03-30 15:32:00.266584428 +0800
Change: 2021-03-30 15:50:48.026602667 +0800
 Birth: -
huangchangwei@ubuntu:~$ ln diff_mv.txt diff_mv_hardlink.txt
huangchangwei@ubuntu:~$ stat diff_mv_hardlink.txt
  File: 'diff_mv_hardlink.txt'
  Size: 6954            Blocks: 16         IO Block: 4096   regular file
Device: 811h/2065d      Inode: 127264718   Links: 2
Access: (0644/-rw-r--r--)  Uid: ( 1011/huangchangwei)   Gid: ( 1011/huangchangwei)
Access: 2021-03-30 15:32:00.266584428 +0800
Modify: 2021-03-30 15:32:00.266584428 +0800
Change: 2021-03-30 15:50:57.770602825 +0800
 Birth: -
```

## 软链接
软链接本质是一个文件，其存储的内容是对另一个文件的指针。
对一个文件创建软链接，inode编号会不同，被指向文件的inode链接数不会增加，且可以对不存在的文件或目录创建软链接。
```
huangchangwei@ubuntu:~$ ls -li diff*
127139847 -rw-r--r-- 1 huangchangwei huangchangwei 6954 Mar 24 17:23 diff.txt
127264718 -rw-r--r-- 1 huangchangwei huangchangwei 6954 Mar 30 15:32 diff_mv.txt
huangchangwei@ubuntu:~$ 
huangchangwei@ubuntu:~$ ln -s diff_mv.txt diff_mv_softlink.txt
huangchangwei@ubuntu:~$ ls -li diff*
127139847 -rw-r--r-- 1 huangchangwei huangchangwei 6954 Mar 24 17:23 diff.txt
127264718 -rw-r--r-- 1 huangchangwei huangchangwei 6954 Mar 30 15:32 diff_mv.txt
127508630 lrwxrwxrwx 1 huangchangwei huangchangwei   11 Mar 30 15:54 diff_mv_softlink.txt -> diff_mv.txt
```

## 删除
删除文件时，会检查inode中链接数。如果链接数大于1，就只会删掉一个硬链接，不影响数据。
如果硬链接等于1，那么这个inode会被释放掉，对应的块也会被标记为空闲。

# vfs对象及数据结构
## vfs对象
* 超级块对象，代表一个具体的已安装文件系统
* 索引节点对象，代表一个具体文件
* 目录项对象，代表一个目录项，是路径的一个组成部分
* 文件对象，代表由进程打开的文件

## vfs对象对应的函数
* super_operations对象，包括内核针对特定文件系统所能调用的方法，比如write_inode()和sync_fs()等方法
* inode_operations对象，包括内核针对特定文件所能调用的方法，比如create()和link()等方法
* dentry_operations对象，包括内核针对特定目录所能调用的方法，比如d_compare()和d_delete()等方法
* file_operations对象，包括进程针对已打开文件所能调用的方法，比如read()和write()等方法

## super block
各种文件系统都必须实现super block对象，该对象用于存储特定文件系统的信息。
super block对象由super_block结构体标识，定义在文件<linux/fs.h>中。

## 疑问
1. 一个文件必然保存在文件系统中，因此一个inode必然被一个super block所管理(由i_sb指向)。
2. dentry用于建立路径名和inode之间的联系。
