











# sbt、Metals、Bloop的关系

`Metals`是语言服务器, vscode直接对话的对象. 它是一个独立于vscode的**常驻JVM本地进程**. vscode的scala扩展管理它的生命周期. 通信协议是LSP(Language Server Protocol语言服务器协议). 

`Bloop`是构建服务器. 它也是一个独立的**常驻JVM本地进程**. 它通过BSP(构建服务器协议). 它可以编译&测试&运行scala代码.



`sbt`是一个构建工具, 可以执行compile等任务. 它也是一个JVM进程. 

终端命令`sbt compile`会启动该进程. 它读取`build.sbt`、`project/`下的构建定义, 将scala源码编译为.class文件.
Metals导入项目时, 会调用`sbt bloopInstall`命令, 该命令会启动sbt进程, 读取构建定义, 并生成bloop的配置文件. 然后Metals会启动bloop进程, 读取这些配置文件, 并通过BSP协议与bloop通信, 来完成编译测试运行等任务. 也就是说**虽然sbt有编译能力, 但是metals默认让bloop负责编译测试运行.** 









所以vscode提供的整个代码解析跳转之类服务, 导入构建时, 本质是这样的:
vscode和metals通信, metals先调用sbt生成项目配置, 然后调用bloop进行编译, 最后得到结果返还给vscode.
```
VS Code 的 Scala 扩展
  ↓ 启动
Metals 进程
  ↓ 发现这是 sbt 项目，调用 sbt + sbt-bloop
sbt 进程
  ↓ 读取 build.sbt
  ↓ 执行 bloopInstall 任务
生成 .bloop/*.json
  ↓ sbt 退出
Metals 连接/启动 Bloop 进程
  ↓ 通过 BSP 请求编译
Bloop
  ↓ 根据 .bloop/*.json 增量编译
  ↓ 产出 .class、SemanticDB、增量分析文件
Metals
  ↓ 读取 SemanticDB，建立符号索引
```













# 程序加载的四个地址

## 链接地址

编译程序时认为指令将来放在那里. 软件构件阶段决定, 放在npc2.mk:
```Makefile
# `--defsym`定义符号. 这里定义: `_pmem_start`和` _entry_offset`共同决定链接地址.
LDFLAGS   += --defsym=_pmem_start=0x80000000 --defsym=_entry_offset=0x0
```
然后`linker.ld`的`SECTION`段开头

```ld
SECTIONS {
  /* _pmem_start and _entry_offset are defined by compiler LDFLAGS */
  . = _pmem_start + _entry_offset;  /* Set output address to memory start + entry offset */```
