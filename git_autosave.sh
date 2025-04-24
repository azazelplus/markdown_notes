#!/bin/bash

#在 Git Bash 的模拟环境里，`/bin/bash` 实际会映射到 Git Bash 安装目录里的 bash.exe，虽然物理路径不是 /bin/bash，但逻辑上是兼容的喵！还有如果在cmd/ps环境, shebang line不会被识别哦

#悄悄检测是否在git bash 下运行(command -v uname检查`uname`这个命令有没有被识别, 输出结果重定向到黑洞里丢弃掉, `2>&1`表示把标准错误(2)也重定向到标准输出(1), 从而不显示错误信息.
if command -v uname >/dev/null 2>&1; then
	echo "bash mode detected!"
	git add .
	git commit -m "autosave"
	git push
	echo "✅ operation complete，quiting in 2s..."
	sleep 2
else
	# 关闭命令回显, 包括这条命令本身.
	@echo off
	echo "bash not detected. try cmd mode."
	git add .
	git commit -m "autosave"
	git push
	echo "operation complete，quiting in 2s"
	timeout /T 2 >nul
fi