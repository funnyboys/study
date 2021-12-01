[toc]

# 1. build
## 1.1. 参考
[参考链接](https://optee.readthedocs.io/en/latest/building/prerequisites.html)

## 1.2. 实际操作
1. 更新编译环境
```
$ sudo dpkg --add-architecture i386
$ sudo apt-get update
$ sudo apt-get install android-tools-adb android-tools-fastboot autoconf \
        automake bc bison build-essential ccache codespell \
        cscope curl device-tree-compiler \
        expect flex ftp-upload gdisk iasl libattr1-dev libcap-dev \
        libfdt-dev libftdi-dev libglib2.0-dev libgmp-dev libhidapi-dev \
        libmpc-dev libncurses5-dev libpixman-1-dev libssl-dev libtool make \
        mtools netcat ninja-build python-crypto python3-crypto python-pyelftools \
        python3-pycryptodome python3-pyelftools python-serial python3-serial \
        rsync unzip uuid-dev xdg-utils xterm xz-utils zlib1g-dev
```

2. build and run
```
$ repo init -u https://github.com/OP-TEE/manifest.git -m qemu_v8.xml
$ repo sync
$ cd build
$ make toolchains
# Note that if you wish to debug optee-os or a TA, you should disable ASLR
# with flag "CFG_CORE_ASLR=n"
$ make run
QEMU console:         (qemu) c
```

# 2. 使用 gdb 调试 tee
## 2.1. 调试安全侧代码
参考：
https://optee.readthedocs.io/en/latest/building/devices/qemu.html#gdb-secure-world

实际命令：
```
$ ./aarch64-linux-gnu-gdb -q

(gdb) target remote localhost:1234
Remote debugging using localhost:1234
warning: No executable has been specified and target does not support
determining executable automatically.  Try using the "file" command.
0xffffa53309467df0 in ?? ()

(gdb) symbol-file ../../../optee_os/out/arm/core/tee.elf
Reading symbols from ../../../optee_os/out/arm/core/tee.elf...

(gdb) b tee_entry_std
Breakpoint 1 at 0xe11ea90: file core/tee/entry_std.c, line 528.

(gdb) c

```

## 2.2. 调试非安全侧代码
// todo

# 3. reference
## 3.1. code
https://github.com/OP-TEE

## 3.2. doc
https://optee.readthedocs.io/en/latest/

# 4. 问题
## 4.1. gdb 找不到 libncursesw.so.5
1. 将 source.list 替换为官方源
2. `sudo apt-get install libncursesw5`

## 4.2. repo init 时脚本报错，SyntaxError: invalid syntax
问题如下：
```
huang@huang-P65xRP:~/git/optee$ repo init -u https://github.com/OP-TEE/manifest.git -m qemu_v8.xml
gpg: keybox '/home/huang/.repoconfig/gnupg/pubring.kbx' created
gpg: /home/huang/.repoconfig/gnupg/trustdb.gpg: trustdb created
gpg: key 16530D5E920F5C65: public key "Repo Maintainer <repo@android.kernel.org>" imported
gpg: key 67B7E448692B382C: public key "Conley Owens <cco3@android.com>" imported
gpg: Total number processed: 2
gpg:               imported: 2

Get https://gerrit.googlesource.com/git-repo/clone.bundle
Get https://gerrit.googlesource.com/git-repo
  File "/home/huang/git/optee/.repo/repo/main.py", line 79
    file=sys.stderr)
        ^
SyntaxError: invalid syntax
```

原因：
系统有两个版本 python ， 2.* 和 3.* ，默认使用 2.* ，替换为 3.* 即可

1. 查看 python 版本：
```
huang@huang-P65xRP:~/git/optee$ python --version
Python 2.7.18
```

2. 使用 python3 init
```
python3 /usr/bin/repo init -u https://github.com/OP-TEE/manifest.git -m qemu_v8.xml
```