```





# 链接和编译


# ELF文件结构

## 总览
`ELF`（可执行与可链接格式）文件(`.o`, `.so`, `a.out`, `可执行文件`)长这样:
```
+----------------------+
| ELF header           |  ← 文件头，说明这是 ELF、类型、入口点、程序头表、节头表的位置
+----------------------+
| Program Header Table |  ← >>>程序头表. 每行是一个segment, 描述一个内存区间, 以及对这块内存的装载要求. 它告诉加载器这块内存要不要装载
+----------------------+
| 正文区(section 内容)  |
| ---加载型section---   |  ← 这些section的TYPE=PT_LOAD, 被某个 LOAD 段覆盖.
|                      |  
| .text                | text 代码段：函数体编译后的机器指令，CPU 从这里取指执行。
|                      | C 例子：void f(void) { ... } 的函数体。
|                      | 文件：占文件空间（PROGBITS）。
|                      | 权限：通常 Alloc + Exec，不可写（AX）。
|                      | readelf：看 Section 的 Flg 有 A/X；对应 LOAD 段常为 R E。
+----------------------+
| .rodata              | rodata 只读数据段：程序运行中不应被修改的常量数据。
|                      | C 例子：const int c = 42; 全局 const 数组；
|                      |        字符串字面量 "hello"；switch 跳转表等。
|                      | 文件：占文件空间（PROGBITS）。
|                      | 权限：通常 Alloc，不可写；section 自身通常无 X。
|                      | 注意：它可能和 .text 放进同一个 R E 的 LOAD 段，
|                      |       所以“所在段可执行”不等于“.rodata 可写”。
+----------------------+
| .data                | data 已初始化数据段：初值非零、运行时可读写的全局/静态变量。
|                      | C 例子：int g = 7;  static int s = 3;
|                      |        const char *p = "abc"; 中的指针变量 p。
|                      | 文件：占文件空间，初值保存在 ELF 文件里（PROGBITS）。
|                      | 加载：加载器/启动代码把初值从文件复制到内存。
|                      | 权限：Alloc + Write（WA），通常不可执行。
+----------------------+
| .bss                 | bss 未初始化/零初始化数据段：
|                      | 未初始化或初值为 0 的全局/静态变量。
|                      | C 例子：int g_zero;  static int s_zero;
|                      |        int g_explicit_zero = 0;（通常也进 .bss）
|                      | 文件：不占文件内容（NOBITS）；ELF 只记录“需要多大内存”。
|                      | 加载：运行前必须清零；裸机下通常由启动代码清 .bss。
|                      | 权限：Alloc + Write（WA）。
|                      | readelf -lW：对应 LOAD 段常见 MemSiz > FileSiz，
|                      |              差值部分就是需要清零的 .bss/段尾区域。
+----------------------+
|---非加载型 section--- |     ← 这些section的TYPE≠PT_LOAD. 不属于任何 LOAD 段. 不映射进内存. 有些没有对应程序头
|  .riscv.attributes   |     → 有自己的 LOPROC+0x3 程序头, 但不加载
|  .symtab             |     → 符号表. 
|  .strtab             |     → 字符串表
|  .shstrtab           |     → 节名字符串表
|  .comment            |     → 注释信息
|  .debug_*(可选)       |     → 调试信息
|                      |    其他section...
+----------------------+
| Section Header Table |  ← >>>节头表：每行记录了一个正文的section.
+----------------------+
```

可以用`readelf`命令来查看一个elf文件.
它的常用选项: 
* -l 查看Program Header Table
* -S 查看Section Header Table
* -W 等价于--wide, 用宽格式输出方便查看

判断一个非PT_LOAD的section是否属于一个program header, 原则是:
elf可执行文件运行时, 是否需要在内存中定位这些数据. 
例如:
* PT_PHDR, 程序头表自身.


* 相比ELF文件, 镜像文件(img)则抛弃了elf头, 程序头, 节头表等, 只保留了section内容, 可以直接拷贝到内存相应地址运行.1

## Program Header Table, PHDR, 程序头表

ELF中的Program Header Table部分. 每一行是一个`Program Header`(PH)程序头, 
每个程序头描述一个`segment`(段). 具体的就是一个`文件字节区间`, 以及对这块字节的装载要求. 它告诉加载器这块字节要不要装载进内存, 装载到哪, 多大, 权限是什么.
ELF可执行程序运行时, 加载器读到一个LOAD属性的PHDR, 于是按照它的要求, 把对应的一块字节装载到对应内存地址上去.
不同的操作系统, 这个`装载`或者说`映射`操作实现不同:
* 对有MMU的系统, 比如linux, 运行程序时, 加载器会创建页表. 程序运行到这里才会缺页, 然后拷贝进来.
* 对无MMU的系统, 比如裸机, nemu模拟器, 没有页表可建, 只能老老实实把对应的字节拷贝到对应内存.



看一个具体例子.

输入查看`ELF`文件的`PHDR`的命令:
```
riscv64-unknown-elf-readelf -lW \ 
  am-kernels/tests/cpu-tests/build/fact-riscv32-npc2.elf
```

输出分为三部分:

```text
(文件概览)
Elf file type is EXEC (Executable file) //是可执行文件, 不是.o/.so文件
Entry point 0x80000000          // ELF向加载器要求: 执行时, 映射进内存后, 会跳到0x80000000开始执行第一条指令. 由于这个elf文件是ysyx的npc的裸机测试程序, 不需要linux内核, 所以加载器不是一个操作系统, 而是nemu-interpreter这个C程序. 它负责把这个文件加载到对应内存, 然后跳转到0x80000000, 开始执行exec_once()函数...
There are 3 program headers, starting at offset 52  // 从本elf文件的第52byte开始是程序头表.

(PHDR, 程序头表. 有3个程序头, 分别描述了3个segment. 注意elf中段没有名字.)
Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  LOPROC+0x3     0x0011e3 0x00000000 0x00000000 0x0001a 0x00000 R   0x1   //类型是LOPROC+0x3. 全称是Low Processor加载器遍历程序头时, 看到类型不是LOAD就直接跳过, 不映射, 不加载. 
  LOAD           0x001000 0x80000000 0x80000000 0x00184 0x00184 R E 0x1000
  LOAD           0x001184 0x80000184 0x80000184 0x00034 0x00070 RW  0x1000

(section到segment的映射)
 Section to Segment mapping:
  Segment Sections...
   00     .riscv.attributes 
   01     .text .rodata 
   02     .data .bss
