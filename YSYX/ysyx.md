**
# 0. 常用命令&环境变量





* $(nproc)
  * Number of PROCessors







# 1.小工具

## 1.1 nano

**nano 的设计哲学：**

-   **所见即所得** - 直接输入文字
    
-   **无模式切换** - 不需要在普通模式、插入模式之间切换
    
-   **快捷键组合** - 所有功能都用 Ctrl/Alt 组合键完成


#### 基本导航

-   **光标移动**：直接使用方向键
    
-   **转到文件开头**：`Alt+\` 或 `Alt+Home`
    
-   **转到文件结尾**：`Alt+/` 或 `Alt+End`
    
-   **转到行首**：`Ctrl+A`
    
-   **转到行尾**：`Ctrl+E`
    

#### 编辑操作

-   **撤销**：`Alt+U`
    
-   **重做**：`Alt+E`
    
-   **复制**：`Alt+6` 或 `Ctrl+6`（先标记，然后移动光标选择区域）
    
-   **剪切**：`Ctrl+K`
    
-   **粘贴**：`Ctrl+U`
    

#### 搜索和替换

-   **搜索**：`Ctrl+W`
    
-   **替换**：`Ctrl+\`
    

#### 文件操作

-   **保存**：`Ctrl+O`
    
-   **退出**：`Ctrl+X`
    

#### 帮助

-   **显示帮助**：`Ctrl+G` - 所有快捷键都会显示在屏幕底部












## 1.2ccache

ubuntu自装. 在`/usr/lib/ccache/`

配置:

先确定装了没有:
```bash
which ccache
```

在./.bashrc最后加入`export PATH="/usr/lib/ccache:$PATH"`


原理:`/usr/lib/ccache/gcc`是一个和gcc同名的脚本文件.

在PATH的开头插入该路径, 导致用户输入`gcc`命令的时候优先选择`/usr/lib/ccache/gcc`, 劫持了真正的gcc编译器. 而前者就是ccache设计的包装器. 它自动管理什麽时候用缓存, 什麽时候调用gcc重新编译.

类似的劫持做法:

-   **ccache**：劫持 gcc, g++, clang 等编译命令
    
-   **virtualenv**：劫持 python, pip 命令
    
-   **nvm**：劫持 node, npm 命令
    
-   **conda**：劫持各种科学计算命令



```
ccache确实跳过了完全重复的编译过程, 发挥了加速的作用. 如果和多线程编译共同使用, 编译速度还能进一步加快!

在开发项目的过程中, 有时确实会需要在清除编译结果后进行全新的编译(fresh build). 到了PA的后期, 你可能会多次编译一些包含数百个文件的库, 在这些场合下, ccache能够极大地节省编译的时间, 从而提高项目开发的效率.
```

## 1.3 fceux-am

这是一个红白机模拟器项目...


## 1.4 strace

这是 Linux 下的一个系统调用跟踪工具，它能记录程序执行过程中所有的系统调用（比如 open、read、write、mmap、execve 等）。

也就是说，它能看到程序“和内核之间都说了啥”。 

例子:

`strace -f -o strace.log make --debug=v`

| 选项               | 含义                                              |
| ---------------- | ----------------------------------------------- |
| `-f`             | 跟踪所有由主进程创建的子进程（比如 `make` 会 fork 出许多编译子进程）       |
| `-o strace.log`  | 把输出保存到 `strace.log` 文件中，而不是直接打印到终端              |
| `make --debug=v` | 执行 `make` 命令，并启用 **verbose 模式**，显示 make 的详细调试输出 |


# 3.系统调用 syscall

操作系统内核掌控了所有硬件资源（文件、内存、网络、设备等）。  
普通程序（比如 `make`、`gcc`）不能直接访问这些资源，必须通过**系统调用**来请求内核帮忙。

例如`write`, 实际上是一段汇编指令:

```asm
write:
    mov eax, 1        ; 系统调用号 1 对应 write
    mov edi, 1        ; 第一个参数 fd=1
    mov rsi, msg      ; 第二个参数 buffer
    mov edx, 5        ; 第三个参数 length
    syscall            ; 🚪 触发系统调用，进入内核
    ret
