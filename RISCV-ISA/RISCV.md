











# 1. 计算机组成.

## 1.0 杂项

### 1.0.1 大端序(big-endian) & 小端序(little-endian)

指的是数据在内存中, **以字节为单位的**的存放数据.

但是在一个字节(8bit)内部, 永远是小端序.

* 小端序(x86, ARM, RISCV...):
低地址放低字节, 高地址放高字节.


* 小端序(TCP等协议):
低地址放低字节, 高地址放高字节.


例如有一个32bit数: `0x12345678`

对应4个字节分别存放`0x12`, `0x34`, `0x56`, `0x78`

小端序:

|地址 |  数据 (1字节)|
|--|--|
|0x1000 | 78   <- 最低有效字节|
|0x1001  |56|
|0x1002  |34|
|0x1003  |12   <- 最高有效字节|


我们在汇编语言无需在意, 默认小端序就可以了.

## 1.1 概述

组成计算机的5个经典部件:

* **输入**(键盘, 鼠标, LCD(带电容来触摸), 摄像头...)和**输出**(LCD(带电容来触摸), 音响...). 统称为**I/O设备**
* 存储器
* 数据通路(**运算器**)和**控制器** (合称为**处理器/中央处理单元/CPU**)
  * 数据通路(datapath)

![alt text](image-1.png)


## 1.2 内存

* **DRAM**(动态随机访问存储器. 易失性) 作为计算机的内存. 存储程序运行时的信息. 如DDR4/5内存条. 

* **SRAM**(静态随机访问存储器, 易失性)比DRAM更快, 集成度更低. 用在CPU/GPU的寄存器堆; 一些SoC也有片上SRAM作为快速工作内存. 最多几MB.

* **高速缓存**(cache memory)一般作为普通内存的缓冲.
* 主存: 可以理解为DRAM, 计算机的内存.
* 闪存/固态硬盘/SSD: 非易失性半导体存储. 更慢更便宜.  写入一定次数后损坏.
* 磁盘/硬盘/机械硬盘: 旋转盘片的磁介质材料非易失性存储. 






## 1.3 指令系统体系结构/体系结构/instruction set architecture/architecture/ISA/指令集架构

它是一本协议手册. 是硬件和软件之间的接口, 或者说契约. 它包含了程序员写二进制机器语言所需要的全部信息. 



操作系统封装了一些低级的功能, 提供给程序员基本指令系统和操作系统接口, 合称为**应用二进制接口(application binary interface, ABI)**.

ABI也是一本手册. 它规定:
-   **调用约定**：函数参数怎么传（寄存器？栈？），返回值放哪。 
-   **系统调用接口**：比如 Linux 的 `read()`、`write()` 系统调用号。  
-   **二进制格式**：可执行文件用 ELF 还是 PE。
-   **数据布局**：结构体、对齐方式、字节序。

![alt text](image-5.png)



## 1.4 计算机系统的状态机模型



![alt text](image-10.png)


简单回忆一下状态机

![alt text](image-11.png)


**程序是个状态机.**

![???](image-12.png)

![alt text](image-13.png)

事实上, C程序并不是从main()第一条指令开始, return指令结束的.
main函数其实也是有其他函数调用的.

main函数的第一次调用来自OS/运行时环境, 这个就是宿主环境.

之后的函数调用都是程序内部调用.

![alt text](image-14.png)

![alt text](image-15.png)


**CPU是个状态机**.

这是显然的, 数字电路是状态机.

![alt text](image-16.png)

![alt text](image-17.png)



**指令集是个状态机**.


![alt text](image-18.png)


👇下面的指令集状态机是简化版本的指令集. 真正的指令集不但有指令, 还有很多内存管理, I/O, 中断, 异常等等部分, 是状态更复杂, 转移规则更复杂(考虑I/O也作为规则等等)的状态机.

>R即寄存器; 包括PC和(RISC-V)32个通用整数寄存器;
>M即内存.
![alt text](image-19.png)


![alt text](image-20.png)

**程序, 指令, CPU状态机如何联系在一起?**
编译器实现这件事.

* **程序运行状态机**和**指令状态机**的转换: **编译器**
  * $s_{compile}$: 这个映射将描述程序运行的一组状态(即程序计数器和所有程序内变量的值)一一映射为指令集的一组状态(即寄存器+内存单元的值)
  * $e_{compile}$:这个映射将程序状态机的激励时间(函数语句)一一映射为汇编指令.
* **指令状态机**和**CPU状态机**的转换: 这个就是简单的数字电路一一对应咯


![alt text](image-21.png)



程序在CPU上运行的完整步骤:

| 步骤 | 内容 |
|------|------|
| 1 | 根据指令集手册的功能描述，画一张 CPU 的电路图 → **结构设计** |
| 2 | 用 RTL 代码描述 CPU 电路图 → **RTL 设计** |
| 3 | 根据 RTL 代码生成版图文件 → **后端物理设计** |
| 4 | 根据版图文件生产芯片 → **制造生产** |
| 5 | 编写程序 → **软件编程** |
| 6 | 将程序翻译成指令集手册中描述的指令序列 → **编译** |
| 7 | 程序在 CPU 上执行 = 指令序列控制 CPU 芯片电路进行状态转移 |
| 8 | 三个状态机产生联系：Sc ~ Sisa ~ Scpu |

![alt text](image-22.png)


## 1.5 程序的执行和模拟器

### 1.5.0 引言
![alt text](image-9.png)


### 1.5.1 交叉编译器 riscv64-unknown-elf-gcc交叉编译器

#### 1.5.1.0 前言: 本地编译和交叉编译的区别

它是相对于**本地编译**来说的.

##### 1.本地编译:

你在一台 **x86 的电脑**上写了 `a.c`，然后用普通的 `gcc` 编译：
```bash
gcc a.c -o a.out
```
编译器生成的 `a.out` 就是 **能在这台 x86 电脑上直接运行**的程序。  

👉 源代码和目标程序运行的环境（架构、系统）是一样的。

##### 2.  交叉编译(cross compilation)* 
你还是在 **x86 的电脑**上写 `a.c`，但是这次你想要它在 **RISC-V CPU** 上运行。  
那你得用一个“交叉编译器”：
    

```bash
riscv64-unknown-elf-gcc a.c -o a.elf
```
这个编译器不是生成 x86 机器码，而是生成 **RISC-V 的机器码**。  
👉 源代码和目标程序运行的环境 **不一样**。

换句话说：
-   编译发生在 x86 电脑上    
-   运行发生在 RISC-V 芯片（或者 QEMU 模拟的 RISC-V CPU）上
    
这就叫 **交叉编译**。

* 🚩 为什么要交叉编译？

  因为很多时候目标环境太弱（比如嵌入式开发板、微控制器）：
  -   它可能根本跑不动编译器   
  -   或者内存/存储不足  
      所以只能在 PC 上编译好程序，然后“交叉编译”成目标机器能跑的二进制，再烧进去或放进去运行。

#### 1.5.1.1 使用 riscv64-unknown-elf-gcc

***
我们选择交叉编译器`riscv64-unknown-elf-gcc`.
  -   **`riscv64`** → 目标架构是 RISC-V 64-bit（但它也能编译出 32-bit 的代码，加参数就行）    
  -   **`unknown`** → 没有指定具体厂商/平台（通用）    
  -   **`elf`** → 目标文件格式是 ELF（Executable and Linkable Format，Linux/嵌入式常用的可执行文件格式）   
  -   **`gcc`** → 编译器前端，C/C++ 源码编译器


安装它.
```bash
sudo apt install gcc-riscv64-unknown-elf
```

---
---

使用它.

```bash
riscv64-unknown-elf-gcc a.c -o a.elf
```
* 使用默认选项, RV64来编译.



**RV32裸机开发时的编译选项:**
```bash
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -Wl,-Ttext=0x80000000 -O2 a.c -o a.out
```


-   `-march=rv32i` → 目标架构是 RV32I 基础指令集    
-   `-mabi=ilp32` → ABI（应用二进制接口），使用 32-bit 寄存器宽度    
-   `-ffreestanding` → 告诉编译器这是裸机环境，不依赖标准库    
-   `-nostdlib` → 不链接 C 标准库（因为裸机没 libc）    
-   `-Wl,-Ttext=0x80000000` → 链接选项，把程序`_start()`入口点地址设为虚拟地址`0x80000000`。   
-   `-O2` → 优化等级    
-   `-o a.out` → 输出文件名


#### 1.5.1.2 riscv64-linux-gnu工具链





### 1.5.2 架构模拟器 QEMU(Quick EMUlator)

QEMU是一个开源的 **硬件模拟器**。

它能在你的 PC 上虚拟出各种 CPU 架构和硬件（x86、ARM、RISC-V、MIPS…），这样你就能：

-   在没有 RISC-V 硬件的情况下，模拟一台 RISC-V 板子；    
-   把你编译好的 **裸机程序** 或 **操作系统内核** 在 QEMU 里跑起来。
-   你需要写好程序源码(.c文件), 然后使用