```

先看第一个程序头
```
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  LOPROC+0x3     0x0011e3 0x00000000 0x00000000 0x0001a 0x00000 R   0x1   
```
我们要先搞懂这个程序头(容易猜到, 这是个C结构体)的属性的意思:
* Type: 段类型. 
  * PT_LOAD(PT有些场合省略): 可加载段. 最常见最核心的类型, 要搬运到内存的段.
  * PT_DYNAMIC: 动态段. 里面有动态链接器需要的动态链接信息.如果程序使用了动态链接库, 就需要这个段. 它指向`.dynamic`节. 这里放置了动态链接器工作需要的信息.
  * PT_INTERP: 解释器段. 这里保存了一个字符串, 内容是解释器程序的路径. 静态链接的程序或逻辑不需要.
  * PT_RISCV_ATTRIBUTES: RISC-V特有的段类型. 里面保存了RISC-V架构相关的属性信息, 比如ABI、ISA扩展等. 
    总结下:
    | 特性  | `PT_LOAD` | `PT_DYNAMIC` | `PT_INTERP` | `PT_RISCV_ATTRIBUTES` |
    | --- | --- | --- | --- | --- |
    | **加载器行为** | **加载**到内存 | 读取其指向的节 | 读取路径字符串 | **跳过** |
    | **是否占内存** | 是   | 是（`.dynamic` 节本身） | 是（路径字符串） | **否** (`memsz=0`) |
    | **核心用途** | 装载代码/数据 | 为动态链接器提供信息 | 指定动态链接器路径 | 记录构建元信息 |
    | **适用场景** | 所有程序 | 动态链接的程序 | 动态链接的可执行文件 | RISC-V 架构的程序 |
* Offset: 偏移. 这个段的字节内容从elf文件第几个byte开始
* VirtAddr: 虚拟地址. 程序运行时, 要求加载器把这个段装载到哪个虚拟内存地址.
* PhysAddr: 物理地址. 程序运行时, 要求加载器把这个段装载到哪个物理内存地址.
* FileSiz: 段占多少byte.
* MemSiz: 段在内存中占多少byte. 它可能和FileSize不同: MemSiz > FileSiz时, 意为要在内存中多分配一些清零好的空间. `.bss`段(存有未初始化全局变量)就需要这样做.原因也很简单, 为了节省空间, 所有未初始化全局变量都是0, elf中懒得记录, MemSiz=0, 但是这些全局变量需要在内存中占有一块位置.
* Flg: 权限标志. R=可读, W=可写, E=可执行.
* Align: 对齐要求. 是一个数字, 表示VirtAddr和Offset要按多少byte对齐. 
* 
第一个程序头:
* Type是`LOPROC+0x3`, 它其实就是(数字解析问题)`PT_RISCV_ATTRIBUTES`. 
* Offset=`0x0011e3`, 
* VirtAddr=`0x00000000`, PhysAddr=`0x00000000`. 因为不需要装载到内存, 所以没有内存地址.
* FileSiz=`0x0001a`,  这个段有26byte.
* MemSiz=`0x00000` 不占用内存. 说明这一段不需要被装载到内存, 只是让加载器阅读的.
* Flg=`R`   不加载的段其实没有运行时意义, 权限没有意义.
* Align=`0x1`.  对齐只要求1byte, 等于无对其要求. 因为根本不需要装载.

第二个程序头:
* Type是`LOAD`, 说明这是一个可加载段.
* Offset=`0x001000`, 这个段的字节内容从elf文件第4096byte开始
* VirtAddr=`0x80000000`, PhysAddr=`0x80000000`. 程序运行时, 要求加载器把这个段装载到虚拟内存地址0x80000000开始. 因为没有MMU, 所以虚拟地址=物理地址.
* FileSiz=`0x00184`,  这个段有388byte
* MemSiz=`0x00184` 和FileSiz相同. 说明这个段里没有.bss.
* Flg=`R E` 可读可执行, 不可写.
* Align=`0x1000`. 对齐要求是4KB. 
* 事实上`elf header`, `PHDR table`这两个section在elf文件的第一页(0~0x1000)


## Section Header Table, `SHDR`, 节头表

`ELF`文件中最末尾的部分. 每行描述一个section的信息.


仍然以这个具体例子说明.
输入readelf -SW命令来查看节头表:
```
riscv64-unknown-elf-readelf -SW\
  am-kernels/tests/cpu-tests/build/fact-riscv32-npc2.elf
