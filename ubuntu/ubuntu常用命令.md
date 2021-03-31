# 包管理
## 查找可用的包
```
yum list | grep 7z
```

## 安装deb包
```
sudo rpm -ivh TrueMeet.deb
```

## 更新deb包
```
sudo rpm -Uvh
```

## 卸载包
```
sudo yum remove krdc.x86_64
```

## 查找已经安装的包
```
yum list installed | grep krdc
```

# terminal快捷键
## 新建tab
ctrl + shift + t

## 关闭当前tab
ctrl + d

# 常用技巧或工具
## ssh设置心跳防止被关闭
https://blog.csdn.net/earl_yuan/article/details/50454032
修改 /etc/

## ssh命令带密码操作
使用sshpass工具，在ssh命令前通过sshpass参数。
```
先安装一个软件包
yum install -y sshpass

在ssh命令的前面带上sshpass
sshpass -p 'redhat' ssh root@192.168.1.1
```