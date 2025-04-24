#!/bin/bash

#在 Git Bash 的模拟环境里，`/bin/bash` 实际会映射到 Git Bash 安装目录里的 bash.exe，虽然物理路径不是 /bin/bash，但逻辑上是兼容的喵！

git add .
git commit -m "autosave"
git push