```

输出:
```
There are 10 section headers, starting at offset 0x1568:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .text             PROGBITS        80000000 001000 000144 00  AX  0   0  4
  [ 2] .rodata           PROGBITS        80000144 001144 000040 00   A  0   0  4
  [ 3] .data             PROGBITS        80000184 001184 000034 00  WA  0   0  4
  [ 4] .bss              NOBITS          800001b8 0011b8 00003c 00  WA  0   0  4
  [ 5] .comment          PROGBITS        00000000 0011b8 00002b 01  MS  0   0  1
  [ 6] .riscv.attributes RISCV_ATTRIBUTES 00000000 0011e3 00001a 00      0   0  1
  [ 7] .symtab           SYMTAB          00000000 001200 000260 10      8  18  4
  [ 8] .strtab           STRTAB          00000000 001460 0000b7 00      0   0  1
  [ 9] .shstrtab         STRTAB          00000000 001517 00004f 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  p (processor specific)
```

其中,
* Type: section种类. 详见下方.
* Addr: elf文件运行时, 这个section会被装载到内存哪个地址. 0则为不加载的节. 只有Flg有`A`的section才会被加载. 
* Off: 在elf文件中的哪个byte.
* Size: 
  * 对type为`PROGBITS`的节, 即为在elf文件中占多少byte. 结合Off, 可以知道节在elf文件中的字节大小. 运行时,在内存中也占有Size个bytes.
  * 对type为`NOBITS`的节, 它在elf文件中占0byte. Size表示它在elf文件运行时占有Size个bytes.
  * 对其他Flg不含`A`的节, 表示在elf文件中占Size bytes. 但是不用装载到内存.
* ES: Entry Size. 每个条目有多大. 只对表类字节有意义.
* Flg: 权限标志. 可以看输出的参考信息Key to Flags
* LK: link. 关联的另一个section的索引.

和`segment`类似, `section`也有TYPE(结构体叫做`sh_type`)

`sh_type` 常见取值
| 值   | 含义  | 内容  |
| --- | --- | --- |
| `NULL` | 空节  | 无内容，占位用. 第 0 个 section 必须是它.|
| `PROGBITS` | 程序数据 | **含有文件数据的的普通Section**. |
| `NOBITS` | 不占文件内容 | 有逻辑大小, 但文件里不存字节. 内存要分配Size个bytes的0填充. 如`.bss` |
|--|下面的这些section的Flg大多是空的, 所以是不重要的静态只读, 不参与内存映射|--|
| `SYMTAB` / `DYNSYM` | 符号表 / 动态符号表 | 符号名 → 地址的表 |
| `STRTAB` | 字符串表 | 一串以 `\\0` 分隔的字符串，给符号表/节名用 |
| `RELA` / `REL` | 重定位表 | 链接/加载时要改哪些地址 |
| `HASH` / `GNU_HASH` | 符号哈希表 | 动态链接时快速查符号 |
| `DYNAMIC` | 动态链接信息 | `.dynamic`，依赖库、重定位表位置等 |
| `NOTE` | 注释  | build-id、ABI tag 等 |
| `INIT_ARRAY` / `FINI_ARRAY` | 初始化/析构函数指针数组 | 程序启动/退出时调用 |
| `PREINIT_ARRAY` | 预初始化函数指针数组 | 在 `INIT_ARRAY` 之前调用 |
| `RISCV_ATTRIBUTES` | RISC-V 属性 | 记录用了哪些 RISC-V 扩展 |
| `GNU_*` | GNU 扩展 | 如 `GNU_verdef`、`GNU_verneed`、`GNU_property` |


* 输入节(input section): 输入链接器的文件(.o目标文件; .a静态库)中的section.
* 输出节(output section): 它们在ld脚本的`SECTIONS{}`中被定义. 比如`.text`, `.data`, `.rodata`, `.bss`. 它们是链接产物elf文件中的一个section.




## 静态符号表 `.symtab`

一个TYPE=SYMTAB的`section`, 里面是符号表. 它记录了所有函数和全局变量的名字、类型、大小、在elf文件中的偏移、在内存中的地址等信息. 链接器和调试器都需要这个符号表来解析符号. 链接器在链接时需要解析符号表来决定函数和全局变量的地址, 调试器在调试时需要解析符号表来显示函数名和变量名.

它是静态文本. 不需要被映射到内存.


仍然以这个具体例子说明.

输入readelf -sW命令来查看节头表:
```
riscv64-unknown-elf-readelf -sW\
  am-kernels/tests/cpu-tests/build/fact-riscv32-npc2.elf
