# 1. 查看和切换 python 版本
## 1.1. 查看版本
```
$ python --version
Python 2.7.18
```

## 1.2. 切换版本
安装工具：
```
sudo update-alternatives --install /usr/bin/python python /usr/bin/python2 100
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 150
```

手动选择 python 版本：
```
sudo update-alternatives --config python
```

## 参考
[python版本切换](https://blog.csdn.net/thankyou0/article/details/79610854#:~:text=ubuntu%E5%88%87%E6%8D%A2python%E7%89%88%E6%9C%AC%20%E4%B8%B4%E6%97%B6%20%E5%88%87%E6%8D%A2%20%E7%B3%BB%E7%BB%9F%20%E5%88%87%E6%8D%A2%20%E4%B8%B4%E6%97%B6%20%E5%88%87%E6%8D%A2%20%E7%B3%BB%E7%BB%9F%E7%9A%84%E9%BB%98%E8%AE%A4,python%20%E5%88%99%E8%A1%A8%E7%A4%BA%20Pyt.%20Linux%20Ubuntu%2016.04%E4%B8%8B%20Python%E7%89%88%E6%9C%AC%E5%88%87%E6%8D%A2%20%E9%97%AE%E9%A2%98.)