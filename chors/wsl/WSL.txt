## wsl2和win10的交互复制粘贴方式.



	从https://www.c3scripts.com/tutorials/msdos/paste.zip
	下载paste.exe, 放在`C:\Windows\System32`中.
	配置 Vim 快捷键
	在~/.vimrc文件中添加以下内容：
	```
		" 复制到Windows剪贴板（Visual模式下选中文本按;y）
		vmap ;y : !/mnt/c/Windows/System32/clip.exe<CR>u''

		" 从Windows剪贴板粘贴（任何模式下按;p）
		map ;p :read !/mnt/c/Windows/System32/paste.exe <CR>
		map! ;p <Esc>:read !/mnt/c/Windows/System32/paste.exe <CR>
	```

	目前似乎;y会强制复制选中部分的完整行。无伤大雅...
	有时似乎;y会导致gbk-utf-8的乱码. 比如选中一个单独行`a(单个字母)`的时候,我暂时无法解释...

## 运行脚本
	比如当前目录有一个init.sh脚本, 直接运行一个脚本文件（如 init.sh）需要满足以下条件之一：

	* 脚本文件具有可执行权限：你可以通过 chmod +x init.sh 赋予脚本可执行权限。

	* 显式指定解释器：如果你没有赋予脚本可执行权限，可以通过显式调用解释器来运行脚本，例如 bash init.sh。
	
	
## 环境变量
	查看某个环境变量目前的值, echo $(环境变量名).
	使用export命令在当前终端临时给某个环境变量赋值, export NVBOARD_HOME=~/ysyx-workbench/nvboard
	将export命令写入./bashrc里, 直接在永久对某个环境变量赋值(退出系统重进仍然生效), echo 'export NVBOARD_HOME=~/ysyx-workbench/nvboard' >> ~/.bashrc
	source ~/.bashrc	#让修改立即生效
	Linux 其实有很多“层级”来存储环境变量，不是只有 ~/.bashrc 一个地方：

	变量存储位置					作用范围						生效时机
	/etc/environment			全局（所有用户）				开机时加载
	/etc/profile				全局，但仅适用于“登录 shell”	用户登录时加载
	/etc/bash.bashrc			全局，所有 Bash 终端都生效		终端启动时加载
	~/.profile					当前用户，仅“登录 shell”有效	用户登录时加载
	~/.bashrc					当前用户，适用于交互式终端		每次打开终端时加载
	export VAR=xxx（直接运行）	仅当前 shell	立即生效，		终端关闭后失效
	
	* 短期用 export（只在当前终端生效）。
	* 长期用 ~/.bashrc 或 ~/.profile（开机自动加载）。
	* 系统级变量放 /etc/environment（所有用户都能用）。
	
	* 注意, 在bash中$(command)是命令替换, 比如`$(pwd)`会被`/home/azazel`替换.  如果表示环境变量, 不加(), 即$NVBOARD_HOME, 会被这个环境变量值`/home/azazel/nvboard`替换. 
	* 而在编写Makefile时环境变量要加括号, $(NVBOARD_HOME)会被`/home/azazel/nvboard`替换.



## 关于Makefile
	* 奶奶的, Makefile的注释必须单独成行,,,,诸如`TOPNAME = top	 # 这句话的意思xxx`会导致#前面的空格也被读进去然后就完蛋了....
	
	* **优先级**: Makefile中, 变量赋值加载顺序是语句先后. 后面覆盖前面. 可以用?=(最低优先级),  `override a=5`(最高优先级). 不过在bash里make命令行参数直接指定 `make a=5`无视Makefile有最高优先级
	
	* 关键词变量. Makefile有一些约定俗成的变量（常见的“内部变量”）. 这些变量虽然 不是 Makefile 的关键字，但 make 及编译工具默认会识别：
		* CC：指定 C 编译器（默认为 gcc）
		* CXX：指定 C++ 编译器（默认为 g++）
		* CFLAGS：C 编译选项
		* CXXFLAGS：C++ 编译选项
		* LDFLAGS：链接选项（影响最终生成的可执行文件）.
			* `LDFLAGS`是Makefile内部参数, 用于指定`链接器`linker. +=表示在已有值上append新选项. -l库名表示链接该库.
		* CPPFLAGS：预处理选项（如 -I 头文件路径）
		* LDLIBS：库文件（通常是 -l 选项，但和 LDFLAGS 不同，它出现在链接命令的 最后）
			
	* 下面是一些注释笔记.
	```Makefile
	# 那些在TARGET(:build)块外的等式, 意义是: 变量TOPNAME的值是"top", 可以用$(TOPNAME)得到字符串"top". 顺带, Makefile有严格的缩进要求, TARGET块内的命令必须缩进.
	# `?=`意味着, 考虑`INC_PATH?=`, 如果INC_PATH变量还没有被定义, 则定义其值为""(空). 如果外部已传入, 比如用户输入命令`make INC_PATH=/somepath`, 则该行语句不执行.
	#default 目标块在用户没有指定目标的时候执行. 比如用户输入`make`.  此处具体来说执行的是`./build/top`. 如果用户输入`make build`, 则执行build目标块.
	# `$^`表示所有依赖文件的列表, `@(bash command)`表示执行之后的bash command但不在终端显示命令.
	```
