在ubuntu上安装它.
```bash
sudo apt update
sudo apt install qemu-system-misc
```

安装完以后，你就有了很多命令，比如：
-   `qemu-system-riscv32` → 模拟 32 位 RISC-V 系统  
-   `qemu-system-riscv64` → 模拟 64 位 RISC-V 系统


注意qemu是一个大框架, 包含很多可执行程序:
qemu-system-<架构>

比如：
-   `qemu-system-x86_64` → 模拟 64 位 x86    
-   `qemu-system-arm` → 模拟 ARM    
-   `qemu-system-riscv32` → 模拟 RISC-V 32 位    
-   `qemu-system-riscv64` → 模拟 RISC-V 64 位

查看其中每个模拟器的版本:

```bash
azazel@DESKTOP-NJKSK6O:~/test$ qemu-system-riscv32 --version
QEMU emulator version 6.2.0 (Debian 1:6.2+dfsg-2ubuntu6.26)
Copyright (c) 2003-2021 Fabrice Bellard and the QEMU Project developers
```


### 1.5.3 程序运行在什麽环境?? 一个例子: 一个裸机(freestanding)环境C程序

程序运行在裸机环境(freestanding)或操作系统环境.

***

**选择qemu-system-riscv32作为freestanding环境,
riscv64-unknown-elf-gcc作为交叉编译器.**

***

0x10000000是qemu-system-riscv32中virt机器模型的串口地址.


***
**实验步骤:**
***
1. 写一个在逻辑环境中运行的_start()函数`a.c`
```c
#include <stdio.h>
void _start(){
    volatile uint8_t *p = (uint8_t *)(uintptr_t)0x10000000;
    *p = 'A';
    while(1);
}
```

2. 使用编译指令:
```bash
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 \
    -ffreestanding -nostdlib -Wl,-Ttext=0x80000000 -O2 \
    a.c -o a.out
```


-   `-march=rv32i` → 目标架构是 RV32I 基础指令集    
-   `-mabi=ilp32` → ABI（应用二进制接口），使用 32-bit 寄存器宽度    
-   `-ffreestanding` → 告诉编译器这是裸机环境，不依赖标准库    
-   `-nostdlib` → 不链接 C 标准库（因为裸机没 libc）    
-   `-Wl,-Ttext=0x80000000` → 链接选项，把程序`_start()`入口点地址设为虚拟地址`0x80000000`。   
-   `-O2` → 优化等级    
-   `-o a.out` → 输出文件名



3. 使用qemu模拟一个riscv32架构来运行它:


```bash
qemu-system-riscv32 -nographic -M virt -bios none -kernel a.out
```

-   **`qemu-system-riscv32`** → 启动 32 位 RISC-V 的 QEMU 模拟器    
-   **`-nographic`** → 不开图形界面，用终端输出（方便调试）    
-   **`-M virt`** → 模拟一台 “virt” 虚拟开发板（常见的通用机型）    
-   **`-bios none`** → 不加载 BIOS（因为你是 `-ffreestanding -nostdlib`，直接跑裸机程序）    
-   **`-kernel a.out`** → 把你刚刚编译好的 ELF 程序加载到虚拟机内存并执行

结果:顺利打印出A然后进入死循环.

![alt text](image-23.png)



***

4. 然后我们来看看这个程序的汇编指令: (使用`riscv64-unknown-elf`交叉编译器工具链(安装见1.5.-1)内提供的objdump命令)

```bash
riscv64-unknown-elf-objdump -d a.out
```

输出:


```bash

a.out:     file format elf32-littleriscv


Disassembly of section .text:

80000000 <_start>:
80000000:       100007b7                lui     a5,0x10000
80000004:       04100713                li      a4,65
80000008:       00e78023                sb      a4,0(a5) # 10000000 <_start-0x70000000>
8000000c:       0000006f                j       8000000c <_start+0xc>
```

*** 

每一行是一条汇编指令.  以`80000000:       100007b7                lui     a5,0x10000
`为例, 
-   **`80000000:`**    
    -   这是这条指令所在的内存地址（PC = Program Counter）。        
    -   意味着 `_start` 函数的第一条指令位于 `0x80000000`。
        
-   **`100007b7`**    
    -   这是这条指令的 **机器码（十六进制编码）**。        
    -   RISC-V 每条指令固定 32 位（4 字节），所以 `0x100007b7` 就是这条 `lui` 的编码形式。        
    -   CPU 实际执行的就是这个机器码。
        
-   **`lui`**    
    -   这是指令助记符（Mnemonic），是人类可读的汇编指令。        
    -   `lui` = Load Upper Immediate（把立即数放到寄存器的高 20 位）。
        
-   **`a5,0x10000`**    
    -   指令的操作数。        
    -   表示：把立即数 `0x10000` 加载到寄存器 `a5` 的高 20 位。        
    -   执行后 `a5 = 0x10000 << 12 = 0x10000000`。


***
***

**三条指令的意思:**

* 1\. `80000000: 100007b7 lui a5,0x10000`
  -   `lui` = Load Upper Immediate    
  -   把 `0x10000` 放到寄存器 `a5` 的高 20 位    
  -   结果：`a5 = 0x10000 << 12 = 0x10000000`  
      （正好是你代码里写的 `0x100000` 地址，只是 C 编译器用 `lui` 搞成 32 位完整地址）
    

* * *

* 2\. `80000004: 04100713 li a4,65`
  -   `li` = Load Immediate（伪指令）    
  -   实际上等价于：`addi a4, x0, 65`   
  -   所以 `a4 = 65 = 0x41 = 'A'`
    

* * *

* 3\. `80000008: 00e78023 sb a4,0(a5)`
  -   `sb` = store byte    
  -   把 `a4` 中的 65 (`'A'`) 存到 `[a5 + 0]` 地址里   
  -   因为 `a5 = 0x10000000`   
  -   实际操作： `*0x10000000 = 'A'`
    

也就是往内存/外设写入一个 ASCII 字母 A。  
这一步就是你代码里 `*p = 'A';`

* * *

* 4\. `8000000c: 0000006f j 8000000c`
  -   `j` = jump，无条件跳转    
  -   直接跳回自己，形成死循环    
  -   对应 C 代码里的 `while(1);`
    

* * *

* 总结
1.  程序从 `0x80000000` 开始运行   
2.  准备了地址 `0x10000000`  
3.  往那个地址写入 `'A'`    
4.  程序停在死循环





### 1.5.4 程序怎麽结束? 一个例子:

阅读C99手册:
***
5.1.2.1 Freestanding environment
2 The effect of program termination in a freestanding environment is implementation-defined(由实现定义的).
***
在裸机环境下, 程序终止并没有一个标准的函数来结束, 需要根据具体硬件提供的方式来退出.


我们同样使用`riscv-unknown-elf-gcc`作为交叉编译器, `QEMU`作为架构模拟器, 进行裸机C开发.

**QEMU virt 机器的退出机制**
- 在 QEMU 的 `virt` 机器模型上，提供了一个**模拟的硬件设备(映射到32bit宽寄存器)**来接收退出信号。
- 这个设备被映射到内存中的一个特殊地址`0x100000`。向这个地址写入一个特定的“魔法数字 (magic number)(`0x5555`)”，QEMU 监视器就会捕获到这个操作，并认为用户请求退出，从而优雅地关闭整个模拟器.
-  它检测其32位寄存器的低16位是否为0x5555(即`if ((written_value & 0xFFFF) == 0x5555)`)






1. 我们写一个`b.c`
```c
#include <stdin.h>

void_start(){
  //向QEMU的串口地址的第一个8字节写一个"A". 注意到该模拟串口是8bit位宽的设备(只支持8bit的ASCII码).
  volatile uint8_t *p = (uint8_t *)(uintptr_t)0x10000000;
  *p = 'A';

  //向虚拟退出设备对应内存写一个0x5555. 该虚拟机是32位的, 
  volatile uint32_t *exit = (uint32_t *)(uintptr_t)0x100000;
  *exit = 0x5555; //对该地址写入魔法数字0x5555

  _start();//正常情况下, 不会执行到这一句. 因为上一条指令已经导致 QEMU 退出了。如果因为某种原因退出失败，这会导致程序无限递归，重新尝试退出。
}

```

2. 然后交叉编译它得到可以在rv32架构上跑的可执行文件:
```bash
riscv64-unknown-elf-gcc -ffreestanding -nostdlib -Wl,-Ttext=0x80000000 -O2 b.c -o b.out
```
* 注意如果没有加freestanding选项, 编辑器就不会自带一个最小的内置头文件集合, 会报错找不到stdint.h.

3. 然后再QEMU上运行它:
```bash
qemu-system-riscv32 -nographic -M virt -bios none -kernel b.out
```


