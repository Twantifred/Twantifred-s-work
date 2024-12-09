# Digital_circuit

-->> https://nju-projectn.github.io/dlco-lecture-note/index.html


## 代码管理

- 当你的代码完成开发，可以提交本地代码

```bash
# 进入工程主目录
git status		# 查看你的修改状态
git add .
git commit -m "必要的注释信息"
git push		# 将本地代码提交到远程代码托管平台
```

`git push` 时**请务必留意日志信息**，可能因网络或提交冲突等原因 push 失败

- 若提交冲突，请先更新本地代码

```bash
git pull		# 更新本地代码
# 再提交代码
```

`git pull` 时**请务必留意日志信息**，可能因网络或合并冲突等原因 pull 失败

- 若合并冲突，请手动解决冲突文件，可以联系文件冲突当事人协商处理，并重新提交本地代码

```bash
# 手动解决冲突文件
# 再提交代码
```
