# read和fread的区别
## 参考
https://stackoverflow.com/questions/584142/what-is-the-difference-between-read-and-fread#:~:text=read%20%28%29%20is%20a%20low%20level%2C%20unbuffered%20read.,%28%29%20in%20order%20to%20fill%20its%20buffer.%20Share.

## read
read() is a low level, unbuffered read. It makes a direct system call on UNIX.

## fread
fread() is part of the C library, and provides buffered reads. It is usually implemented by calling read() in order to fill its buffer.

# buffered, unbuffered, direct IO的区别
## 参考
https://blog.csdn.net/codinghonor/article/details/45272127

## buffered IO
buffered IO是指在用户态增加了一层缓冲buffer，用来提高IO读写效率。
当用户读取数据时，用户缓冲区已经将buffer读满，下次读时，可以直接从buffer中读，无需再经过内核和硬盘。
当用户写数据时，写入用户buffer区，定时将这段buffer的数据flush到硬盘上，减少向内核发送write命令的次数。
* 读取：disk -> 内核缓冲区 -> 用户缓冲区 -> 用户程序
* 写回：用户程序 -> 用户缓冲区 -> 内核缓冲区 -> disk

## unbuffered IO
用户态没有缓冲区，读写直接发给内核。
* 读取：disk -> 内核缓冲区 -> 用户程序
* 写回：用户程序 -> 内核缓冲区 -> disk

## direct IO
没有缓冲区，直接与硬件交互
* 读取：disk -> 用户程序
* 写回：用户程序 -> disk