4. 看看反汇编:
```bash
azazel@DESKTOP-NJKSK6O:~/test$ riscv64-unknown-elf-objdump -d b.out

b.out:     file format elf64-littleriscv


Disassembly of section .text:

0000000080000000 <_start>:
    80000000:   100007b7                lui     a5,0x10000
    80000004:   04100713                li      a4,65
    80000008:   00e78023                sb      a4,0(a5) # 10000000 <_start-0x70000000>
    8000000c:   6795                    lui     a5,0x5
    8000000e:   00100737                lui     a4,0x100
    80000012:   55578793                addi    a5,a5,1365 # 5555 <_start-0x7fffaaab>
    80000016:   c31c                    sw      a5,0(a4)
    80000018:   8082                    ret
```

* `0000000080000000 <_start>:` 表示`_start`符号在可执行文件的虚拟地址(VA) `0000000080000000`. 这是因为我们编译时用的命令为`riscv64-unknown-elf-gcc -ffreestanding -nostdlib -Wl,-Ttext=0x80000000 -O2 b.c -o b.out`, 其中`-Wl,-Ttext=0x80000000`即告诉链接器把 .text 段（代码段）放到虚拟地址 0x80000000 开始。
* **第1条指令**: `80000000:   100007b7                lui     a5,0x10000`
  * `80000000:` 表示该指令的内存地址. noted: rescv32模拟下, 内存地址是按byte寻址的, 所以一条指令为32/8=4byte, 下一条指令应该在`80000004`.
  * `100007b7` 该指令的32位机器码内容对它反汇编, 就得到了注记符人类可读形式的指令: `lui a5, 0x10000`. 它是**U-type**指令.
    * 高20位正好是`0x10000`, 对应lui指令的imm[31:12] . 
    * 再低5位是`01111`即`0x15`, 对应寄存器x15(a5).
    * 最低7位是`011 0111`, 对应指令`lui`.
  * 这条指令将寄存器`x15`存入数据`0x10000000`. 这是QEMU的模拟串口基地址.
* **第2条指令**: `li      a4,65`
  * 将`x14`寄存器存入`65`, 即'A'
* **第3条指令**: `sb      a4,0(a5) # 10000000 <_start-0x70000000>`
  * 注释表示: `a5`寄存器内的值, 以及其相对于`_start`地址的偏移量为`-0x70000000`. 
  * 该指令往`a5`寄存器的值地址`10000000`写入了寄存器`a4`的低8位数据, 即写入了'A'(或者说`0x65`)
* **第4条指令**: `lui     a5,0x5`
  * 向`a5`寄存器写入`0x5<<12`, 即写入`0x5000`. 这是退出魔法数字`0x5555`的高位. 一会儿指令6会用addi补全低位.  
  * 覆盖了`a5`刚刚存储的写入数据地址`10000000`, 因为已经用完了.
* **第5条指令**: `lui a4,0x100`
  * 向`a4`写入`0x100 000`. 这是一会让要写入魔法数字的地址.
* **第6条指令**: `addi a5,a5,1365 # 5555`
  * 把`a5`增加`1365(dec)=0x555`, 最终结果是`a5=0x5555`. 注释提示最终结果. 这就构造好了魔法数字.
* **第7条指令**: ` sw a5,0(a4)`
  * 写入魔法数字. 把`a5`的32bit直接写入`a4`地址. 
* **第8条指令**: `ret`
  * 伪指令. 展开为`jalr x0, ra, 0`. 因为裸机开发时程序入口点是`_start`, 它不是别的函数调用的. 此时ra未定义, 这条指令会跳到奇怪的地方. 不过正常情况下第七条指令就结束了, 本指令不会运行.



## 1.6 RISCV架构

它有32个32bit宽的通用寄存器x0~x32, 其中x0永远为0x0, 

    **读取 x0** → 永远得到 `0`  

    **写入 x0** → 指令执行了，但结果会被丢弃，不会改变 x0 的值。

**它们有约定的用途.**

| 名称        | ABI Mnemonic/注记符/别名    | 用途  | preserved across calls?       |
| --------- | -------- | ------------------------------ |-- |
| `x0`      | `zero`   | 永远为 0      |   --(immutable)      |
| `x1`      | `ra`     | return address，函数返回地址|no          |
| `x2`      | `sp`     | stack pointer，栈指针   | yes          |
| `x3`      | `gp`     | global pointer，全局指针    |--(unallocatable)        |
| `x4`      | `tp`     | thread pointer，线程指针  |--(unallocatable)          |
| `x5–x7`   | `t0–t2`  | 临时寄存器（调用者保存） |no                  |
| `x8`      | `s0/fp`  | saved register / frame pointer |yes|
| `x9`      | `s1`     | saved register   |yes      |
| `x10–x17` | `a0–a7`  | 参数/返回值寄存器       |no               |
| `x18–x27` | `s2–s11` | 被调用者保存寄存器   | yes                   |
| `x28–x31` | `t3–t6`  | 临时寄存器           | no               |

preserved across calls? 这一项描述一个函数(caller)调用另一个函数(callee)时, 对该寄存器的操作/

* yes: callee-saved, 被调用者保存. 
  * 如果callee想用这个寄存器, 必须先把它的原先值保存(写到栈内存里维护起来)再覆写. 该callee函数运行结束后, **必须将其恢复原状**, 以供回到caller函数继续运行时使用.
* no: caller-saved, 调用者保存.
  * 调用一个子函数后, callee可能直接覆写这个寄存器, 把里面的内容抹除. caller必须保证不往里面存储调用callee结束后自己可能还要使用的数据, 或者自己做好内容保存后再调用callee.
* x3, x4不可以随便分配覆写, 故不讨论. x0≡0没有讨论意义. 





## 1.7 尝试自己写一个支持RV32的freestanding环境!

QEMU 虽然是个开源项目，但还挺复杂，不利于我们理解细节  
- 25000+ 个源文件，110000+ 行源代码  


### 1.7.1 先设计框架:
让我们来设计一个面向 RISC-V 程序的简单 freestanding 运行时环境！  
- 程序从地址 0 开始执行  
- 只支持两条指令  
  - addi 指令  
  - ebreak 指令  
    - 寄存器 a0 = 0 时，输出寄存器 a1 低 8 位的字符  
    - 寄存器 a0 = 1 时，结束运行  
    - ABI Mnemonic

我们尝试写一个运行在这个环境上的程序.

prog.c
```c

//ebreak()接受两个立即数arg0, arg1, 并把它们分别放进寄存器a0, a1, 然后执行ebreak指令.(注意不是递归执行自己这个函数而是RV的ebreak指令)
static void ebreak(long arg0, long arg1){
  //asm()内联汇编函数. 每一行""内是一条汇编指令. volatile关键字不允许编译器优化掉这断汇编.
  //%0, %1是占位符, 代表后面输入的参数arg0, arg1.
  //汇编代码之后, 第一个`:`后面是输出操作数, 第二个`:`后面是输入操作数.
  //"i"表示立即数约束: 要求该参数是一个编译时常量.
  //"r"表示寄存器约束: 该参数可以放在任意寄存器中.
  asm volatile(
    "addi a0, x0, %0;"
    "addi a1, x0, %1;"
    "ebreak"
    : 
    : "i"(arg0), "i"(arg1)
  );
}


//即令a0=0, a1=ch, 然后执行ebreak指令. 约定当a0=0时表示打印字符.  
static void putch(char ch){ebreak(0, ch);}

//即令a0=1, a1=code, 然后执行ebreak指令. 约定当a0=1时表示程序结束, a1是返回码. 之后进入死循环.
static void halt(char code){ebreak(1, code); while(1);}

void _start(){
  putch('A');
  halt(0);
}

```

这程序显然不能在qemu模拟器上直接运行, 也不能在windows/linux上直接运行.


