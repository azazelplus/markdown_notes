> 本笔记已迁移至：[Scala与Chisel](../YSYX%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/Scala/Scala%E4%B8%8EChisel.md)。以下保留迁移前内容。


# a





# Decoupled
默认是“生产者/发送端”视角，也就是output. 但里面有一个反向的 ready。
Decoupled(new ID2EX)
内部等价:
```
val ready = Input(Bool())
val valid = Output(Bool())
val bits  = Output(new ID2EX)
```

# WireDefault 
Wire的便捷写法, 声明一根线, 给它一个默认值.




# case class传参

`case class`是`class`的一种. `case`修饰符告诉编译器"要额外生成一些东西".

* `case class`会让编译器自动生成一个伴生对象, 并为其生成 `apply` 方法、`unapply` 方法、`toString` 方法、`equals` 方法和 `hashCode` 方法等.
* `case class`的构造参数会被编译器自动复制为一个成员变量(val), 可以直接访问.
例如下面两种写法都使你可以访问`a`字段:
```scala
class MyClass(val a: Int)   // 你手写了 val → a 是成员
case class MyClass(a: Int)  // 编译器自动加 val → a 是成员
```

case class CoreConfig(
    addrBits: Int = 32,
    dataBits: Int = 32,
    hasRVM: Boolean = true,
    ...
){
    val wordSize: Int = dataBits / 8                    // 一个数据字包含的字节数，RV32 时为 4
    val regAddrBits: Int = log2Up(physRegs)             // 寄存器编号所需位数，32 个寄存器时为 5
}

前面()内是构造参数, 有默认值, 实例化时允许传参覆盖.

后面{}的是类的派生成员,


















