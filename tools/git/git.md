# 查看和切换分支
## 查看本地分支
git branch

## 查看远程分支
git branch -r

## 查看所有分支
git branch -a
列表中星号表示为当前使用的分支。

## 切换分支
git checkout 分支名

# 删除分支
## 删除本地分支
git branch -d bug_xzx

## 删除remote分支
git push origin --delete bug_xzx

# https和ssh的clone方式区别
## https
不管是谁，拿到url随便clone，但是在push的时候需要验证用户名和密码。

## ssh
clone的项目你必须是拥有者或者管理员，而且需要在clone前添加SSH Key。
添加ssh key之后，git push不再需要密码。