### 1.7.2 搭配我们的蜜汁Makefile!
```Makefile
# 给riscv'-unknown-elf-gcc写的Makefile. 
# 裸机开发QEMU模拟RISC-V环境下的freestanding程序.

# 用法: make SRC=your_source.c OUT=your_output.out
# Makefile - RISC-V freestanding: 支持编译当前目录下的多个 .c 文件


# 用法举例：
#   make                # 编译所有 *.c -> *.out
#   make SRC=foo.c      # 只编译 foo.c -> foo.out
#   make foo.out        # 直接以目标名编译
#   make clean          # 删除所有 *.out
#   make clean SRC=foo.c    # 删除 foo.out
#   make clean FILE=foo.out # 删除 foo.out
#   make -B SRC=foo.c   # 强制重建
#   make -n SRC=foo.c   # 只显示将执行的命令（dry-run）


CC = riscv64-unknown-elf-gcc

MODE =   # qemu 或 yemu

# 编译选项
CFLAGS = -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -O2
CFLAGS_qemu = -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -O2

# 链接选项
# 裸机开发必须指定程序的起始地址, 0x80000000是QEMU默认的加载地址.
LDFLAGS = -Wl,-Ttext=0x80000000
LDFLAGS_qemu = -Wl,-Ttext=0x80000000
LDFLAGS_yemu = -Wl,-Ttext=0x80000000



# 如果在命令行传入 SRC=xxx.c 则只编译那个文件，否则编译当前目录下所有 .c
SRC ?=
# wildcard *.c 得到当前目录所有.c文件名字, 用空格连接.
C_SOURCES := $(wildcard *.c)

# 如果SRC是空的(输入命令没指定SRC), 则SRCS表示所有.c文件.
# strip()去掉变量值的前后空格.
# :=表示立即替换. =表示用到这个变量的时候才替换.
ifeq ($(strip $(SRC)),)
  SRCS := $(C_SOURCES)
# 如果指定了SRC, SRCS=SRC, 只编译这个文件.
else
  SRCS := $(SRC)
endif

# OUTS表示所有要生成的.out文件. 
# patsubst()是替换函数. 把SRCS字符串中的`.c`替换成`.out`
OUTS := $(patsubst %.c,%.out,$(SRCS))



# 默认目标为all, 依赖为OUTS中的所有.out文件. 
# 比如`make`时, OUTS对应当前目录下所有.c文件的.out文件, 执行all, 生成所有.out.
# 比如`make SRC=foo.c`时, OUTS对应foo.out, 执行all, 生成foo.out.
.PHONY: all 
all: $(OUTS)



# 通用模式规则. %是占位符, 会被替换为make后面的参数. 只用在target:prerequisite这两段中. 这两段用了%的话. 这条规则(即完整的三段式)也叫做`模式规则`. 否则叫做普通规则, 比如`all`和`clean`就是普通规则.
# 比如`make foo.out`时, %.out就是foo.out, %.c就是foo.c(如果foo.c在当前目录存在), $<就是foo.c, $@就是foo.out.  默认编译为qemu可运行的.out文件. 请自行查看编译和连接选项.
# 使用方式: make MODE=你想要的编译模式 SRC=your_source.c
# 或者 make MODE=你想要的编译模式 your_source.c.
# 请注意ifeq后面必须加一个空格. ifeq (...,...)
%.out: %.c
ifeq ($(MODE),)
	@echo  "havent specified compile MODE, default to MODE=qemu."
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@
else ifeq ($(MODE),qemu)
	$(CC) $(CFLAGS_qemu) $(LDFLAGS_qemu) $< -o $@
else ifeq ($(MODE),yemu)
	$(CC) $(CFLAGS_yemu) $(LDFLAGS_yemu) $< -o $@
else 
	$(error "Unknown MODE: $(MODE). Use 'qemu' or 'yemu'.")
endif



# 这是一个普通规则. 用来编译可供QEMU模拟器运行的.out文件. 使用方法是`make gcc_rv32_qemu SRC=your_source.c`
# 已弃用, 请使用`make MODE=qemu SRC=your_source.c`
.PHONY: gcc_rv32_qemu
gcc_rv32_qemu_file.out: $(SRCS)
	$(CC) $(CFLAGS_qemu) $(LDFLAGS_qemu) $< -o $@


# 这是一个普通规则. 用来编译可供我的yemu运行的.out文件. 使用方法是`make gcc_rv32_yemu SRC=your_source.c`
# 已弃用, 请使用`make MODE=yemu SRC=your_source.c`
.PHONY: gcc_rv32_yemu
gcc_rv32_yemu_file.out: $(SRCS)
	$(CC) $(CFLAGS_yemu) $(LDFLAGS_yemu) $< -o $@




# clean 规则：
# - 若指定 FILE=xxx 则删除 FILE
# - 否则若指定 SRC=foo.c 则删除 foo.out
# - 否则删除所有 *.out
# 注意: Makefile可以不声明变量直接用. 比如这里的FILE, 如果用户没指定, $(FILE)就是空字符串.
.PHONY: clean
clean:
#如果 FILE 变量非空, 则删除 FILE 指定的文件.
ifneq ($(strip $(FILE)),)
	rm -f $(FILE)
else
# 如果SRC变量非空, 则删除对应的.out文件. 对应命令`make clean SRC=foo.c`
ifneq ($(strip $(SRC)),)
	rm -f $(patsubst %.c,%.out,$(SRCS))
# 如果只运行`make clean`没有指定SRC或FILE, 则删除所有.out
else
	rm -f *.out
endif
endif




# 反汇编规则.
# 使用方法: make dump BIN=yourfile.out 或者 make dump BIN=yourfile.out ALIASES=no(不使用别名, 展示原生指令)
BIN ?=
# 默认不使用-M no-aliases选项.
ALIASES ?= yes
.PHONY: dump
dump:
# 如果没有指定 BIN 变量, 则提示用户如何使用.
ifeq ($(strip $(BIN)),)
	@echo "请使用: make dump BIN=yourfile.out"
else
ifeq ($(ALIASES),no)
	riscv64-unknown-elf-objdump -M no-aliases -d $(BIN)
else
	riscv64-unknown-elf-objdump -d $(BIN)
endif
endif


#QEMU规则. 用法: make qemu BIN=yourfile.out
.PHONY: qemu
qemu:  qemu-system-riscv32 -nographic -M virt -bios none -kernel $(BIN)



.PHONY: help
help:
	@echo "Makefile usage:"
	@echo "  make                 # build all .c -> .out"
	@echo "  make SRC=foo.c       # build only foo.c -> foo.out"
	@echo "  make foo.out         # build by target"
	@echo "  make clean           # remove all .out"
	@echo "  make clean SRC=foo.c # remove foo.out"
	@echo "  make clean FILE=foo.out # remove foo.out"
	@echo "  make -B SRC=foo.c    # force rebuild"
	@echo "  make -n SRC=foo.c    # dry-run (show commands)"



```

### 1.7.3 两条指令的程序实现

![alt text](image-25.png)



指令循环函数实现: 
```c
void inst_cycle() {
    uint32_t inst = *(uint32_t *)&M[PC];  //取出一条32bit指令
    if (
      ((inst & 0x7f) == 0x13) && ((inst >> 12) & 0x7) == 0  // 判断是addi
      ) { 
      //第二操作数是否为x0, 是的话忽略写入x0.
        if (((inst >> 7) & 0x1f) != 0) {
            R[(inst >> 7) & 0x1f] = R[(inst >> 15) & 0x1f] + 
                (((inst >> 20) & 0x7ff) - ((inst & 0x80000000) ? 4096 : 0));
        }
    } else if (inst == 0x00100073) { // 判断是ebreak
        if (R[10] == 0) {
            putchar(R[11] & 0xff);
        } else if (R[10] == 1) {
            halt = true;
        } else {
            printf("Unsupported ebreak command\n");
        }
    } else {
        printf("Unsupported instruction\n");
    }
    pc += 4;
}
```

初始状态:
![alt text](image-24.png)


### 1.7.4 YEMU V1.0!!


内存M中存储的指令:

![alt text](image-26.png)

```c
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//PC寄存器.
uint32_t R[32], PC;

//我们分配出64byte的内存. 是的, 这非常小.
//它存放的数据就是那个最简单的_start()函数的汇编指令, 一共7条. 它输出一个A然后结束.
uint8_t M[64] = {
    0x13, 0x05, 0x00, 0x00, 0x93, 0x05, 0x10, 0x04, 0x73, 0x00, 0x10, 0x00,
    0x13, 0x05, 0x10, 0x00, 0x93, 0x05, 0x00, 0x00, 0x73, 0x00, 0x10, 0x00,
    0x67, 0x00, 0x00, 0x00
};

bool halt = false;

void inst_cycle() {
    uint32_t inst = *(uint32_t *)&M[PC];
    if ((inst & 0x7f) == 0x13 && ((inst >> 12) & 0x7) == 0) { // addi
        if (((inst >> 7) & 0x1f) != 0) {
            R[(inst >> 7) & 0x1f] = R[(inst >> 15) & 0x1f] + 
                (((inst >> 20) & 0x7ff) - ((inst & 0x80000000) ? 4096 : 0));
        }
    } else if (inst == 0x00100073) { // ebreak
        if (R[10] == 0) { 
            putchar(R[11] & 0xff); 
        } else if (R[10] == 1) { 
            halt = true; 
        } else { 
            printf("Unsupported ebreak command\n"); 
        }
    } else { 
        printf("Unsupported instruction\n"); 
    }
    PC += 4;
}

int main() {
    PC = 0; 
    R[0] = 0; // can be omitted since uninitialized global variables are initialized with 0 in C.
    while (!halt) { 
        inst_cycle(); //指令循环
    }
    return 0;
}


```

## 1.9 ARM架构

### 1.9.0 概述


#### 1.9.0.1 arm架构的寄存器: 15个通用寄存器 + 1个PC + CSPR + SPSR

ARM A32有16个通用寄存器(R0~R15). 不同于riscv32, 现在寄存器只需要4bit来寻址了.


