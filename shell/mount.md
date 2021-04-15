# 标准用法
`mount -t type device dir`
将设备device以type类型，mount到dir目录。
例如：
```
// 挂载debugfs
mount -t debugfs none /debug

// 挂载U盘
mount -t vfat /dev/sda1 /mnt
```
