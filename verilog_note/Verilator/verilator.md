



# 0. 简介

## 0.1 verilator仿真流程

### step1: `verilate`, 即由.v文件生成cpp文件

第一步称为**verilate**,
verilator将你写的.v文件(老语法或者SystemVerilog)来生成一组cpp文件(一组.c和.h文件. 包括头文件`Vtop.h`, 一个叫做`Vtop`的cpp类.  

如果你使用`--sc`选项, 则使用`SystemC`库(`#include <systemc.h>`), 生成更复杂的cpp代码).

还需要自己编写`main.cpp`文件, 它被称为wrapper. 在main函数里你要`#include "Vtop.h"`, 实例化这个模块`Vtop* top = new Vtop;`, 写一个主循环, 处理波形输出/仿真控制等.


**指令**:

使用命令`verilator -Wall --cc top.v --exe main.cpp --trace`
* `top.v`是你写的v代码, 
* `-Wall`显示所有警告
*  `--c`生成cpp模型(而不是--sc使用systemC库)
*  `--exe main.cpp` 指定main程序文件(即wrapper)
*  `--trace` 支持波形生成

verilator会创建目录`obj_dir/`并生成一堆文件`Vtop.cpp`, `Vtop.h`, `Vtop__Syms.*`, `verilated.*`, `Vtop.mk`(**根据你现在的环境自动生成的Makefile文件, 利用它进行下一步编译**) 等等.



### step2. 编写cpp主函数, 然后编译, 得到可执行仿真程序.

进入目录`obj_dir/`, 用make命令, 调用g++编译.

`make -C obj_dir -f Vtop.mk Vtop`
其中:
* `-C`即cd的意思, 切换到`obj_dir`目录. 它是 `--directory=obj_dir` 的简写。
* `-f`指定Makefile. `Vtop.mk`即step1中verilate生成的Makefile. 
* `Vtop`指定目标名字. 最终生成的可执行文件就叫`Vtop`.

### step3. 运行simulator

`./obj_dir/Vtop`运行可执行文件.

如果在main.cpp中使用了波形相关代码, 会生成`wave.vcd`波形文件.

### step4. 查看波形

可以使用软件gtkwave等.

`gtkwave wave.vcd`



## 0.2 例子 


### 0.2.1 例子1: --biary简单执行

最简单的示例模块甚至不需要手动编译, 如写一个our.v

```verilog
module our;
   initial begin
      $display("Hello World");
      $finish;
   end
endmodule
```

直接运行--biary选项命令(一键编译的教学模式). 带上这个参数, verilator会尝试自己写一个main.cpp来实现完成你的简单模块. 这一版要求你的模块没有参数列表, 是纯粹的**纯行为级**模块.

运行命令:
`verilator --binary -j 0 -Wall our.v`

其中:
* `-j 0`的意思是并行编译参数:0表示自动使用电脑上所有cpu核心.
* `-Wall`显示所有警告.


在生成的obj_dir文件夹中可以看到
```bash
azazel@DESKTOP-NJKSK6O:~/verilator_stuff/our_test/obj_dir$ ls -la
total 504
drwxr-xr-x 2 azazel azazel   4096 Jul 28 20:48 .
drwxr-xr-x 3 azazel azazel   4096 Jul 28 20:48 ..
-rwxr-xr-x 1 azazel azazel 159720 Jul 28 20:48 Vour
-rw-r--r-- 1 azazel azazel   2928 Jul 28 20:48 Vour.cpp
-rw-r--r-- 1 azazel azazel   2642 Jul 28 20:48 Vour.h
-rw-r--r-- 1 azazel azazel   1734 Jul 28 20:48 Vour.mk
-rw-r--r-- 1 azazel azazel  17220 Jul 28 20:48 Vour__ALL.a
-rw-r--r-- 1 azazel azazel    359 Jul 28 20:48 Vour__ALL.cpp
-rw-r--r-- 1 azazel azazel    537 Jul 28 20:48 Vour__ALL.d
-rw-r--r-- 1 azazel azazel  15280 Jul 28 20:48 Vour__ALL.o
-rw-r--r-- 1 azazel azazel    738 Jul 28 20:48 Vour__Syms.cpp
-rw-r--r-- 1 azazel azazel    921 Jul 28 20:48 Vour__Syms.h
-rw-r--r-- 1 azazel azazel    806 Jul 28 20:48 Vour___024root.h
-rw-r--r-- 1 azazel azazel   3186 Jul 28 20:48 Vour___024root__DepSet_h637983f1__0.cpp
-rw-r--r-- 1 azazel azazel   2942 Jul 28 20:48 Vour___024root__DepSet_h637983f1__0__Slow.cpp
-rw-r--r-- 1 azazel azazel    750 Jul 28 20:48 Vour___024root__DepSet_hf7027e39__0.cpp
-rw-r--r-- 1 azazel azazel    614 Jul 28 20:48 Vour___024root__Slow.cpp
-rw-r--r-- 1 azazel azazel    899 Jul 28 20:48 Vour__main.cpp
-rw-r--r-- 1 azazel azazel    487 Jul 28 20:48 Vour__ver.d
-rw-r--r-- 1 azazel azazel   1805 Jul 28 20:48 Vour__verFiles.dat
-rw-r--r-- 1 azazel azazel   1590 Jul 28 20:48 Vour_classes.mk
-rw-r--r-- 1 azazel azazel    677 Jul 28 20:48 verilated.d
-rw-r--r-- 1 azazel azazel 201400 Jul 28 20:48 verilated.o
-rw-r--r-- 1 azazel azazel    410 Jul 28 20:48 verilated_threads.d
-rw-r--r-- 1 azazel azazel  30144 Jul 28 20:48 verilated_threads.o
```