| 寄存器  | 用途 / 特性  | 备注   |calling convention函数调用约定|
| ------- | -------- | ------ |--|
| **R0–R3**    | 通用寄存器 | 用于函数参数传递、临时运算，可自由使用  | caller-saved |
| **R4–R11**   | 通用寄存器 / 保存寄存器  | 一般在函数调用中用于保存局部变量，调用者负责保存  | callee-saved|
| **R12 (IP)** | Intra-Procedure-call scratch register | 用作函数调用中间寄存器（临时），可用于跳转地址计算  |caller-saved|
| **R13 (SP)** | Stack Pointer（栈指针）  | 指向当前堆栈顶，用于 push/pop、堆栈操作     |--|
| **R14 (LR)** | Link Register（返回地址寄存器）   | 函数调用返回地址保存寄存器，执行 `BL` 指令会自动写入   |--|
| **R15 (PC)** | Program Counter（程序计数器）  | 指向下一条要执行的指令，读取写入时要注意流水线偏移 (+8)  |--|
| **CPSR**  | Current Program Status Register   | 包含 NZCV 条件标志位、模式位、中断屏蔽等，不是通用寄存器. |--|
| **SPSR**     | Saved Program Status Register  | 保存模式切换前的 CPSR，用于异常/中断返回，只有特权模式有. |--|

* callee saved: 被调用的函数必须负责在返回前恢复原状.
* caller saved: 父函数必须负责里面东西自己保存好.(被调用函数可以随便改)


#### 1.9.0.2  ① ARM 汇编的大小写敏感性
-   **ARM 汇编指令（Mnemonic）本身是大小写不敏感的**。   
    -   `ADD r0, r1, r2` ✅        
    -   `add r0, r1, r2` ✅        
    -   `AdD r0, r1, r2` ✅        
-   汇编器（比如 GNU as，或者 ARM 自家的 armasm）都会接受大小写混用。    
-   但 **寄存器名** 也一样大小写不敏感：`R0` 和 `r0` 都行。    
-   **符号/标签** 则依赖汇编器：GNU as 默认是大小写敏感的（`Label` ≠ `label`）。


#### 1.9.0.3 条件执行码{cond}

虽然不同类型的arm指令结构不同, **但每条arm指令都分配了4bit的`{cond}`域.** 可以在指令名之后加上如下的条件执行码字母, 手动设置`{cond}`域, 从而实现条件执行该指令. 

当运行到该指令时, 查看CPSR寄存器的NZCV四个位, 从而判断是否执行该指令.

如果没有设置, 默认{cond}=AL=`1110`.


| 条件码     | 二进制编码  | 含义    | 触发条件                     |
| ------- | ------ | ---------- | ------ |
| `EQ`    | `0000` | equal  | Z=1     |
| `NE`    | `0001` | not equal   | Z=0      |
| `CS/HS` | `0010` | carry set / higher or same | C=1   |
| `CC/LO` | `0011` | carry clear / lower        | C=0|
| `MI`    | `0100` | negative                   | N=1 |
| `PL`    | `0101` | positive (plus)            | N=0                      |
| `VS`    | `0110` | overflow set               | V=1                      |
| `VC`    | `0111` | overflow clear             | V=0                      |
| `HI`    | `1000` | higher (unsigned)          | C=1, Z=0                 |
| `LS`    | `1001` | lower or same (unsigned)   | C=0 or Z=1               |
| `GE`    | `1010` | greater or equal (signed)  | N=V                      |
| `LT`    | `1011` | less than (signed)         | N≠V                      |
| `GT`    | `1100` | greater than (signed)      | Z=0 and N=V              |
| `LE`    | `1101` | less or equal (signed)     | Z=1 or N≠V               |
| `AL`    | `1110` | always                     | 无条件（默认）                  |
| （保留）    | `1111` | 通常未定义/保留                   | ARMv7 里 `NV`（never），后来弃用 |


示例:
```
TST     r0, #1      ; 无条件，测试最低位是否为1
TSTEQ   r0, #1      ; 只有 Z=1 时才测试（即前一条指令结果为0时）
TSTNE   r0, #1      ; 只有 Z=0 时才测试
```

#### 1.9.0.4 CPSR寄存器 NZCV标志位 {S}后缀

* NZCV是 **CPSR（Current Program Status Register）** 里的四个条件标志位.

* CPSR在**带{S}后缀的指令**或者**比较/测试类指令(CMP, TST, CMN, TEQ, ...)**运行后更新结果. (后者这几个指令可以认为永远隐式包含{S}后缀.)

| 标志     | 含义    | 什么时候置位？  |
| ---- | -------- | ------------------------------ |
| **N** (Negative) | 结果是否为负数   | 如果结果最高位=1   |
| **Z** (Zero)     | 结果是否为零            | 如果结果=0                         |
| **C** (Carry)    | 无借位(减法), 无进位(加法)| 如果 `Rn >= Operand2`，C=1；否则 C=0 |
| **V** (Overflow) | 有符号数运算结果溢出 | **加法溢出**: 最高位操作数同号结果异号. **减法溢出**: 最高位操作数异号，结果符号和被减数不同。|


### 1.9.1 数据处理 指令


-   \[31:28\] 条件码 (cond)    
-   \[27:26\] `00` → 表示数据处理类    
-   \[25\] I → `second operand`是否使用立即数.` I=1`->是立即数. `I=0`->是寄存器.    
-   \[24:21\] opcode → 运算类型（ADD/SUB/AND/ORR/MOV/CMP 等）    
-   \[20\] S → 是否更新标志位   
-   \[19:16\] Rn → 第一个操作数寄存器   
-   \[15:12\] Rd → 结果寄存器   
-   \[11:0\] Operand2 → 第二个操作数（寄存器或立即数）

| 指令名   | 指令语法  | 作用 | 主要字段（编码概要） | 常见示例 | 说明 / 注意  |  
|--|--|---| -- |-- |--- |
| **ADD**| `ADD{cond}{S} Rd, Rn, Operand2` | `Rd = Rn + Operand2` | data-processing（如上） `opcode=0100`  | `ADD r0, r1, r2` ; `ADD r0, r0, #1` | `S=1` 时更新 NZCV；Operand2 可为移位寄存器或立即数（imm8+rotate） | 
| **ADC** (Add with Carry, 带进位加)| `ADC{cond}{S} Rd, Rn, Operand2` |              `Rd = Rn + Operand2 + C` | data-processing `opcode=0101`                 | `ADC r2, r3, r4`                    | 用于多字长加法（结合 C 标志）                                  |
| **SUB**（Subtract，减法）              | `SUB{cond}{S} Rd, Rn, Operand2` |                  `Rd = Rn - Operand2` | data-processing `opcode=0010`                 | `SUB r0, r1, #4`                    | `S=1` 时影响 NZCV                                   |
| **SBC**（Subtract with Carry，带借位减） | `SBC{cond}{S} Rd, Rn, Operand2` |        `Rd = Rn - Operand2 - (1 - C)` | data-processing `opcode=0110`                 | `SBC r0, r1, r2`                    | 用于多字长减法                                          |
| **RSB**（Reverse Subtract，反向减）     | `RSB{cond}{S} Rd, Rn, Operand2` |                  `Rd = Operand2 - Rn` | data-processing `opcode=0011`                 | `RSB r0, r1, #0`（可做取反）              | 常用于快速求 `-Rn`（例如 `RSB r0, r0, #0` 得到 `-r0`）       |
| **RSC**（Reverse SBC，反向带借位减）       | `RSC{cond}{S} Rd, Rn, Operand2` |        `Rd = Operand2 - Rn - (1 - C)` | data-processing `opcode=0111`                 | `RSC r0, r1, r2`                    | 较少用，配合 C 标志做多字长运算                                 |
| **AND**（按位与）                      | `AND{cond}{S} Rd, Rn, Operand2` |                  `Rd = Rn & Operand2` | data-processing `opcode=0000`                 | `AND r0, r1, r2`                    | `S=1` 可用于测试位（但 TST 更常用）                           |
| **ORR**（Logical OR，按位或）           | `ORR{cond}{S} Rd, Rn, Operand2` |                             \`Rd = Rn | Operand2\`                                    | data-processing `opcode=1100`       | `ORR r0, r1, #0xFF`                               |
| **EOR**（Exclusive OR，按位异或）        | `EOR{cond}{S} Rd, Rn, Operand2` |                  `Rd = Rn ^ Operand2` | data-processing `opcode=0001`                 | `EOR r0, r1, r2`                    | 常用于翻转掩码位                                          |
| **BIC**（Bit Clear，位清除）            | `BIC{cond}{S} Rd, Rn, Operand2` |                 `Rd = Rn & ~Operand2` | data-processing `opcode=1110`?（注：实际opcode固定）  | `BIC r0, r1, #0x1`                  | 用于清除位，等价 `AND Rn, ~Op2`                           |
| **MOV**（Move，寄存器/立即数传送）           | `MOV{cond}{S} Rd, Operand2`     |             `Rd = Operand2`（忽略 Rn 字段） | data-processing `opcode=1101`，Rn 通常不使用        | `MOV r0, r1` ; `MOV r0, #0xFF`      | `S=1` 时会更新标志（注意影响 CPSR）                           |
| **MVN**（Move Not，取反并传送）           | `MVN{cond}{S} Rd, Operand2`     |               `Rd = ~Operand2`（忽略 Rn） | data-processing `opcode=1111`                 | `MVN r0, r1`                        | 等同 `Rd = NOT Op2`                                 |
| **CMP**（Compare，比大小）              | `CMP{cond} Rn, Operand2`        | 计算 `Rn - Operand2`，**只更新 NZCV，不写 Rd** | data-processing `opcode=1010`，S 位隐含为 1（不写 Rd） | `CMP r0, #0`                        | 用于条件分支判断（BEQ/BNE/…）                               |
| **CMN**（Compare Negative，加对比）     | `CMN{cond} Rn, Operand2`        |           计算 `Rn + Operand2`，只更新 NZCV | data-processing `opcode=1011`                 | `CMN r0, #1`                        | 常用于检测溢出/相加后的标志                                    |
| **TST**（Test，按位与测试）               | `TST{cond} Rn, Operand2`        |           计算 `Rn & Operand2`，只更新 NZCV | data-processing `opcode=1000`                 | `TST r0, #1`                        | 用于测试某些位是否为 1                                      |
| **TEQ**（Test Equivalence，按位异或测试）  | `TEQ{cond} Rn, Operand2`        |           计算 `Rn ^ Operand2`，只更新 NZCV | data-processing `opcode=1001`                 | `TEQ r0, #0xFF`                     | 用于测试相等性的位层面差异                                     |



