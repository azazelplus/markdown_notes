




#　修改当前ssh_key文件的默认地址

如果不小心修改了当前ssh_key的路径, 比如原来的路径是
`/f/aza/code/ssh_key/ssh_key`,现在修改为了`/f/aza/code_chors/ssh_key/ssh_key`
此时访问计算机其他使用该ssh_key的仓库, 命令`git push`将会报错:

找不到`/f/aza/code/ssh_key/ssh_key`.

## 解决方法:

在git bash中运行命令
```bash
export GIT_SSH_COMMAND="ssh -i [新路径]"
```
但是这个命令下次打开git bash还要再输入一遍. 解决方法是直接写入到`/c/users/azazel`内的配置文件`.bashrc`.

运行命令打开.bashrc(如果还没有的话nano会创建新文件)
```bash
nano ~/.bashrc
```
把命令 `export GIT_SSH_COMMAND="ssh -i [新路径]"`写入到.bashrc中.

然后重启git即可. 确保`/c/users/azazel`中还正确存在文件`/bash_profile`, 没有的话就创建一个.`nano ~/.bash_profile`命令此时会创建默认的bash_profile for windows, 它包含了默认内容, 可以正确地在每次启动git时加载.bashrc的命令.