```

在C语言中, 有关内核调用的函数都是一个**陷阱接口**(wrapper).




| 调用名                                           | 作用                         | 举例说明                                                 |
| :-------------------------------------------- | :------------------------- | :--------------------------------------------------- |
| `read(fd, buf, count)`                        | 从文件描述符 `fd` 里读数据到缓冲区 `buf` | 比如从文件或终端输入读取内容                                       |
| `write(fd, buf, count)`                       | 向文件描述符 `fd` 写数据            | 比如向屏幕（stdout=1）打印输出                                  |
| `close(fd)`                                   | 关闭文件描述符                    | 类似 C 语言里的 `fclose()`                                 |
| `openat(dirfd, path, flags, mode)`            | 打开文件（新版的 open）             | 比如 `openat(AT_FDCWD, "main.c", O_RDONLY)` 打开当前目录下的文件 |
| `newfstatat(dirfd, path, statbuf, flags)`     | 获取文件的元信息（大小、时间戳、权限等）       | `make` 用它来判断「文件是否比目标新」                               |
| `brk(addr)`                                   | 改变进程的堆（heap）边界，用来申请或释放堆内存  | 类似 `malloc()` 在底层调用的东西                               |
| `mmap(addr, length, prot, flags, fd, offset)` | 申请一段内存映射区域                 | 比如加载共享库或大块文件                                         |
| `fcntl(fd, cmd, arg)`                         | 操作文件描述符的属性（比如非阻塞模式、锁）      | `gcc` 或 shell 常用它控制文件句柄                              |
| `execve(path, argv, envp)`                    | 执行一个新程序                    | `make` 用它启动 `gcc` 或其他命令                              |
| `clone()` / `fork()`                          | 创建子进程                      | make 会用来并行执行多个任务                                     |
| `exit_group(code)`                            | 结束整个进程组                    | 程序正常退出时调用的系统调用                                       |









# 4. 思想

## 4.1
![alt text](image.png)


## 4.2

![alt text](image-1.png)

# 2 nemu

## 2.0 初探框架代码


框架代码内容众多, 其中包含了很多在后续阶段中才使用的代码. 随着实验进度的推进, 我们会逐渐解释所有的代码. 因此在阅读代码的时候, 你只需要关心和当前进度相关的模块就可以了, **不要纠缠于和当前进度无关的代码, 否则将会给你的心灵带来不必要的恐惧.**


```
ics2024
├── abstract-machine   # 抽象计算机
├── am-kernels         # 基于抽象计算机开发的应用程序
├── fceux-am           # 红白机模拟器
├── init.sh            # 初始化脚本
├── Makefile           # 用于工程打包提交
├── nemu               # NEMU
└── README.md


nemu
├── configs                    # 预先提供的一些配置文件
├── include                    # 存放全局使用的头文件
│   ├── common.h               # 公用的头文件
│   ├── config                 # 配置系统生成的头文件, 用于维护配置选项更新的时间戳
│   ├── cpu
│   │   ├── cpu.h
│   │   ├── decode.h           # 译码相关
│   │   ├── difftest.h
│   │   └── ifetch.h           # 取指相关
│   ├── debug.h                # 一些方便调试用的宏
│   ├── device                 # 设备相关
│   ├── difftest-def.h
│   ├── generated
│   │   └── autoconf.h         # 配置系统生成的头文件, 用于根据配置信息定义相关的宏
│   ├── isa.h                  # ISA相关
│   ├── macro.h                # 一些方便的宏定义
│   ├── memory                 # 访问内存相关
│   └── utils.h
├── Kconfig                    # 配置信息管理的规则
├── Makefile                   # Makefile构建脚本
├── README.md
├── resource                   # 一些辅助资源
├── scripts                    # Makefile构建脚本
│   ├── build.mk   #编译规则. 包含源文件和头文件的依赖关系. 由gcc的-MMD选项生成, 通过fixdep工具处理.
│   ├── config.mk
│   ├── git.mk                 # git版本控制相关
│   └── native.mk   # 一些用于运行和清除编译结果的.PHONY目标
├── src                        # 源文件
│   ├── cpu
│   │   └── cpu-exec.c         # 指令执行的主循环
│   ├── device                 # 设备相关
│   ├── engine
│   │   └── interpreter        # 解释器的实现
│   ├── filelist.mk
│   ├── isa                    # ISA相关的实现
│   │   ├── mips32
│   │   ├── riscv32
│   │   ├── riscv64
│   │   └── x86
│   ├── memory                 # 内存访问的实现
│   ├── monitor                #监视器.
│   │   ├── monitor.c
│   │   └── sdb                # 简易调试器
│   │       ├── expr.c         # 表达式求值的实现
│   │       ├── sdb.c          # 简易调试器的命令处理
│   │       └── watchpoint.c   # 监视点的实现
│   ├── nemu-main.c            # 你知道的...
│   └── utils                  # 一些公共的功能
│       ├── log.c              # 日志文件相关
│       ├── rand.c
│       ├── state.c
│       └── timer.c
└── tools                      # 一些工具
    ├── fixdep                 # 依赖修复, 配合配置系统进行使用
    ├── gen-expr
    ├── kconfig                # 配置系统
    ├── kvm-diff
    ├── qemu-diff
    └── spike-diff