#### SWP

```
`SWP{B}{cond} Rd, Rm, [Rn]`
```
-   `SWP`：交换 word (32-bit)   
-   `SWPB`：交换 byte (8-bit)   
-   `Rd` ← 从内存 `[Rn]` 读出的数据   
-   `[Rn]` ← 写入 `Rm` 的数据   
-   `cond`：条件执行码

**SWP实现信号量操作**

-   `SWP` 常用于 **多处理器 / 中断环境下的原子操作**（比如实现自旋锁）。  
-   在 ARMv6 之后，`SWP/SWPB` 被 **弃用 (deprecated)**，推荐使用新的 **LDREX / STREX** 指令对来实现原子交换。


### 1.9.2 单数据传输 指令(load/store)

-   \[31:28\] cond   
-   \[27:26\] `01` → 表示 load/store   
-   \[25\] I → 偏移是立即数还是寄存器   
-   \[24\] P → 前/后变址    
-   \[23\] U → 加/减偏移   
-   \[22\] B → 是否字节（1=byte, 0=word）   
-   \[21\] W → 是否写回基址    
-   \[20\] L → 1=LDR, 0=STR    
-   \[19:16\] Rn → 基址寄存器    
-   \[15:12\] Rd → 数据寄存器    
-   \[11:0\] offset → 偏移量
   


| 指令名（缩写/中文）                                     | 指令                      |                         作用 | 主要字段（编码概要）                                                                                                           | 常见寻址语法（示例）                                             | 说明 / 注意                                           |
| ---------------------------------------------- | ----------------------- | -------------------------: | -------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------ | ------------------------------------------------- |
| **LDR**（Load Register，加载寄存器）                   | `LDR Rd, [Rn, #offset]` |   从内存加载一个 32-bit word 到 Rd | `cond(31:28)` `01(27:26)` `I(25)` `P(24)` `U(23)` `B(22)` `W(21)` `L(20)=1` `Rn(19:16)` `Rd(15:12)` `offset12(11:0)` | `LDR r0, [r1]` ; `LDR r0, [r1,#8]` ; `LDR r0, [r1,r2]` | `P/U/W` 控制前/后/写回；`I=0` 表示寄存器偏移（移位可用），`I=1` 表示立即偏移 |
| **STR**（Store Register，存寄存器）                   | `STR Rd, [Rn, #offset]` |           将 Rd 的 word 存到内存 | 同上，`L(20)=0`                                                                                                         | `STR r2, [r3,#4]` ; `STR r4, [r5],#8`                  | `STR r, [rn,#off]!` 表示前变址并写回（Rn 更新）               |
| **LDRB / STRB**（Load/Store Byte，字节）            | `LDRB/STRB`             |            读/写 8-bit（byte） | 与 `LDR/STR` 格式相同，`B(22)=1`                                                                                           | `LDRB r0, [r1,#3]`                                     | 只传/存 1 字节                                         |
| **LDRH / STRH**（Load/Store Halfword，半字）        | `LDRH/STRH`             |       读/写 16-bit（halfword） | 半字/字节也有“特定编码变体”（halfword/byte-transfer 格式），语法类似                                                                      | `LDRH r0, [r1,#2]` ; `STRH r1, [r0,#2]!`               | 半字访问建议对齐到偶地址；`LDRSH/LDRSB` 会符号扩展到 32-bit          |
| **LDRSB / LDRSH**（Load Signed Byte/Half，带符号加载） | `LDRSB/LDRSH`           | 加载带符号的 byte/halfword 并符号扩展 | 类似 `LDRH` 的特殊格式                                                                                                      | `LDRSB r0, [r1,#5]`                                    | 将 8/16 位符号扩展为 32 位放到 Rd                           |



### 1.9.3 块数据传输 指令(LDM(load motiple)/STM)

| 指令名（缩写/中文）    | 指令   |作用 | 主要字段（编码概要）| 语法（示例）       | 说明 / 注意  |
| ---------- | ------- | ------: | --------- | ------- | ---- |
| **LDM**（Load Multiple，多寄存器加载） | `LDM{cond} Rn{!}, <register_list>` |  从内存一次加载多个寄存器（load multiple） | `cond(31:28)` `100(27:25)` `P(24)` `U(23)` `S(22)` `W(21)` `L(20)=1` `Rn(19:16)` `register_list(15:0)` | `LDM r13!, {r0-r3, r12, pc}`            | `register_list` 是 16-bit 掩码 (bit0->R0 ... bit15->R15)；`W`=1 表示写回（常用于弹栈）                    |
| **STM**（Store Multiple，多寄存器存） | `STM{cond} Rn{!}, <register_list>` | 把一组寄存器一次写回内存（store multiple） | 与 `LDM` 类似，`L(20)=0`                                                        | `STM r13!, {r4-r7, lr}`     | 常用于函数前保存寄存器（入栈）      |
| **寻址顺序 IA/IB/DA/DB**（寻址模式说明）  | 跟在LDM, STM后排列组合  |                 决定传输的地址计算和顺序 | 映射：IA (P=0,U=1), IB (P=1,U=1), DA (P=0,U=0), DB (P=1,U=0)                                              | `LDMIA r0!, {...}` 等价于 `LDM r0!, {...}` | IA = Increment After；IB = Increment Before；DA/DB = Decrement variants。影响寄存器传输的先后顺序（关键于栈操作） |
| **PUSH / POP**（伪指令，栈操作）       | `PUSH {regs}` / `POP {regs}`       |            汇编伪指令：把寄存器推入/从栈弹出 | `PUSH {regs}` -> `STMDB sp!, {regs}` ; `POP {regs}` -> `LDMIA sp!, {regs}`                             | `PUSH {r4-r7,lr}` ; `POP {r4-r7,pc}`    | `POP` 中包含 `pc` 时会把 `pc` 载入并跳转（作为返回）；DB/IA 组合常用于约定的栈生长方向   |

* **寻址顺序 IA/IB/DA/DB**
  * 如果不指定寻址顺序 IA/IB/DA/DB, 直接使用LDM, STM, 默认都是先写入再向上滑动指针(**IA**):
    * LDM==LDMIA
    * STM==STMIA

  ***
  ***

  * IA/IB/DA/DB的区别:

    ![alt text](image-27.png)

    A: 先写入再滑动指针.
    B: 先滑动指针再写入.

  * 寻址顺序的**旧写法**: **FA, FD, EA, ED**
    * F(full)对应A(after), 意为让内存块存满数据再移动地址, 即操作完后, 基地址会指向最后一个...
    * E(empty)对应B{before)
    * A(ascending)对应I(increament)
    * D(descending)对应D(decreament)

* `{!}`的作用: 协会基址寄存器Rn. 例如:
  * `STMIA r0, {r1,r2}` 
    * 把`r1`写入内存`mem[r0]`
    * 把`r2`写入内存mem[r0+4].
  * `STMIA r0!, {r1,r2}` 
    * 把`r1`写入内存`mem[r0]`
    * 把`r2`写入内存mem[r0+4]
    * 然后`r0=r0+8`.



### 1.9.4 分支/控制流转移 指令

**控制流转移** 指的就是会改变PC值的指令.

