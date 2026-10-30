






# 指令分类
R I S B U J 六种分类依据的仅是编码格式, 而不是功能. 
比如csr指令属于I type, 没什麽道理...


# rs1 rs2 rd 源寄存器(源操作数) 目标寄存器(目标操作数)
源操作数: 要read的数据, 内容不变. 可以是寄存器(`rs1`, `rs2`)也可以是内存.
目标操作数: 要write的数据, 内容将要被写. 可以是寄存器(`rd`)也可以是内存. 


RISCV的一个核心设计原则: **寄存器字段位置全局固定**

`rs1`被定义为`inst[19:15]`    ← 永远是 rs2 的位置
`rs2`被定义为`inst[24:20]`    ← 永远是 rs1 的位置
`rd`被定义为`inst[11:7]`      ← 永远是 rd 的位置

不需要先判断type, 拿到指令直接切了去访问寄存器, 这是RISCV译码简单的重要原因!



# LOAD指令 L型指令 STORE指令 S型指令

- LOAD指令族: `lb`, `lh`, `lw`, `lbu`, `lhu`. opcode=`0000011`. 属于I-type指令.
- STORE指令族: `sb`, `sh`, `sw`. opcode=`0100011`. 属于S-type指令.

**这是RISCV中仅有两类内存访问指令, 其他指令都只操作寄存器.**

- LOAD指令read内存.
- STORE指令write内存.

* 为什麽有S-type, 没有L-type?
  LOAD指令需要一个源寄存器(读取地址)`rs1`, 一个目标寄存器(读入结果)`rd`, 一个立即数偏移`offset`. 两个寄存器一个立即数, 配置和I型指令一样. 于是被归于I型指令.
  STORE指令只需要两个源寄存器(要写的数据; 要写入内存的地址), 指令本身不写入任何寄存器. 这是独有的指令结构. 于是归为S-type.



两个最常用:

`lw` Load Word, 读取32bit数据.
lw rd, offset(rs1)

`sw` Store Word, 写入32bit数据.
sw rs2, offset(rs1)

# R型指令

比较清爽的纯寄存器指令. 只需要`rs1`, `rs2`(运算用)和`rd`(存计算结果), 不需要imm字段.


# opcode #funct3 # funct7 # alu_op



## opcode

`opcode`是标准 32 位基础指令(R I S B U J)的[6:0]位. 是区分inst的**第一层过滤**. **区分指令为10个大类**.
低两位固定为 `11`, 表示 32 位标准指令.


`opcode`将`RV32I`指令集分为`11`个大类
| opcode | 助记符 | 大类 | 包含指令 |
| --- | --- | --- | --- |
| `0110011` | `OP` | R 型 | `add`, `sub`, `and`, `or`, `xor`, `sll`, `srl`, `sra`, `slt` |
| `0010011` | `OP-IMM` | I 型算术/逻辑 | `addi`, `andi`, `ori`, `xori`, `slti`, `slli`, `srli`, `srai` |
| `0000011` | `LOAD` | LOAD | `lb`, `lh`, `lw`, `lbu`, `lhu` |
| `0100011` | `STORE` | STORE | `sb`, `sh`, `sw` |
| `1100011` | `BRANCH` | B 型 | `beq`, `bne`, `blt`, `bge`, `bltu`, `bgeu` |
| `1101111` | `JAL` | J 型 | `jal` |
| `1100111` | `JALR` | I 型跳转 | `jalr` |
| `0110111` | `LUI` | U 型 | `lui` |
| `0010111` | `AUIPC` | U 型 | `auipc` |
| `1110011` | `SYSTEM` | I 型 SYSTEM | `ecall`, `ebreak`, `csrrw`, `csrrs`  |
| `0001111` | `FENCE` | I 型 FENCE | `fence`, `fence.i` |

在M, A, F/D, V...扩展中, 有新的opcode大类:

| opcode | 助记符 | 大类  | 包含指令 / 说明 |
| --- | --- | --- | --- |
| `0000111` | `LOAD-FP` | 浮点加载（F/D/Q/Zfh） | `flw`, `fld`, `flq`, `flh` |
| `0100111` | `STORE-FP` | 浮点存储（F/D/Q/Zfh） | `fsw`, `fsd`, `fsq`, `fsh` |
| `0101111` | `AMO` | 原子内存操作（A） | `lr.w`, `sc.w`, `amoswap.w`, `amoadd.w`, `amoxor.w`, `amoand.w`, `amoor.w`, `amomin.w`, `amomax.w`, `amominu.w`, `amomaxu.w`；RV64 还有 `.d` 版本 |
| `1000011` | `MADD` | 浮点融合乘加（F/D/Q） | `fmadd.s`, `fmadd.d`, `fmadd.q` |
| `1000111` | `MSUB` | 浮点融合乘减（F/D/Q） | `fmsub.s`, `fmsub.d`, `fmsub.q` |
| `1001011` | `NMSUB` | 浮点融合负乘减（F/D/Q） | `fnmsub.s`, `fnmsub.d`, `fnmsub.q` |
| `1001111` | `NMADD` | 浮点融合负乘加（F/D/Q） | `fnmadd.s`, `fnmadd.d`, `fnmadd.q` |
| `1010011` | `OP-FP` | 浮点运算（F/D/Q） | `fadd`, `fsub`, `fmul`, `fdiv`, `fsqrt`, `fsgnj`, `fmin`, `fmax`, `fcvt`, `fmv`, `fclass`, `feq/flt/fle` 等 |
| `1010111` | `OP-V` | 向量运算（V） | `vsetvli`, `vsetivli`, `vsetvl` 及绝大多数向量算术/逻辑/内存指令 |
| `0011011` | `OP-IMM-32` | RV64 专用 I 型 | `addiw`, `slliw`, `srliw`, `sraiw` |
| `0111011` | `OP-32` | RV64 专用 R 型 | `addw`, `subw`, `sllw`, `srlw`, `sraw`；M 扩展：`mulw`, `divw`, `divuw`, `remw`, `remuw` |
| `0001011` | `custom-0` | 自定义 | 厂商自定义指令空间 |
| `0101011` | `custom-1` | 自定义 | 厂商自定义指令空间 |
| `1011011` | `custom-2` | 自定义 | 厂商自定义 / 长指令编码空间 |
| `1111011` | `custom-3` | 自定义 | 厂商自定义 / 长指令编码空间 |