```


输出:

```
Symbol table '.symtab' contains 38 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 80000000     0 SECTION LOCAL  DEFAULT    1 
     2: 80000144     0 SECTION LOCAL  DEFAULT    2 
     3: 80000184     0 SECTION LOCAL  DEFAULT    3 
     4: 800001b8     0 SECTION LOCAL  DEFAULT    4 
     5: 00000000     0 SECTION LOCAL  DEFAULT    5 
     6: 00000000     0 SECTION LOCAL  DEFAULT    6 
     7: 00000000     0 FILE    LOCAL  DEFAULT  ABS start.o
     8: 80000000     0 NOTYPE  LOCAL  DEFAULT    1 $x
     9: 00000000     0 FILE    LOCAL  DEFAULT  ABS fact.c
    10: 80000010     0 NOTYPE  LOCAL  DEFAULT    1 $x
    11: 80000028     0 NOTYPE  LOCAL  DEFAULT    1 $x
    12: 00000000     0 FILE    LOCAL  DEFAULT  ABS trm.c
    13: 800000f4     0 NOTYPE  LOCAL  DEFAULT    1 $x
    14: 80000100     0 NOTYPE  LOCAL  DEFAULT    1 $x
    15: 80000144    64 OBJECT  LOCAL  DEFAULT    2 mainargs
    16: 00000000     0 FILE    LOCAL  DEFAULT  ABS muldi3.o
    17: 80000120     0 NOTYPE  LOCAL  DEFAULT    1 $x
    18: 80000100    32 FUNC    GLOBAL HIDDEN     1 _trm_init
    19: 80009000     0 NOTYPE  GLOBAL DEFAULT    4 _stack_pointer
    20: 800001b8    60 OBJECT  GLOBAL HIDDEN     4 f
    21: 80000144     0 NOTYPE  GLOBAL DEFAULT    1 _etext
    22: 80000000     0 NOTYPE  GLOBAL DEFAULT  ABS _pmem_start
    23: 800001b8     0 NOTYPE  GLOBAL DEFAULT    4 _bss_start
    24: 800001b8     0 NOTYPE  GLOBAL DEFAULT    3 edata
    25: 80009000     0 NOTYPE  GLOBAL DEFAULT    4 _heap_start
    26: 80001000     0 NOTYPE  GLOBAL DEFAULT    4 _stack_top
    27: 80009000     0 NOTYPE  GLOBAL DEFAULT    4 end
    28: 80000010    24 FUNC    GLOBAL HIDDEN     1 check
    29: 80000144     0 NOTYPE  GLOBAL DEFAULT    1 etext
    30: 80000000    16 FUNC    GLOBAL DEFAULT    1 _start
    31: 00000000     0 NOTYPE  GLOBAL DEFAULT  ABS _entry_offset
    32: 80000120    36 FUNC    GLOBAL DEFAULT    1 __mulsi3
    33: 80000028   204 FUNC    GLOBAL HIDDEN     1 main
    34: 800001b8     0 NOTYPE  GLOBAL DEFAULT    3 _data
    35: 80000184    52 OBJECT  GLOBAL HIDDEN     3 ans
    36: 80009000     0 NOTYPE  GLOBAL DEFAULT    4 _end
    37: 800000f4    12 FUNC    GLOBAL HIDDEN     1 halt
```




* 链接器处理`ELF`文件时, 查看`Section Header Table`, 把`.text`, `.data`, `.rodata`, `.bss`这些`section`拼起来, 进行重定位+解析符号, 决定它们在内存中的地址.
* 操作系统处理`ELF`文件时, 查看`PHDR`, 把`.text`, `.data`, `.rodata`, `.bss`这些`segment`加载到内存中, 进行重定位+解析符号, 决定它们在内存中的地址.

有时`section`和`segment`中文都翻译成`段`. 只是分别是`链接器&调试器视角`和`操作系统视角`.
section是链接器和调试器使用的概念，而segment是操作系统使用的概念。


|     | section | segment |
| --- | --- | --- |
| 谁用  | 链接器 `ld`、调试器、`objdump`、你 | 加载器 / bootloader / 模拟器 |
| 表名  | Section Header Table | Program Header Table |
| `readelf` 选项 | `-S` / `-SW` | `-l` / `-lW` |
| 粒度  | 细：`.text`、`.rodata`、`.data`、`.bss`、`.symtab`、`.debug_info`… | 粗：`LOAD`、`PHDR`、`GNU_STACK`… |
| 关心什么 | “这段是什么内容” | “这段要加载到哪、多大、什么权限” |

-   **section** 回答：这是代码？只读数据？可写数据？符号表？调试信息？
-   **segment** 回答：程序启动时，把文件里哪一块搬到内存哪个地址，多大，读/写/执行权限是什么。








## 动态符号表 `.dynsym`






#



#