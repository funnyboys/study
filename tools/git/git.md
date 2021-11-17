# 1. 查看和切换分支
## 1.1. 查看本地分支
git branch

## 1.2. 查看远程分支
git branch -r

## 1.3. 查看所有分支
git branch -a
列表中星号表示为当前使用的分支。

## 1.4. 切换分支
git checkout 分支名

# 2. 删除分支
## 2.1. 删除本地分支
git branch -d bug_xzx

## 2.2. 删除remote分支
git push origin --delete bug_xzx

# 3. https和ssh的clone方式区别
## 3.1. https
不管是谁，拿到url随便clone，但是在push的时候需要验证用户名和密码。

## 3.2. ssh
clone的项目你必须是拥有者或者管理员，而且需要在clone前添加SSH Key。
添加ssh key之后，git push不再需要密码。