注意M 扩展指令属于R type, 靠`funct7`=`0000001`区分.


## funct3

`funct3`是标准 32 位基础指令的[14:12]位. 是区分inst的**第二层过滤**. 细分指令.

* riscv设计中, `OP`(R型指令)和`OP-IMM`(I型算术/逻辑指令)的`funct3`编码是刻意一一对齐的.

| funct3 | `OP`对应指令 | `OP-IMM`对应指令 |
| --- | --- | --- |
| `000` | add/sub | addi |
| `001` | sll | slli |
| `010` | slt | slti |
| `011` | sltu | sltiu |
| `100` | xor | xori |
| `101` | srl/sra | srli/srai |
| `110` | or  | ori |
| `111` | and | andi |

所以对这两类指令，`funct3`表示同一种ALU运算,区别只在于操作数来源:
-   `OP` 的第二个操作数来自 `rs2`；
-   `OP-IMM` 的第二个操作数来自 `imm`；
这个区别在 ALU 的输入选择阶段处理. 于是 ALU 不在意具体指令解码, 它只接收传来的两个操作数, 做`alu_op`信号要求的运算, 输出结果. 于是ALU单元完全和ISA解耦.


## funct7

`funct7`是标准 32 位基础指令的[31:25]位. 是区分inst的**第三层过滤**, `R型指令`(包括其M,V扩展)使用. 在I型指令里, 这里被借用为imm字段.  `funct7`预留了相当丰富的空间, **为 R 指令扩展更多运算提供可能**.





# imm_b字段
b型指令中的立即数, 被打散得更碎.
val imm_b = Cat(Fill(19, inst(31)), inst(31), inst(7),
                inst(30, 25), inst(11, 8), 0.U(1.W))


# zimm字段

3个csr的imm版本指令中的5bit立即数. 它的位置是`rs1`字段原本的位置.








# srl 逻辑右移
作用是把数当作无符号数右移.

# sra 算术右移
作用是把数当作有符号数右移.



# lw load word 从内存读数据到寄存器


# sw store word 从寄存器读数据到内存




# M扩展



| 指令  | 含义  | 输出  |
| --- | --- | --- |
| `MUL` | 乘法  | 乘积低 32 位 |
| `MULH` | 有符号 × 有符号 | 乘积高 32 位 |
| `MULHSU` | 有符号 × 无符号 | 乘积高 32 位 |
| `MULHU` | 无符号 × 无符号 | 乘积高 32 位 |
| `DIV` | 有符号除法 | 商   |
| `DIVU` | 无符号除法 | 商   |
| `REM` | 有符号除法 | 余数  |
| `REMU` | 无符号除法 | 余数  |

`整数除法有符号溢出`

32bit除法中, 当有符号除法,且为 -2^31 / -1 时, 发生有符号溢出. 商为 2^31, 但32bit寄存器表示有符号数范围:`-2^31 ~ 2^31-1`.

RISCV要求:
整数除法的除零和溢出, **不产生任何异常exception和陷阱trap**.
`整数除法有符号溢出`: `div`返回原始被除数`dividend`, `rem`返回`0`.
`除零`: `div`返回所有位置`1`. 这在无符号除法中为`最大无符号值`, 有符号除法中为`-1.` `rem`则要求返回原始被除数`dividend`.


# CSR

CSR指令来自`Zicsr`扩展.
常见6条:
| 指令  | 功能  |
| --- | --- |
| `CSRRW` | 原子读写 CSR |
| `CSRRS` | 原子读取并置位 CSR |
| `CSRRC` | 原子读取并清除 CSR |
| `CSRRWI` | 使用 5-bit 立即数读写 CSR |
| `CSRRSI` | 使用 5-bit 立即数置位 CSR |
| `CSRRCI` | 使用 5-bit 立即数清除 CSR |


RISCV32I为CSR预留了12bit编码空间`csr[11:0]`.
`[11:10]`用来编码**读写权限**. 
`[9:8]`用来编码**最低特权级**.
`[7:0]`低8bit用来编码地址. 所以理论上可以有`2^8=256`个CSR寄存器, 但实际上只定义了几十个. 





# 杂项

## word
word是一个历史包袱, 但是RISCV中word明确为32bit, 无论什麽机器和XLEN.
| RISC-V 术语 | 位宽  | 说明  |
| --- | --- | --- |
| byte | 8   | 字节  |
| halfword | 16  | 半字  |
| **word** | **32** | 字，固定 32 位 |
| doubleword | 64  | 双字  |
| quadword | 128 | 四字  |

##



