| 指令名 | 指令格式 | 功能说明  | 常见语法 | 主要编码概要   |
| ----- | ---- | ------- | --- | --------- |
| **B(branch)**                        | `B{cond}`      | 跳转到指定 PC 相对偏移                      | `B label` ; `BEQ loop` | `cond(31:28)` + `101` + `offset(23:0)` |
| **BL(Branch with Link)**              | `BL{cond}`     | 跳转并保存返回地址到 `LR`（子程序调用）             | `BL func`              | 同 `B`，但 `L(24)=1`                      |
| **Branch and Exchange**           | `BX Rm`        | 跳转到寄存器 `Rm` 指定的地址，并切换 ARM/Thumb 状态 | `BX lr` ; `BX r3`      | `cond` + `000100101111111111110001 Rm` |
| **Branch with Link and Exchange** | `BLX`          | 子程序调用，跳转到寄存器或立即数地址，同时可能切换 Thumb    | `BLX r3` ; `BLX label` | 两种编码变体（寄存器型/立即数型）                      |
| **Supervisor Call**               | `SVC` (以前 SWI) | 触发软中断，陷入内核/OS 调用                   | `SVC #imm`             | `cond` + `1111` + `imm24`              |
| **MOV PC, Rm**                    | -              | 把寄存器值装进 `PC`，相当于间接跳转               | `MOV pc, r0`           | 数据处理类，目的寄存器是 `PC`                      |
| **LDR PC, \[addr]**               | -              | 从内存加载地址到 `PC`，相当于间接跳转              | `LDR pc, [r0]`         | 单数据传输类，目的寄存器是 `PC`                     |
| **POP {..., PC}**                 | -              | 从栈中弹出 PC，实现子程序返回                   | `POP {r4-r7, pc}`      | 多寄存器加载（LDM/POP 的语法糖）                   |

* 



### 1.9.5 协处理器 指令










### 1.9.6 四种指令寻址

| 寻址方式    | 指令给出的内容 | 实际操作数来源      | 例子                |
| ------- | ------- | ------------ | ----------------- |
| 立即数寻址   | 常数      | 直接使用该常数      | `MOV R0, #5`      |
| 寄存器寻址   | 寄存器编号   | 寄存器中的内容      | `ADD R0, R1, R2`  |
| 直接寻址    | 内存地址    | 指定地址的内存内容    | `LDR R0, =0x2000` |
| 寄存器间接寻址 | 地址寄存器   | `[寄存器]`指向的内存 | `LDR R0, [R1]`    |


#### 1.9.2.1 立即数寻址

- 操作数是立即数. 直接写在指令里，不需要去寄存器或内存再取。  
- 比如`MOV r0, #5      ; 把立即数 5 送到 r0`

#### 1.9.2.2 寄存器寻址

也就是操作数是寄存器~

`ADD r2, r1, r0      ; r2 = r1 + r0`

#### 1.9.2.3 寄存器间接寻址

-   指令的操作数写作`[寄存器]`，这个寄存器存放内存地址. CPU 再去这个地址取真正的操作数。    
-   典型用法就是访问数组、指针。   
-   例如ARM 里 LDR/STR 指令格式经常使用间接寻址:  
```assemble
LDR R0, [R1]     ; R0 = mem[R1] 
STR R2, [R3]     ; mem[R3] = R2
```    
-   还可以带一个**12bit**的立即数偏移offset. 分为**前变址**写法和**后变址**写法. 
```asm
LDR R0, [R1]        ; R0 = mem[R1+4]. R1不变. 无变址, 
LDR R0, [R1, #4]    ; R0 = mem[R1+4], R1不变. 前变址. 
LDR R0, [R1, #4]!   ; R0 = mem[R1+4], 然后R1 = R1+4. 前变址+写回.
LDR R0, [R1], #4    ; R0 = mem[R1], 然后R1 = R1+4. 后变址.
```

* `!` **写回标志**表示：**同时把计算后的地址写回基地址 Rn（自动更新基地址）**。 必须用在`[]`后面. 


* 寄存器间接寻址的**自动寻址**

指的是一些间接寻址命令下, ARM帮你改变基地址寄存器的情况, 而少写一条`ADD rn, rn, offset`指令.

  * 通过自己写附带偏移实现自动寻址: `LDR R1, [R0], #4` 该命令做:
    * R1 ← MEM[0x1000]
    * R0 ← 0x1000 + 4
    * 不需要再写一条`ADD R0, R0, #4`了, 称为自动寻址.
  * 使用STM/LDM命令时ARM自动进行寻址, 帮你一个一个填好.



#### 1.9.2.4 直接寻址(罕见)

`LDR r0, =0x2000     ; 把地址 0x2000 装入 r0`


### 1.9.7 ARM流水线

**📌 ARM 的流水线发展**
-   ARM7：3 级流水线（取指/译码/执行）。  
-   ARM9/ARM10：5 级流水线（增加访存、写回阶段）。   
-   Cortex-A 系列（现代 ARM）：更长（8~14 级甚至更深），支持乱序执行。

和riscv的差异:

-   **RISC-V**：    
    -   PC 自动管理，不暴露给汇编程序员。       
    -   流水线存在，但对汇编代码透明。       
    -   更“干净”的设计。
        
-   **ARM**：    
    -   PC 可以当作普通寄存器显式使用。       
    -   流水线会影响 PC 的值（取到的是地址 + 偏移）。      
    -   程序员要额外小心，特别是在写裸机或手写汇编时。

###

###







# 2. 指令

## 2.0 一些基本概念

冯·诺依曼 1945 年提出：
-   既然 **程序本质上也是一串指令（数字）**，为什么不把它和数据一样放进存储器？
-   CPU 从存储器里读指令，就能决定做什么运算；读数据，就能操作对象。
-   这样一来，换个程序就是换内存内容，不需要改电路。
这就是 **存储程序计算机（Stored-program Computer）**。

**如今所有现代计算机都采用冯诺依曼架构, 是存储程序计算机.**


**字节(byte)**
几乎所有现代计算机架构里（包括 x86 和 RISC-V），1 个字节 = 8 bit。

这是国际标准，基本不会变。

**字(word)**
-   **字(word)** 的含义：和机器架构紧密相关，通常指 **处理器自然操作的数据宽度**。在不同架构里，“word”的大小可能不同：     
    -   在 **RISC-V RV32/64**：一个字 = 32 bit（= 4 字节）           
    -   在早期 **x86（16 位 8086）**：一个字 = 16 bit      
    -   在现在 **x86-64**：虽然是 64 位处理器，但很多文档里仍把 word 定义成 16 bit，为了兼容历史（所以才有 `word`\=16b, `dword`\=32b, `qword`\=64b 的奇怪叫法 😂）。

-   **XLEN** 指的是一个具体 RISC-V 处理器实现中 **原生整数寄存器的位宽 (Native Integer Register Width)**.
    -   risc-v32， XLEN=32
    -   risc-v64， XLEN=64
     






* **操作数(operand)**: 在计算机指令里, 操作数就是指令要处理的数据. 它的物理存在就是寄存器. 
* **立即数**: 指的是指令里写的常数. 
    * 例如下面两条指令中:
      * 第一条指令中, a称为**目标操作数**, b,c称为**源操作数**
      * 第二条指令中, 1,2是**立即数**.
      * 操作数a, b, c是`x0`~`x31`32个**通用整数寄存器**之一. 对RV32架构, 它们是32bit宽的寄存器, 对RV64架构, 它们是64bit宽的寄存器.

```
add a. b. c    //将变量b,c相加, 其和放入a.
sub a. 2. 1    //将常数1-2, 其和放入a.
```

* 每个RISC-V算术指令只执行一个操作, 并且只有三个变量.
* 每行只能包含一条指令.
* 用//注释.
* 当我们说寄存器的时候, 一般默认指的是RISC-V架构下的那**32个通用整数寄存器（x0 - x31）**。而且，**只有它们可以直接作为算术和逻辑运算指令的操作数。**
* 更多的大量数据被存储在内存中. 需要**数据传输指令**把它们放到寄存器去里运算.
* 对于复杂指令集(CISC), 有时允许直接把内存地址A的值和内存地址B的值相加存到内存地址C. 这在RISC-V中不被允许. 所有算数必须先把数据搬到这32个寄存器里.



下面是一些常用数据传输指令:
```
-   `lw x1, (a)` // 载入指令Load word: 将内存地址A处的**字（Word）** 加载到寄存器x1。  
-   `lw x2, (b)` // 载入指令Load: 将内存地址B处的**字（Word）** 加载到寄存器x2。    
-   `add x3, x1, x2` // 运算指令: 在寄存器x1和x2上进行加法运算。  
-   `sw x3, (c)` // 存储指令Store word: 将寄存器x3中的结果**存储**回内存地址C。
```


* RISCV架构下(事实上所有架构特点都如此), 只有32个寄存器 


