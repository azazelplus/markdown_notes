



# 0. verilator 简介

## 0.1 verilator仿真流程


//编写一个verilog模块并仿真的流程:
//1.进入项目文件夹. 编写.v模块文件top.v
//2.编写.cpp驱动文件sim_main.c_str
//3.运行命令链接它们. verilator --cc --exe --build -j -Wall --trace sim_main.cpp top.v
//--build即自动寻找规则编译. 如果没有加, verilator生成的obj_dir目录中就没有可执行文件Vtop, 你需要自己做饭: 使用Vtop.mk这个Makefile, 运行make `make -j -f(即filename) Vtop.mk Vtop`来做一个Vtop出来.
//-j(即jobs)number表示同时运行的编译任务数。例如，-j4 表示同时运行 4 个编译任务。如果不指定数字，make 会根据系统的 CPU 核心数自动选择并行任务数。)
//--trace是波形分析用的. 这个选项会开启波形分析功能, 比如为模块对象top创建trace成员以供设置等.
//4.进入verilator生成的obj_dir目录, 运行其生成的可执行文件Vtop. 如果没用--build选项, 第四步还会要自己make.





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



### 0.2.3 例子3: 使用SystemC

* 编写our.v:
```verilog
  module our (clk);
     input clk;  // Clock is required to get initial activation
     always @(posedge clk)
        begin $display("Hello World"); $finish; end
  endmodule
```

* 编写sc_main.cpp:

(注意:verilator命令要求main函数名为`sc_main`, 同时生成的makefile里要求wrapper文件名为`sc_main.cpp`.)

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

* 使用verilator生成的makefile来编译. `make -j -C obj_dir -f Vour.mk Vour`


* 尝试运行: `obj_dir/Vour`

### 0.2.4 例子4: Examples in the Distribution

## 0.3 波形生成

波形生成请参考例子`双控开关`.
//关于波形生成: verilator波形生成一般使用VDC(value change dump)格式. 大致步骤:
//1.在`cpp驱动文件`中启用波形跟踪. (引入头文件, 启用波形跟踪, 创建VCD记录对象, 关闭波形文件)
//2.运行仿真(即运行`含有--trace选项的verilator命令得到的可执行文件`)时生成VCD波形文件.(wave.vcd)
//3.用GTKWave查看波形文件.(先apt-get install gtkwave)

//chors:
//FST格式的波形文件大致是VCD格式的1/50, 但它仅能被GTKWave支持. 如果想使用fst格式的波形文件, 只需要使用`VerilatedFstC`代替`VerilatedVcdC`.

### 0.3.1 例子: 双控开关

这个例子不适用systemC, 写一个简单的a^b逻辑, 在wrapper中生成100组随机ab信号来测试. 同时输出一个波形图.

* 先编写`top.v`和`sim_main.cpp`


```verilog
module top(
	input a,
	input b,
	output f
);
	assign f= a^b;
endmodule
```


