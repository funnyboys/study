# build
## 参考
[参考链接](https://optee.readthedocs.io/en/latest/building/prerequisites.html)

## 实际操作
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
```

## 遇到的问题
1. repo init 时脚本报错，SyntaxError: invalid syntax
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

# reference
## code
https://github.com/OP-TEE

## doc
https://optee.readthedocs.io/en/latest/