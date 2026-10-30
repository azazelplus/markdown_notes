<!-- review-toc:start -->
<details>
<summary>复习目录（点击展开）</summary>

- [1 Scala：case class 与配置参数](#1-scalacase-class-与配置参数)
- [2 Chisel：握手接口与组合信号](#2-chisel握手接口与组合信号)
  - [2.1 Decoupled](#21-decoupled)
  - [2.2 WireDefault](#22-wiredefault)
- [3 sbt、Metals、Bloop的关系](#3-sbtmetalsbloop的关系)

</details>
<!-- review-toc:end -->

配套：[流水线与体系结构](../RISC-V与计算机体系结构/RISC-V与计算机体系结构.md)

# 1 Scala：case class 与配置参数

`case class`是`class`的一种. `case`修饰符告诉编译器"要额外生成一些东西".

* `case class`会让编译器自动生成一个伴生对象, 并为其生成 `apply` 方法、`unapply` 方法、`toString` 方法、`equals` 方法和 `hashCode` 方法等.
* `case class`的构造参数会被编译器自动复制为一个成员变量(val), 可以直接访问.
例如下面两种写法都使你可以访问`a`字段:
```scala
class MyClass(val a: Int)   // 你手写了 val → a 是成员
case class MyClass(a: Int)  // 编译器自动加 val → a 是成员
```

```scala
case class CoreConfig(
    addrBits: Int = 32,
    dataBits: Int = 32,
    hasRVM: Boolean = true,
    ...
){
    val wordSize: Int = dataBits / 8                    // 一个数据字包含的字节数，RV32 时为 4
    val regAddrBits: Int = log2Up(physRegs)             // 寄存器编号所需位数，32 个寄存器时为 5
}
```

前面()内是构造参数, 有默认值, 实例化时允许传参覆盖.

后面{}的是类的派生成员,

# 2 Chisel：握手接口与组合信号

## 2.1 Decoupled
默认是“生产者/发送端”视角，也就是output. 但里面有一个反向的 ready。
Decoupled(new ID2EX)
内部等价:
```
val ready = Input(Bool())
val valid = Output(Bool())
val bits  = Output(new ID2EX)
```

## 2.2 WireDefault 
Wire的便捷写法, 声明一根线, 给它一个默认值.

# 3 sbt、Metals、Bloop的关系

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