```


NEMU主要由4个模块构成: 
* monitor, 
  * Monitor(监视器)模块是为了方便地监控客户计算机的运行状态而引入的. 它除了负责与GNU/Linux进行交互(例如读入客户程序)之外, 还带有调试器的功能, 为NEMU的调试提供了方便的途径. 从概念上来说, monitor并不属于一个计算机的必要组成部分, 但对NEMU来说, 它是必要的基础设施. 如果缺少monitor模块, 对NEMU的调试将会变得十分困难.
* CPU, 
* memory, 
* 设备. 



我们已经在上一小节简单介绍了CPU和memory的功能, 设备会在PA2中介绍.



这个页面对上述API进行了整理, 供将来查阅使用, 目前你无需完全明白它们的作用. "抽象"是计算机系统中一个非常重要的概念, 如果你现在不明白抽象的意义, 不必担心, 在PA的后续内容中, 你会一次又一次地遇到它.

https://ysyx.oscc.cc/docs/ics-pa/nemu-isa-api.html



###  2.0.1 配置系统 kconfig

```
在一个有一定规模的项目中, 可配置选项的数量可能会非常多, 而且配置选项之间可能会存在关联, 比如打开配置选项A之后, 配置选项B就必须是某个值. 直接让开发者去管理这些配置选项是很容易出错的, 比如修改选项A之后, 可能会忘记修改和选项A有关联的选项B. 配置系统的出现则是为了解决这个问题.
```

NEMU中的配置系统位于`nemu/tools/kconfig`, 它来源于GNU/Linux项目中的kconfig, 我们进行了少量简化. kconfig定义了一套简单的语言, 开发者可以使用这套语言来编写"配置描述文件". 在"配置描述文件"中, 开发者可以描述:

-   配置选项的属性, 包括类型, 默认值等
-   不同配置选项之间的关系
-   配置选项的层次关系

在NEMU项目中, "配置描述文件"的文件名都为`Kconfig`, 如`nemu/Kconfig`. 当你键入`make menuconfig`的时候, 背后其实发生了如下事件:

-   检查`nemu/tools/kconfig/build/mconf`程序是否存在, 若不存在, 则编译并生成`mconf`
-   检查`nemu/tools/kconfig/build/conf`程序是否存在, 若不存在, 则编译并生成`conf`
-   运行命令`mconf nemu/Kconfig`, 此时`mconf`将会解析`nemu/Kconfig`中的描述, 以菜单树的形式展示各种配置选项, 供开发者进行选择
-   退出菜单时, `mconf`会把开发者选择的结果记录到`nemu/.config`文件中
-   运行命令`conf --syncconfig nemu/Kconfig`, 此时`conf`将会解析`nemu/Kconfig`中的描述, 并读取选择结果`nemu/.config`, 结合两者来生成如下文件:
    -   可以被包含到C代码中的宏定义(`nemu/include/generated/autoconf.h`), 这些宏的名称都是形如`CONFIG_xxx`的形式
    -   可以被包含到Makefile中的变量定义(`nemu/include/config/auto.conf`)
    -   可以被包含到Makefile中的, 和"配置描述文件"相关的依赖规则(`nemu/include/config/auto.conf.cmd`), 为了阅读代码, 我们可以不必关心它
    -   通过时间戳来维护配置选项变化的目录树`nemu/include/config/`, 它会配合另一个工具`nemu/tools/fixdep`来使用, 用于在更新配置选项后节省不必要的文件编译, 为了阅读代码, 我们可以不必关心它




























## 2.1 开始阅读代码: main

main在哪里? 

在nemu/src/nemu-main.c


//如何找一个项目的main? 最好的方法: gdb在main符号处打断点

![alt text](image-2.png)


开始读main吧!


![alt text](image-3.png)

注意这里, 以前遇到过这个bug, 不换行的话不能及时正常输出, 这是因为输出缓冲区. 具体参考`man setbuf`

![alt text](image-4.png)



![alt text](image-5.png)


![alt text](image-6.png)


![alt text](image-7.png)

![alt text](image-8.png)


![alt text](image-9.png)


## kconfig menuconfig工具链:

## Kconfig工具

Kconfig是一种领域特定语言 (DSL).

用于描述配置选项、依赖关系、默认值.

menuconfig是一个图形化界面工具, 它可以让用户方便地修改配置选项. 它的实现是是调用 `scripts/config.mk` 中定义的 `menuconfig` 伪目标:

```Makefile
# 伪目标menuconfig. 
# $(Q) 让命令静默执行.
# $(MCONF) → menuconfig菜单界面程序
# $(CONF) → 配置同步程序
# $(FIXDEP) → 依赖关系修复工具
menuconfig: $(MCONF) $(CONF) $(FIXDEP)
# 执行命令`mconf nemu/Kconfig`, 启动菜单配置界面
	$(Q)$(MCONF) $(Kconfig)