```cpp
//verilator不提供自动仿真功能, 需要cpp代码驱动, 这就是对每个.v模块文件的sim.main.cpp的作用. 它被称为wrapper.
#include <stdio.h>	
#include <stdlib.h>
#include <assert.h>
#include "Vtop.h"	//verilator自动生成的头文件,运行verilator --cc witch.v后自动生成, 在obj_dir目录下.
#include "verilated.h"	//同上

#include "verilated_vcd_c.h"	//如果需要波形生成, 引入VCD波形头文件.




int main(int argc, char** argv){
	
	//初始化verilator环境.
      VerilatedContext* contextp = new VerilatedContext;	//创建`仿真上下文类`对象 contextp. `VerilatedContext`这个类是verilog提供的仿真环境管理器, 其成员和方法能实现多个管理功能之类的.
      contextp->commandArgs(argc, argv);	//解析命令行参数      
	  contextp->traceEverOn(true);	//启用波形跟踪
	  //设置完环境管理器contextp后, 在其基础上创建模块对象top.
	  Vtop* top = new Vtop{contextp};		//创建Vour类(verilator的顶层模块) top, 绑定它到仿真上下文.  此处模块类名是 V{模块名}，其中 {模块名} 是你 Verilog 顶层模块（module）的名
	  
	  
	//创建VCD波形记录对象tfp(trace file pointer)
	VerilatedVcdC* tfp = new VerilatedVcdC;
	top->trace(tfp, 99);	//指定信号跟踪深度.深度为1则只记录顶层模块的信号变化. 深度更高则记录子模块的变量的变化. 常用默认值99, 意为记录所有有意义信号.
	tfp->open("wave.vcd");	//打开波形文件"wave.vcd",并准备将数据写入.在仿真结束后就可以使用工具(如 GTKWave)查看这个文件, 得到波形.
	
	
	  
	//循环执行仿真. 
	for(int i=0;i<100;++i){		//运行一百个时钟周期, verilator默认将dump的时间步长设为1ps. 随机测试我们的XOR门
		//生成随机输入信号.
		int a = rand() & 1;	//rand()返回一个大整数, 如`00110000000111001`. 和1进行&运算时只取最低位, 由于0&1=0,1&1=1, 而最低位随机, 所以`rand()&1`可以随即给出0或1.
		int b = rand() & 1;
		top->a = a;	//传递给verilog模块
		top->b = b;
		top->eval();	//run sim
		
		tfp->dump(i);	//波形记录对象tfp记录时间步i的信号
		
		printf("a = %d, b = %d, f = %d \n", a, b, top->f);
		assert(top->f == (a^b));	//assertion检查XOR计算是否正确.		`
	}
	//仿真结束清理.
	delete top;
	delete contextp;
	delete tfp;
	
	return 0;
}
```

* 然后使用verilator命令:

```bash
verilator --cc top.v --exe sim_main.cpp --trace -Wall
```
| 参数                   | 说明                                          |
| -------------------- | ------------------------------------------- |
| `--cc`               | 生成 C++ 模型（不生成 SystemC）                      |
| `top.v`              | 你的顶层 Verilog 模块                             |
| `--exe sim_main.cpp` | 使用 `sim_main.cpp` 作为仿真主程序（Verilator 不自带仿真器） |
| `--trace`            | 启用 VCD 波形生成（启用 tfp/wave.vcd）                |
| `-Wall`              | 打开所有警告                                      |

* 然后使用verilator生成好的文件和makefile编译:

```bash
make -C obj_dir -f Vtop.mk Vtop
```


* 运行仿真:


```bash
./obj_dir/Vtop
```

* 显示波形: (如果你安装了`gtkwave`, 这样安装:`apt-get install gtkwave`)

```bash
gtkwave wave.vcd
```

























# 1. SystemC


## 1.0 安装SystemC: 从源码构建安装软件.

首先从官网`https://www.eda.org/downloads/standards/systemc`下载`systemc-2.3.3.tar.gz`到一个空文件夹.`./systemC`


* 解压
```bash
tar -xzf systemc-2.3.3.tar.gz
cd systemc-2.3.3
```


* 编译. 先创建一个objdir, 然后在该文件夹编译. 这个目录是 CMake 构建中常见的“**out-of-source build**”，也就是编译结果不污染源码本身。


```bash
mkdir objdir
cd objdir
../configure --prefix=$HOME/systemc #配置编译参数
make -j$(nproc) #使用所有CPU核心来编译
make install    #将内容复制到`/usr/local/systemc-2.3.3/`.
```

* 添加环境变量. 打开.bashrc
```bash
vim ~/.bashrc
```

* append这些在末端:

```bash
export SYSTEMC_HOME=$HOME/systemc-2.3.3-install
export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-linux64:$LD_LIBRARY_PATH
export CPLUS_INCLUDE_PATH=$SYSTEMC_HOME/include:$CPLUS_INCLUDE_PATH
export SYSTEMC_INCLUDE=$SYSTEMC_HOME/include
export SYSTEMC_LIBDIR=$SYSTEMC_HOME/lib-linux64
```

* 在当前shell激活一下环境变量
```bash
source ./.bashrc
```

* 安装完毕, 删除原来的systemC文件夹.

* 安装了啥?

| 目录/内容                                 | 是否需要保留 | 说明                    |
| ------------------------------------- | ------ | --------------------- |
| `build/`                              | ❌ 不需要  | 编译中间产物，已安装完成可以删       |
| `/usr/local/include/systemc`          | ✅ 需要   | 头文件位置，用于编译 SystemC 程序 |
| `/usr/local/lib/libsystemc.a` 或 `.so` | ✅ 需要   | 库文件，链接用               |













































# NV Board