不但自动生成了main函数`Vour__main.cpp`, 还帮你编译好了得到了可执行文件`Vour`.

打开这个`Vour__main.cpp`:

```cpp
//这个是 Verilator 为简化教学或测试时生成的“默认仿真驱动程序”.
// Verilated -*- C++ -*-
// DESCRIPTION: main() calling loop, created with Verilator --main

#include "verilated.h"
#include "Vour.h"

//======================

int main(int argc, char** argv, char**) {
    // Setup context, defaults, and parse command line
    Verilated::debug(0);//调试等级为0(关闭额外调试信息)
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};//创建一个上下文对象, 也就是仿真环境.
    contextp->commandArgs(argc, argv);//解析命令行参数

    // Construct the Verilated model, from Vtop.h generated from Verilating
    const std::unique_ptr<Vour> topp{new Vour{contextp.get()}};//创建verilated模型实例, 类名为Vour, 对象名为topp 

    // Simulate until $finish.
    while (!contextp->gotFinish()) {
        // Evaluate model. 计算一周期.调用 eval() 计算当前电路状态，相当于“执行一个仿真时钟边沿”。
        topp->eval();
        // Advance time 推进1单位仿真时间.
        contextp->timeInc(1);
    }
    //如果没有$finish, 输出提示: 仿真是因为没有事件可触发而自然终止，而不是 $finish
    if (!contextp->gotFinish()) {
        VL_DEBUG_IF(VL_PRINTF("+ Exiting without $finish; no events left\n"););
    }

    // Final model cleanup
    topp->final();
    return 0;
}
```

### 0.2.2 例子2: cpp模型


编写our.v和our.cpp:

```verilog
module our;
	initial begin
		$display("hello again, world");
		$finish;
	end
endmodule
```


```cpp
#include "Vour.h"
#include "verilated.h"

int main(int argc, char** argv){

    VerilatedContext* contextp = new VerilatedContext;//创建环境
    contextp->commandArgs(argc, argv);//传入命令行参数

    Vour* top = new Vour{contextp};//创建顶层模块实例. contextp是构造函数的参数.
    while(!contextp->gotFinish()){
        top->eval();//仿真
        //不需要+1时间步, 因为这是个简单的单次行为级仿真
    }
    delete top;//删除顶层模块实例
    delete contextp;//删除环境
    return 0;    
}
```

然后运行命令:

`verilator --cc --exe --build -j 0 -Wall sim_main.cpp our.v`



### 0.2.3 使用SystemC

* 编写our.v:
```verilog
  module our (clk);
     input clk;  // Clock is required to get initial activation
     always @(posedge clk)
        begin $display("Hello World"); $finish; end
  endmodule
```

* 编写sc_main.cpp:

```cpp
#include "Vour.h"
//#include "verilated.h" //为何不需要这个头文件了?

using namespace std; //使用标准命名空间
using namespace sc_core;//使用SystemC的核心命名空间

//main函数的名字必须是sc_main, 因为这是SystemC的入口点.
int sc_main(int argc, char** argv){

    Verilated::commandArgs(argc, argv);//传入命令行参数
    sc_clock clk{"clk", 10, SC_NS, 0.5, 3, SC_NS, true};//创建名为`clk`的时钟信号, `10`ns周期(SC_NS表示纳秒), 50%占空比, 初始延迟`3`ns, `true`表示正边沿触发.
    Vour* top = new Vour{"top"};//创建一个顶层模块实例`top`, 名称为`top`.使用初始化参数列表:{top}.
    top->clk(clk);//将top的时钟`top->clk`连接到创建的时钟信号`clk`
    while (!Verilated::gotFinish())
    {
        sc_start(1, SC_NS);//SystemC仿真, 每次仿真1NS. 
    }
    delete top;//删除顶层模块实例
    return 0;//返回0表示正常结束

}

```

* 运行`verilator --sc --exe -Wall sc_main.cpp our.v`




### 0.2.4

























