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