# 同步配置. 
	$(Q)$(CONF) $(silent) --syncconfig $(Kconfig)

savedefconfig: $(CONF)
	$(Q)$< $(silent) --$@=configs/defconfig $(Kconfig)

%defconfig: $(CONF) $(FIXDEP)
	$(Q)$< $(silent) --defconfig=configs/$@ $(Kconfig)
	$(Q)$< $(silent) --syncconfig $(Kconfig)


.PHONY: menuconfig savedefconfig defconfig
```

**这是一个通过 Makefile 规则注入的入口点。**

使用menuconfig的方式即输入`make menuconfig`, 运行这个make伪目标的.

* **第一行命令**会运行可执行文件`nemu/tools/kconfig/build/mconf`, 同时传入参数`nemu/Kconfig`配置文件. 用户在GUI配置选项后, 会改变`nemu/.config`文件内容.
  * `nemu/Kconfig`是**配置定义文件**. 它定义了NEMU项目中所有的配置选项, 以及它们之间的关系. 在Kconfig工作流中, 它只被读取, 不用来写入.
  * `nemu/.config`是**配置存储文件**
* **第二行命令**是可执行文件`nemu/tools/kconfig/build/conf`.
  * `conf` 程序有多种运行模式:
      ```
      enum input_mode {
          defconfig,        // 读取指定的 defconfig 文件
          savedefconfig,    // 保存配置到指定文件
          syncconfig,       // 同步配置（默认用 .config）
          // ...
      }
      ```
      此处使用的是`syncconfig`(同步配置)模式. 该模式选择配置文件(.config)的策略:
      * 先检查环境变量`KCONFIG_CONFIG`是否存在. 存在就用这个路径.
      * 如果不存在, 使用默认路径`./.config`. 此处就是`nemu/.config`.
  * conf程序最终输出文件在./include目录下. 包括:
    * `./include/config/auto.conf` - 给 Makefile 用的格式
    * `./include/generated/autoconf.h` - 给 C 代码用的格式
    * `./include/config/auto.conf.cmd` - 依赖追踪文件
    * `./include/config/*.h` - 每个配置项的标记文件
*******************
*******************
*******************
**这些用户配置选项最终如何影响nemu行为?**

* 主Makefile包含了上述`auto.conf`和`auto.conf.cmd`:

```Makefile
-include $(NEMU_HOME)/include/config/auto.conf
-include $(NEMU_HOME)/include/config/auto.conf.cmd
```

执行make时:
* `auto.conf`决定参与编译的文件SRCS;
* `auto.conf.cmd`决定文件的依赖关系.

最终, 编译进去的`autoconf.h`等头文件影响程序行为.

`autoconf.h`中定义了一些形如`CONFIG_xxx`的宏, C代码中通过条件编译的功能对这些宏进行测试, 来判断是否编译某些代码.

例如, 当CONFIG_DEVICE这个宏没有定义时, 设备相关的代码就无需进行编译.



## fixdep工具

**fixdep工具的作用: 根据配置选项的变化, 只重新编译受影响的文件, 避免不必要的全部重新编译.**

它在`nemu/tools/fixdep`

背景: gcc编译时使用-MMD选项, 会产生`.d`依赖文件. 
  * 我们以前简单的项目直接gcc 产生.o文件, 主要是因为我们不使用可能发生改变的头文件. 
  * `.d`依赖文件的格式, 事实上就是Makefile.
    * 它的内容核心就是**目标文件**的一个补充**空规则**: 空规则的作用是加入新的**依赖**.

没有fixdep时, 如果用户通过menuconfig修改了配置, 如上述过程, `autoconf.h`会被conf程序重新生成. 于是, 所有包含`autoconf.h`的.c文件都会被重新编译. 整个项目几乎都要重新编译.

**depfix的工作流程:**

它的程序入口在`nemu/scripts/build.mk`的模式规则中. 我们记得, `build.mk`被主Makefile包含, 它的职责是定义.c文件的编译规则(通过模式规则方式):

```makefile
# scripts/build.mk 第 48-55 行
$(OBJ_DIR)/%.o: %.c
    @echo + CC $<
    @mkdir -p $(dir $@)
    @$(CC) $(CFLAGS) -c -o $@ $<
    $(call call_fixdep, $(@:.o=.d), $@)  # ← 在这里!
```
在每次编译完一个.c文件后, 会调用fixdep工具, 它的作用是**修正该.c文件对应的.d依赖文件**. 经过这一步修改, 每个.c文件对应的.d文件内原本依赖`autoconf.h`, 会被替换成更细粒度的多个头文件依赖, 这些头文件位于`nemu/include/config/`目录下.

```
      ├─ 读取原始 nemu-main.d
      ├─ 找出所有 CONFIG_* 的使用
      ├─ 生成细粒度依赖 (config/*.h)
      └─ 写回 nemu-main.d (覆盖)
```

***
***
***








## nemu的**寄存器初始化**和**内存**

把寄存器结构体CPU_state的定义放在nemu/src/isa/$ISA/include/isa-def.h中, 

并在nemu/src/cpu/cpu-exec.c中定义一个全局变量cpu. 

初始化寄存器的一个重要工作就是设置cpu.pc的初值, 我们需要将它设置成刚才加载客户程序的内存位置`CONFIG_MBASE`.


**nemu的物理内存是一个数组: `uint8_t pmem[CONFIG_MSIZE];` 在paddr.c中定义. 它模拟nemu的物理内存从`CONFIG_MBASE`直到`0xffffffff`.**

**nemu的模拟内存基地址为`CONFIG_MBASE`(在`autoconf.h`中定义宏).**

| 概念    | 含义         |
| -------- | ----------- |
| **guest 物理地址 (`paddr_t=`)**  | NEMU 模拟出来的“被模拟计算机”的物理内存地址，比如 0x80000000。对 NEMU 而言它只是个整数，不是真实内存位置。                 |
| **host 内存指针 (`uint8_t *`)** | 这个“被模拟计算机”的内存实际上存放在你主机（WSL/Linux）的虚拟内存中，也就是一个普通的 `malloc()` 得到的地址，比如 `0x55ab...`。 |


显然, 有数学关系: 

* **nemu物理地址-`CONFIG_MBASE` = pmem数组的偏移量**

* **pmem数组的偏移量 + pmem = 主机内存指针haddr**





地址转换由这两个函数(in `paddr.c`)完成:

注意:
  * 按字节寻址的系统(所有现代架构), 指针显然是`uint8_t*`, 即`char*`类型.


```C
//从pmem出发, 加上offset, 得到对应主机内存指针.
uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
//
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }
```

* guest_to_host函数接收一个nemu的物理地址`paddr`, 返回其对应的主机内存指针.
  * guest_to_host返回一个`uint8_t*`类型指针. 返回值本身是64位.
  * 举个例子. RISCV32对应的`CONFIG_MBASE`是`0x80000000`. 假设pmem=`0x1000_0000_0000_0000`(即nemu的物理内存是从wsl系统的pmem开始的一段数组). 对nemu的一个物理地址`0x80000010`, 此字节对应的主机内存指针haddr即为:
```
haddr = pmem + offset;
      = pmem + (paddr - CONFIG_MBASE);
      = 0x0000555555567000 + (0x80000010 - 0x80000000);
      = 0x0000555555567010;
```

* 同理, host_to_guest函数接收一个主机内存指针`haddr`, 返回其对应的nemu物理地址.



**物理内存的起始地址**

x86的物理内存是从0开始编址的, 但对于一些ISA来说却不是这样.

例如mips32和riscv32的物理地址均从`0x80000000`开始. 因此对于mips32和riscv32, 其`CONFIG_MBASE`将会被定义成`0x80000000`. 将来CPU访问内存时, 我们会将CPU将要访问的内存地址映射到`pmem`中的相应偏移位置, 这是通过`nemu/src/memory/paddr.c`中的`guest_to_host()`函数实现的. 例如如果mips32的CPU打算访问内存地址`0x80000000`, 我们会让它最终访问`pmem[0]`, 从而可以正确访问客户程序的第一条指令. 这种机制有一个专门的名字, 叫地址映射, 在后续的PA中我们还会再遇到它.



## 3. 准备第一个客户程序


Monitor读入客户程序并对寄存器进行初始化后, 这时内存的布局如下:


```
pmem:

CONFIG_MBASE      RESET_VECTOR
      |                 |
      v                 v
      -----------------------------------------------
      |                 |                  |
      |                 |    guest prog    |
      |                 |                  |
      -----------------------------------------------
                        ^
                        |
                       pc
```

load_img()函数运行后, 会将一个有意义的客户程序从镜像文件读入到内存, 覆盖刚才的内置客户程序. 这个镜像文件是运行NEMU的一个可选参数, 在运行NEMU的命令中指定. 如果运行NEMU的时候没有给出这个参数, NEMU将会运行内置客户程序.

Monitor的初始化工作结束后, `main()`函数会继续调用`engine_start()`函数 (在`nemu/src/engine/interpreter/init.c`中定义). 代码会进入简易调试器(Simple Debugger)的主循环`sdb_mainloop()` (在`nemu/src/monitor/sdb/sdb.c`中定义), 并输出NEMU的命令提示符:

```
(nemu)
```

nemu将RV32的`ebreak`指令来充当`nemu_trap`指令. 也就是说, NEMU作为模拟器, 没有忠实地模拟`ebreak`本来在RV32中被设计要做的事(产生调试异常), 而是在ID/EX时拦截它, 调用函数`NEMUTRAP(pc, R(10))`, 特殊处理. 总之就是,

**当模拟器遇到这个已选定的RV32指令时，就把它当作“客户程序要告诉模拟器退出并带状态码”的信号来处理。**


![alt text](image-17.png)


这个选项在Kconfig中定义了. 名为**CC_DEBUG**, bool类型配置项, 默认为n(不启用).

```Kconfig
# nemu/Kconfig (第 118-119 行)
config CC_DEBUG
  bool "Enable debug information"
  default n
```

当我们`make menuconfig`->打开该选项后,

将会在`.config`中保存为`CONFIG_CC_DEBUG=y`.

下次make时, conf程序重新生成的`autoconf.h`中会更改配置宏为1:
```C
// nemu/include/generated/autoconf.h
#define CONFIG_CC_DEBUG 1
```

同时, conf程序重新生成的`autoconf`中会更改配置为y:
```Makefile
CONFIG_CC_DEBUG=y
```

`autoconf`被包含在主Makefile中, 影响编译选项CFLAGS_BUILD:

```Makefile
# nemu/Makefile (第 87 行)
CFLAGS_BUILD += $(if $(CONFIG_CC_DEBUG),-Og -ggdb3,)
```

编译时, 多了选项:
* `-Og`: 类似O1优化等级, 但更注重调试体验. 
  * -O0  = 无优化,完全保留调试信息 (调试最容易,运行最慢)
  * -Og  = 优化同时保持可调试        (默认推荐)
  * -O2  = 中等优化,部分调试信息丢失 (运行快,难调试)
  * -O3  = 最高优化,调试信息严重丢失 (运行最快,几乎无法调试)
* `-ggdb3`, 为 GDB 调试器生成调试符号.