```
add a. b. c    //将变量b,c相加, 其和放入a.


//若想将四个变量b,c,d,e相加, 放入a中:
add a. b. c     //将b,c相加放入a
add a. a. d     //将a,d相加放入a
add a. a. e     
```




***
例题

![alt text](image-7.png)
![alt text](image-8.png)


## 2.1 解释器是啥?

我们熟悉的C是这样运作的:(省略预处理环节)
C源文件 → (编译器compiler) → 汇编代码 → (汇编器assembler) → 机器码 → (链接器linker) → 可执行文件

但是大多数其他高级语言如python, java, 追求**跨平台**(跨架构(X86, ARM, RISC-V...), 跨操作系统).

它们需要一个**解释器**. (一个软件. 它针对不同**操作系统**和**硬件架构**)而不同.

这些语言的源代码经过它们的**编译器**后, 生成**字节码bytecode**(`.class` 文件). 这个文件不是某个架构的汇编语言. 是一种中间语言, 它被设计为跨平台的东西. 事实上它是一个虚拟的指令集(也写成一条条指令的样子, 虽然它并不能直接被任何架构读取...)

这些字节码文件通过解释器得到可执行程序.

实际操作上, 解释器被集成在**JVM(Java Virtual Machine，Java虚拟机)**中. 可以认为JVM对应C中的`编译为汇编指令,链接`任务.


java程序多种运行方式:
-   **纯解释执行 (Interpreter mode)**   
    -   JVM 直接逐条把字节码解释成机器指令运行。        
    -   优点：启动快，不需要额外编译。        
    -   缺点：运行效率低，因为每条字节码都要被“现场翻译”一次。
        
-   **JIT 编译执行 (Just-In-Time)**
    
    -   JVM 在运行过程中，发现某段代码经常被调用，就会 **动态把那部分字节码编译成本地机器码**。        
    -   这样后续运行时就直接执行机器码，不再解释。        
    -   优点：热点代码性能接近 C 程序。        
    -   缺点：第一次编译会有额外开销，所以启动稍慢。
        
-   **AOT (Ahead-Of-Time) 编译**（新一些的技术，比如 GraalVM 支持）  
    -   在运行前，就直接把 Java 字节码提前编译成本地机器码。        
    -   优点：启动快，性能稳定。        
    -   缺点：失去了一些跨平台和动态优化的灵活性。





## 2.2 常用指令

### 2.2.1 B-type指令 分支跳转

### 2.2.2 UJ-type指令 长跳转



#### jal(jump and link)

`jal ra, offset`   #将PC设置为PC+offset，同时把下一条指令地址(即`PC+4`)写入 ra.

由约定, ra一般选为x1.



#### jalr(jump and link reg)

`JALR ra, offset(rs1)`

跳转到一个存储在寄存器中的地址（加上一个小的偏移量），同时将返回地址保存到 ra 中。



#### J [伪指令]

无条件跳转到一个目标地址(PC=PC+offset)

`j offset` = `jal x0, offset`. 




### 2.2.3 S-type指令 储存

包括
    * sb
    * sh
    * sw
    * sd(RV64才有)

#### sb(store byte)

将寄存器`rs2`内低8bit存储到内存地址`rs1 + offset`内. offset是偏置, 一个立即数. 

格式:
`sb rs2, offset(rs1)`

例如我想往内存地址0x12345678内存2个ASCII字符'A','B':

```asm
//先构造基地址寄存器, 这里选x1
li x1, 0x12345678

//往x2, x3内分别写入这两个字符
li x2, 'A'
li x3, 'B'

//用sb命令存储
sb x2, 0(x1)
sb x3, 1(x1)
```




#### sw(store word) 

和sb指令语法类似.

`sw rd offset(rs)`



### 2.2.4 I-type指令 立即数运算

包括
    * addi
    * adddiw(RV64专有)
#### addi(add immediate)

将rs1加上一个12bit的立即数后存入rd.

格式: 
`addi rd, rs1, imm12`

### 2.2.5 U-type指令 大立即数加载类

包括
    * lui
    * auipc


#### lui(load upper immediate)

将这个 *低12bit补零的32bit立即数* 放到指定寄存器. 

* 配合`addi`补低位, 可以构造任意32bit数.

#### auipc(add upper immediate to PC) 

将这个 *低12bit补零的32bit立即数* **和当前PC(32bit寄存器)值相加**, 再放到指定寄存器.

* 通常它实现得到相对地址, 或和 `addi` 配合，得到一个变量或函数的绝对地址。


#### li(load immediate) [伪指令]

将一个不大于20bit的立即数放到指定寄存器.



如果数值很小<=12bit(比如`li a4, 65`), addi指令就可以处理, 会翻译为:

```
addi a4, x0, 65
```

如果数值在12~32bit(比如`li a4, value`), 会翻译为:
```
lui a4, high20(value)
addi a4, a4, low12(value)
```
这是riscv独有的一种格式, **用来加载大立即数**.
其格式为:

```less
imm[31:12] | rd[11:7] | opcode[6:0]
```

* 可以看到其高12位为`imm[31:12]`, U-type指令将要加载的大立即数即为一个32bit的数:`imm[31:12]+0x000`. 低12位补0.
* 再往下5位是`rd[11:7]`, 表示目标寄存器(对riscv32, 正好是2^5个通用寄存器)
* 最低7位是`opcode[6:0]`, 表示操作码(具体哪一个U-type指令).
  * lui=0110111 
  * auipc=0010111








### 2.2.6 其他

#### ret(return) [伪指令]

它会被展开为`jal`或`jalr`, 把返回地址存进寄存器ra

#### ecall(environment call)

RISCV架构定义的系统控制指令. 触发一个系统调用.


#### ebreak(environment break)

RISCV架构定义的系统控制指令. 触发一个断点异常.


###


###






































## 2.3 常用指令块

### 2.3.1 开辟栈:

栈空间会在函数开头开辟:

RV32中, 约定栈指针(stack pointer, **sp**)为`x2`.

```
addi sp, sp, -16   # 即x2自减0x16, 开辟 16 byte栈
sw ra, 12(sp)      # 保存返回地址
```
这样 [sp]、[sp+4]、[sp+8]… [sp+12] 就成了函数能用的局部空间。

假设原来 sp = `0x80001000`，

执行 addi sp, sp, -16 后, sp = `0x80000FF0`

那么地址 `0x80000FF0` ~ `0x80000FFF` 就是新开的栈帧。

进入函数时:
```
addi sp, sp, -16   # 开16字节栈
sw ra, 12(sp)      # 保存返回地址
sw s0, 8(sp)       # 保存寄存器 s0
```

离开函数时:
```
lw ra, 12(sp)      # 恢复返回地址
lw s0, 8(sp)       # 恢复寄存器 s0
addi sp, sp, 16    # 回收栈
ret
```



## 2.4 编译器指令优化

### 2.4.1 尾优化

例如我的程序最后递归调用自己:
```c
void myfun(){
  //...
  myfun();
}
```

最后一句调用自己, 本来应该处理为:
`jal ra, _start   #跳转到 _start，同时把返回地址写到 ra`

如果编译器发现这是当前函数最后一句, 没有别的操作了. 就会把这条指令优化为j指令`j _start`, 直接跳转, 不保存返回地址.

##











# 5. 大规模集成电路设计 VLSI(very large scale intergration circuit)



## 5.0










### CPSR(current program status reg)寄存器(当前程序状态寄存器)

它定义cpu现在在做什麽.

### SPSR(saved)寄存器(保存的程序状态寄存器)

它是一个影子寄存器.[ ]
R13一般用于栈指针;






















































# 7. 并行计算机体系结构



MPP(大规模并行处理)













# 8. 杂项

## 集成电路的成本

![alt text](image-2.png)

## 计算机的性能

![alt text](image-4.png)
![alt text](image-3.png)
![alt text](image-6.png)

## "栈"的词义演化

* 最基本的含义: 运算受限的线性表, LIFO, 后进先出。

* 函数运行的"栈区": 调用一个函数时, 计算机需要保存:
  -   返回地址（函数结束要跳回哪里）  
  -   局部变量  
  -   参数

    调用函数时, 要存这些数据, 如果这个函数调用期间又调用其他的函数, 还要存进去新函数的这些数据. 当一个函数返回时, 就可以清除它的数据. 这是一个LIFO结构, 所以我们把存储这些数据的区域称为**栈区**.
  -   调用函数 = `push`  
  -   函数返回 = `pop`

* 软件栈: 
这里的栈不再指代LIFO这种结构, 仅仅是一个层层堆叠的比喻. 
    -   在软件系统里，经常会有多层结构叠加：
    -   **应用程序**（App, 比如浏览器） 
    -   **标准库**（比如 C 标准库）  
    -   **操作系统接口**（系统调用）   
    -   **ISA / 硬件接口**（CPU指令集）

    这些层次就像一摞摞积木往上堆，**高层依赖低层**，因此大家习惯称它为**软件栈**.










































































