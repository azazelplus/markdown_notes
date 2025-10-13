# 9 STM32CubeMX


## 9.0 简单入门示例

我们使用型号:野火指南者. 它使用的MCU是
`stm32f103vet6`



| 类型       | 位置说明                        | 例子                                             |
| -------- | --------------------------- | ---------------------------------------------- |
| **内部外设** | 集成在芯片硅片里的功能模块               | SPI、I2C、UART、ADC、DAC、SDIO、CAN、FSMC、USB、定时器等    |
| **外部外设** | 独立的芯片或模块，焊在开发板上，靠引脚与 MCU 连接 | 传感器模块（温度、湿度）、蓝牙模块、存储芯片（SPI Flash）、液晶屏、按键、LED 等 |




#### 9.0.1. 创建项目流程:
使用三种方式创建新工程: 
* 1.选择MCU
* 2.选择板子
* 3.选择example

我们选择MCU, 搜索stm32f103vet6, 选择双击:

![alt text](image-76.png)

至此创建了一个新的cube工程.

#### 9.0.2. Pinouts&Configuration



在芯片图上点击某个引脚, 会让你选择该引脚的复用功能:


以PC13为例:

![alt text](image-88.png)

| 选项名                 | 含义               | 用途                                 |
| ------------------- | ---------------- | ---------------------------------- |
| **PC13-TAMPER-RTC** | 默认名称（芯片复位后的默认功能） | 通常用于 RTC 的 **防篡改输入**，但默认状态不是 GPIO  |
| **Reset\_State**    | 芯片复位后默认状态        | STM32 上电后此引脚默认处于浮空输入模式             |
| **RTC\_OUT**        | RTC 时钟输出         | 用于将 RTC 的 1Hz 信号输出到外部设备（极少用）       |
| **RTC\_TAMPER**     | RTC 篡改检测输入       | 如果你启用 RTC 防篡改功能，它作为输入检测外部非法干预（如重启） |
| **GPIO\_Input**     | 通用输入             | 作为普通数字输入引脚                         |
| **GPIO\_Output**    | 通用输出             | 💡 最常用模式，用于点灯、驱动外设等                |
| **GPIO\_Analog**    | 模拟输入模式           | 如 ADC 输入（在 PC13 上不会这么用）            |
| **EVENTOUT**        | 输出事件信号           | 用于低功耗应用中的事件触发，极少用                  |
| **GPIO\_EXTI13**    | 外部中断线 13         | 把这个引脚设置成中断源，如按键触发 `EXTI13` 中断      |

>可以方便看到PC14和PC15分别有一个`RCC_OSC32_IN`和`RCC_OSC32_OUT`. 这两个复用引脚用于连接`LSE`外部低速晶振的输入输出端. 启用LSE的时候, PC14和PC15会被占用. 所以一般PC14和PC15不经常用作其他用途了.

#### 9.0.3 配置gpio

我们以配置PC13为gpio输出模式然后用来点灯为例.

右边芯片图中配置PC13为`GPIO_Output`, 然后左边选择System Core->GPIO, 在Configuration横栏选择配置好的PC13行, 下方出现config选项.

![alt text](image-89.png)

| 选项名称                       | 含义                     | 推荐设置（点灯用）                       |
| -------------------------- | ---------------------- | ------------------------------- |
| **GPIO Output Level**      | 上电初始化时该引脚输出的是高电平还是低电平  | 🔺 `High`（先熄灯）或 `Low`（先亮）       |
| **GPIO Mode**              | 引脚工作模式（推挽输出(强制高(3.3v)低(0v)电平, 有拉电阻能力, 但是无法做到比如说你想让引脚接入5v会搞坏板子)或开漏输出(没有拉电阻能力需要外部电路)）      | ✅ `Output Push Pull`（点灯用）       |
| **GPIO Pull-up/Pull-down** | 是否启用芯片内部的可编程电阻配置为上下拉电阻（对输出来说通常无效）   | ❌ `No pull-up and no pull-down` |
| **Maximum Output Speed**   | IO 口输出电平切换速度（相当于驱动能力）  | ✅ `Low`（PC13 驱动能力本就弱, 点灯也不要求）           |
| **User Label**             | 你可以给这个引脚起个名字，生成代码时自动加宏 | 可选，如命名为 `LED1`                  |


#### 9.0.4 配置RCC:

在Pinout&Configuration -> System Core -> RCC 处配置.

![alt text](image-77.png)


HSE和LSE在这里配置, 选项都一样.

| 选项                          | 说明                               |
| ----------------------------- | -------------------------------- |
| `Disable`                     | 关闭 HSE，不使用外部晶振（只靠内部 RC 振荡器 HSI）  |
| `BYPASS Clock Source`         | 外部提供时钟信号（不是晶振，而是模块，如 TCXO）       |
| `Crystal/Ceramic Resonator` ✅ | 用外接晶振，**大多数开发板都选这个**（比如 8MHz 晶振） |


选项**Master Clock Output (MCO)** 即把自己主频输出到某个引脚（如 PA8）”的功能，一般用于调试或外部同步。





#### 9.0.5 配置SYS(系统)

在Pinout&Configuration -> System Core -> SYS 处配置.

配置SYSTEM:
![alt text](image-78.png)

* Debug项

该项决定**调试接口**和**跟踪功能**使用哪些引脚. 一般设为Serial Wire. 

| 选项名称                                     | 引脚占用                                 | 含义/用途                   |
| ---------------------------------------- | ------------------------------------ | ----------------------- |
| **Serial Wire**                          | 2 个引脚：`SWDIO` + `SWCLK`              | 推荐 ✅：常用的调试方式（SWD）       |
| **JTAG (4 pins)**                        | 4 个引脚：`JTDI`, `JTDO`, `JTCK`, `JTMS` | 老式调试方式（很少用了）            |
| **JTAG (5 pins)**                        | 5 个引脚：上面 4 个 + `nTRST`               | 更完整的老式 JTAG 接口          |
| **Trace Asynchronous SW**                | 基于 SWD，再用额外引脚输出调试跟踪数据                | 高级 Trace 功能，STM32F1 不常用 |
| **JTAG with Trace Synchro (1/2/4 bits)** | JTAG + Trace 输出通道                    | STM32F1 系列不支持           |
| **Trace Synchro SW (1 bit)**             | SWD + 1 条 Trace 输出线                  | STM32F1 系列不支持           |






#### 9.0.6 配置其他各种外设

在Pinout&Configuration->Analog(配置ADC和DAC); Timers(配置TIM)...





#### 9.0.7 Clock Configuration


Clock Configuration界面如下图, 其中采用了常用的配置.

![alt text](image-87.png)

* input frequency:

	可以看到最左边主时钟树有两个input frequency, 上面那个32.768KHz的是LSE. 在Pinout&Configration里没开的话就是灰色的. 下面这个是HSE, 即板子上焊的外部晶体晶振.

* 两个内部振荡器LSI, HSI在左上角.

* PLL Source Mux

	这个复选器决定要倍频来自HSE的信号还是HSI的信号. 一般选择HSE.

* PLLMul

	可以选择PLL的倍频倍数. 对`stm13f103vet6`, 最高主频72MHz, 此处可以选择`×9`.


	如果Pinouts&Configs没有选择conectivity->USB通讯, 则此处为灰

	![USB通讯时钟树](image-84.png)

* System Clock Mux:
  
	![alt text](image-85.png)

	这个复选器选择STM32的**主系统时钟SYSCLK**来自哪个源.
	在**选择HSE/HSE分频得到的PLLCLK为源**的时候可以选择是否启用CSS(clock security system). 如果启用, 它会在HSE震荡失败的时候生成一个不可屏蔽中断(NMI), 然后自动切换到HSI.

* AHB Prescaler; APB1 Prescaler

AHB和APB1分频. 分频的结果必须保证不超过该时钟线允许的最大值. 比如下图的配置中, 导致APB1总线时钟`PCLK1`最终为72KHz, 超过了允许的最大值`36 MHz max`, 于是分频器和PCLK1结果两个框标红表示错误. 需要把APB1 Prescaler设为`/2`.

![alt text](image-86.png)



## 9.1 HAL库的NVIC中断实现:

在MX图形化配置界面->NVIC配置, 在这里可以勾选你想要用的中断.

勾选的外设中断会导致在`stm32f1xx_it.h`中自动生成对应的中断函数. 它们都会调用一个很复杂的中断处理函数, 其中有非常多的弱定义. 根据你的实际中断类型, 在某个地方(main.c 或者对应外设.c)实现你的业务逻辑.

举个例子.

勾选rtc中断后, 在`stm32f1xx_it.h`中会自动生成这一段:

```c
void RTC_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_IRQn 0 */
//这里是让你写触发中断之前的抢先逻辑. 一般可以不写
  /* USER CODE END RTC_IRQn 0 */
  HAL_RTCEx_RTCIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_IRQn 1 */
//中断之后的逻辑. 一般可以不写.
  /* USER CODE END RTC_IRQn 1 */
}
```

其中, `HAL_RTCEx_RTCIRQHandler`非常长, 其中弱定义了很多函数, 比如
```c
__weak void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
__weak void HAL_RTCEx_RTCEventErrorCallback(RTC_HandleTypeDef *hrtc)
__weak void HAL_RTCEx_Tamper1EventCallback(RTC_HandleTypeDef *hrtc)
//...
```

它们对应不同的中断情况, 会根据具体发生的中断执行其中一个. 需要自己查你用的中断是哪个.  下面是一些例子.
-   **定时器更新中断 (Update event)** → `HAL_TIM_PeriodElapsedCallback`   
-   **定时器输入捕获事件** → `HAL_TIM_IC_CaptureCallback`   
-   **定时器输出比较事件** → `HAL_TIM_OC_DelayElapsedCallback`   
-   **RTC Alarm** → `HAL_RTC_AlarmAEventCallback`   
-   **UART 接收完成** → `HAL_UART_RxCpltCallback`

我们这个例子要用RTC触发alarm中断, 所以要实现的回调函数是:
```c
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
```

下面我们去`rtc.c`, 找个地方实现这个函数, 里面就是我们的中断逻辑.

注意:

在标准库 SPL 里，IRQHandler 必须你手动做三件事：
1.  **判断中断源** (`RTC_GetITStatus`)   
2.  **清除中断标志** (`RTC_ClearITPendingBit`, `RTC_WaitForLastTask`, `EXTI_ClearITPendingBit`)   
3.  **执行用户逻辑** (`rtc_alarm_triggered = 1;` 等)

而在HAL库开发, 1,2已经在这个`HAL_RTC_AlarmAEventCallback()`函数里全都安排好了. 你只需要写业务逻辑.


```c
//原来标准库写的RTC中断函数:
void RTCAlarm_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
				//清除中断标志
        RTC_ClearITPendingBit(RTC_IT_ALR);
			
				//清除外部中断线
        RTC_WaitForLastTask();
			
				//设置闹钟触发变量为1

        rtc_alarm_triggered = 1;
			
		/*****************red闪烁******************************/
		//GPIO_ResetBits(GPIOB, GPIO_Pin_5);				Delay(0x0FFFFF);
		//GPIO_SetBits(GPIOB, GPIO_Pin_5);					Delay(0x0FFFFF);
		//GPIO_ResetBits(GPIOB, GPIO_Pin_5);					Delay(0x0FFFFF);
		//GPIO_SetBits(GPIOB, GPIO_Pin_5);					Delay(0x0FFFFF);
		/*****************red闪烁******************************/
						
				EXTI_ClearITPendingBit(EXTI_Line17);
    }
}



//现在你在HAL库开发中只需要在tim.c(你要是想也可以在main.c)里实现这个名字的回调函数:
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){

    // 设置闹钟触发标志
    rtc_alarm_triggered = 1;

    /*****************red闪烁******************************/
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); Delay(0x0FFFFF);
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);   Delay(0x0FFFFF);
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); Delay(0x0FFFFF);
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);   Delay(0x0FFFFF);
    /*****************red闪烁******************************/	
}

```


## 9.2 HAL库FATFS读取SD卡的实现


图形化界面开启FATFS后, 产生了很多文件:

* ./FATFS/App下有: 
  * fatfs.c, 
  * fatfs.h 

* ./FATFS/Target下有
  * ffcon.h, 
  * user_diskio.c, 
  * user_diskio.h

* ./Middlewares/Third_Party/FatFs/src/下有
  * diskio.c, 
  * ff.c, 
  * diskio.h, 
  * integer.h...

**这些目录/文件的角色...先看懂再改.**

-   `Middlewares/Third_Party/FatFs/src/`  
    这是 FatFs 的**上层中间件实现**（核心库文件：`ff.c`、`ff.h`、以及一个通用的 `diskio.c` ）。通常 `diskio.c` 是 FatFs 和硬件驱动之间的“接口层”模板（有时里面会调用你在 Target 下实现的低层函数）。**不要盲改核心库**，除非你知道在做什么。
    
-   `./FATFS/Target/`  
    这个目录通常是放**板级/目标平台的实现**（低层磁盘驱动、GPIO/CS 具体实现）的地方，CubeMX 有时会生成 `user_diskio.c` 在这里，目的是让你把具体的硬件实现放这里以避免改动中间件。优先在这里实现 SPI 版本的磁盘驱动。
    
-   `./FATFS/App`（fatfs.c/h）  
    这是示例应用层（how to use FatFs）。通常不需要动它，除非你要改变 mount/路径/行为。





## 9.3 HAL库的USART


CUBEMX配置USART这样就可以. 一般我们使用异步收发, 就是同时可以收发.

![alt text](image-168.png)


常用HAL库函数:

首先, CUBEMX自动生成的usart.c中会声明一个USART外设句柄`huart1`或2/3如果你用USART2/3的话.


```C

//阻塞发送. 串口句柄: huart1
//要发送的数据为(uint8_t *)"[数据字符串]"
//数据长度为7(这个可以用strlen()函数)
//阻塞超时时间. HAL_MAX_DELAY=0xFFFF FFFF U. 这里就是一直等待.
HAL_UART_Transmit(&huart1, (uint8_t *)"Hello\r\n", 7, HAL_MAX_DELAY);

```





### 9.3.1 DEBUG: 回环测试

这是调试小技巧. 当usart不工作时.

首先是MCU自回环. 不连接CH340, 短接TX和RX, 运行下面的回环代码(思路就是发送数据, 然后接收数据, 看能否接收到.) 可以使用亮灯/OLED等手段查看return结果.

如果MCU没问题, 继续排查ch340串口. 连接ch340串口设备, 在PC端回环查看能否打印信息.



```C
//串口测试
#if 1
//printf("usart test\r\n");


  /* 简单 MCU loopback test (blocking) */
  /* 先把 PA9 与 PA10 短接（物理） */
  uint8_t tx[] = "LOOPTEST\r\n";
  uint8_t rx;
  HAL_StatusTypeDef st;

  /* 发送一串 */
  st = HAL_UART_Transmit(&huart1, tx, sizeof(tx)-1, 100);
  if (st == HAL_OK) {
      /* 读取 1 字节看看能否接到（阻塞 200ms） */
      if (HAL_UART_Receive(&huart1, &rx, 1, 200) == HAL_OK) {
          /* 收到后再发回 PC（如果 CH340 连接且工作，则PC会看到） */
          HAL_UART_Transmit(&huart1, &rx, 1, HAL_MAX_DELAY);
      } else {
          /* 没收到：可以用LED或断点提示 */
    	  sparckle_fast(20);
      }
  }
  HAL_Delay(500);

#endif
```

请不要犯USART的TX, RX连接到CH340的TX, RX的低级错误. 

CH340的TX应该连USART的RX.


## 9.4 分析.elf文件: 最後攻勢

我真的不知道为什麽printf()函数不能重定向成功.

我现在必须找到它具体的实现过程了.

项目编译后, 默认一般生成.elf文件, 位置在`./adc_v1.0/Debug/adc_v1.0.elf`

如果没有, 看一下设置:
***
在MX项目中, 选择生成.elf文件. 
Project → Properties → C/C++ Build → Settings → MCU GCC Linker → Miscellaneous

其中有两个框可以提供命令行参数(用空格隔开.)

* Other flages: 给链接器提供额外命令行参数.
  * `-specs=rdimon.specs`: 启用**semihosting(半主机)**. 一种特殊调试模式, 它会捕获标准库函数的实现方式.
  * `-specs=nosys.specs` 不要走调试主机.
  * `-Wl,-Map=${ProjName}.map` 让linker输出一个`map`文件, 文件名为`${ProjName}.map`
* Additional object files: 手工加入想参与link的.o/.a文件路径. 一般不用.
***



使用`arm-none-eabi-nm`命令分析.elf文件.

这是arm架构的nm(name manager), 会列出目标文件的所有符号`symbol`.

eabi(embedded appplication binary interface), 嵌入式应用二进制接口


先检索包含printf符号的指令部分.

```bash
azazel@DESKTOP-NJKSK6O:/mnt/f/aza/STM32CUBE_IDE_WORKSPACE/adc_v0.1/Debug$ arm-none-eabi-nm -C adc_v0.1.elf | grep printf
08004a34 T _printf_common
080045dc W _printf_float
08004b18 T _printf_i
08007c14 T _svfiprintf_r
08007c14 T _svfprintf_r
08007e5c T _vfiprintf_r
08007e5c T _vfprintf_r
20000238 B dbg_printf_count
08008af4 T fiprintf
08008af4 T fprintf
080052ec T iprintf
080052ec T printf
0800537c T siprintf
08005310 T sniprintf
08005310 T snprintf
0800537c T sprintf
```

发现`printf`和`iprintf`同一地址: 
```
080052ec T iprintf
080052ec T printf
```
这意味着printf被定义为iprintf的弱符号或别名. i即int, 是简化版的printf，专门用于嵌入式系统，不支持浮点数.




现在根据这个`iprintf`地址`080052ec`反汇编附近的指令.

```bash
azazel@DESKTOP-NJKSK6O:/mnt/f/aza/STM32CUBE_IDE_WORKSPACE/adc_v0.1/Debug$ 
arm-none-eabi-objdump -D adc_v0.1.elf --start-address=0x080052ec --stop-address=0x080055ec

adc_v0.1.elf:     file format elf32-littlearm


Disassembly of section .text:

080052ec <iprintf>:
 80052ec:       b40f            push    {r0, r1, r2, r3}
 80052ee:       b507            push    {r0, r1, r2, lr}
 80052f0:       4906            ldr     r1, [pc, #24]   ; (800530c <iprintf+0x20>)
 80052f2:       ab04            add     r3, sp, #16
 80052f4:       6808            ldr     r0, [r1, #0]
 80052f6:       f853 2b04       ldr.w   r2, [r3], #4
 80052fa:       6881            ldr     r1, [r0, #8]
 80052fc:       9301            str     r3, [sp, #4]
 80052fe:       f002 fdad       bl      8007e5c <_vfiprintf_r>
 8005302:       b003            add     sp, #12
 8005304:       f85d eb04       ldr.w   lr, [sp], #4
 8005308:       b004            add     sp, #16
 800530a:       4770            bx      lr
 800530c:       20000030        andcs   r0, r0, r0, lsr r0

08005310 <sniprintf>:
...
```

我们发现`iprintf`的汇编实现跳转到了符号`_vfiprintf_r`. 需要进一步寻找.

```bash
080052ec <iprintf>:
  ... 
  bl      8007e5c <_vfiprintf_r>
```

我们来反汇编`_vfiprintf_r`这个符号附近的指令.


```bash
azazel@DESKTOP-NJKSK6O:/mnt/f/aza/STM32CUBE_IDE_WORKSPACE/adc_v0.1/Debug$ arm-none-eabi-objdump -D adc_v0.1.elf --start-address=0x08007e5c --stop-address=0x08007e7c

adc_v0.1.elf:     file format elf32-littlearm


Disassembly of section .text:

08007e5c <_vfiprintf_r>:
 8007e5c:       e92d 4ff0       stmdb   sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8007e60:       460d            mov     r5, r1
 8007e62:       4614            mov     r4, r2
 8007e64:       4698            mov     r8, r3
 8007e66:       4606            mov     r6, r0
 8007e68:       b09d            sub     sp, #116        ; 0x74
 8007e6a:       b118            cbz     r0, 8007e74 <_vfiprintf_r+0x18>
 8007e6c:       6a03            ldr     r3, [r0, #32]
 8007e6e:       b90b            cbnz    r3, 8007e74 <_vfiprintf_r+0x18>
 8007e70:       f7fd fa06       bl      8005280 <__sinit>
 8007e74:       6e6b            ldr     r3, [r5, #100]  ; 0x64
 8007e76:       07d9            lsls    r1, r3, #31
 8007e78:       d405            bmi.n   8007e86 <_vfiprintf_r+0x2a>
 8007e7a:       89ab            ldrh    r3, [r5, #12]

```

似乎出现了一次bl:
```bash
 8007e70:       f7fd fa06       bl      8005280 <__sinit>
```
但是这条汇编指令`__sinit`是newlib / newlib-nano 的标准 I/O 初始化函数. 初始化之后还会跳回来. 继续往下看.


这样自己找`bl`指令太慢了. 使用grip, 把从`_vfiprintf_r`开始相当长的一段汇编中的`bl`抓出来:

```bash
azazel@DESKTOP-NJKSK6O:/mnt/f/aza/STM32CUBE_IDE_WORKSPACE/adc_v0.1/Debug$ arm-none-eabi-objdump -d adc_v0.1.elf --start-address=0x08007e5c --stop-address=0x08007fec \
  | grep -E '\bbl\b|\bblx\b'
 8007e70:       f7fd fa06       bl      8005280 <__sinit>
 8007e82:       f7fd fbe0       bl      8005646 <__retarget_lock_acquire_recursive>
 8007e94:       f7fd fb16       bl      80054c4 <__swsetup_r>
 8007eb2:       f7fd fbc9       bl      8005648 <__retarget_lock_release_recursive>
 8007ef0:       f7ff ffa1       bl      8007e36 <__sfputs_r>
 8007f2c:       f7fd fb8d       bl      800564a <memchr>
 8007fbc:       f7fd fb45       bl      800564a <memchr>
 8007fde:       f7fd fb34       bl      800564a <memchr>

```

可以发现关键输出函数是`__sfputs_r`. 

而在 newlib / newlib-nano 中，`__sfputs_r` 通常会调用  
...→ `__swrite_r` → `_write_r` → `_write`（弱符号，可被用户重定义）.




我们可以验证一下`__sfputs_r`是调用`_write_r`的, 继续反汇编抓取bl指令:
```bash
azazel@DESKTOP-NJKSK6O:/mnt/f/aza/STM32CUBE_IDE_WORKSPACE/adc_v0.1/Debug$ arm-none-eabi-nm -C adc_v0.1.elf | grep __sfputs_r
08007e36 T __sfputs_r

```


越来越乱了...


最终解决了重定向问题, 只留下

```c
int __io_putchar(int ch)
{
    uint8_t c = ch & 0xFF;
    HAL_UART_Transmit(&huart1, &c, 1, HAL_MAX_DELAY);
    return ch;
}
```

bug的原因:

写了太多重定向函数, 其中包括
```c
int _write_r(struct _reent *r, int file, char *ptr, int len)
{
    (void) r; /* unused */
    return _write(file, ptr, len);
}
```
这个不是弱符号, 和libc的符号应该冲突了.

同时, 提供了错误/UB签名. `_write_r` 的签名/返回值有问题，函数栈/寄存器期望被破坏. 应该是...






## 9.9 MX开发细节...

### 9.9.1 printf&scanf浮点数


默认情况下, MX工程使用`newlib-nano`标准库进行C编译.
**newlib-nano**是一个为嵌入式系统特别优化的标准C库的简化版本, 节省Flash和RAM空间.

它对printf等函数做了极端精简, 只支持证书格式(`%d`, `%s`, `%x`...)等, 不支持浮点数(`%f`, `%lf`, `%.2f`...).

如果需要使用printf浮点数, 需要设置:

-   **右键项目** → `Properties`   
-   展开 `C/C++ Build` → 选择 `Settings`   
-   在 `Tool Settings` 选项卡中：   
    -   点击 `MCU/MPU Settings`      
    -   找到 **"Use float with printf from newlib-nano (-u\_printf\_float)", 勾选.**

![alt text](image-165.png)

### 9.9.2 IDE的项目树结构

**CubeIDE的项目结构直接就是文件系统映射!**
***
在 **STM32CubeIDE (基于 Eclipse CDT)** 里：
-   左边的 _Project Explorer_ 本质上就是 IDE 里挂载的工作区目录，它直接映射到你硬盘上的真实目录结构。   
-   你在文件管理器里新建/删除/移动文件，CubeIDE 里会同步更新（反之亦然）。   
-   所以 `folder`、`source folder`、`file`、`source file` 本质就是告诉 IDE 怎么对待这个硬盘上的对象，并不是“单独维护一份虚拟工程结构”。
    

而 **Keil (MDK-ARM)** 是老一套设计：
-   左边的 _Project Tree_ 完全是一个 **虚拟工程视图**，你得手动把文件加进去，不会自动跟随真实目录结构。    
-   实际硬盘目录和工程树没啥绑定关系，所以容易乱、容易出“找不到文件”的坑。    
-   这就是为啥很多人骂 Keil 傻逼. 它更像是 90 年代 VC6 那种手工管理项目.  

***




-   **Folder（普通文件夹）**  
    -   就是物理文件夹。       
    -   IDE 不会自动把里面的 `.c` / `.h` 当作编译输入，除非该文件夹被包含在工程的编译路径里。        
    -   常用于存放文档、资源、或者临时文件。       
-   **Source Folder（源文件夹）**    
    -   这是 Eclipse/CDT 概念：标记为“源码目录”的文件夹会被自动加到 **Include Paths** 和编译器的搜索路径里。       
    -   里面放的 `.c/.h` 会默认参与编译。       
    -   如果你用 CubeIDE 创建 `Source Folder`，它会自动在项目设置里修改 **C Source folders**。
-   Eclipse/CDT 里只有**被标记过的一级目录**才是 Source Folder。   
-   它的子目录如果没有单独标记，就只是普通 Folder。    
-   但编译器在扫 Source Folder 的时候，会递归进去编译 `.c` 文件，所以功能上没区别。








项目树视图的文件夹如果有个斜线划掉, 意思是不参与编译. 右键->`Properties`->参与编译即可

![alt text](image-166.png)


**注意! 有时候IDE里的项目树和实际磁盘不同步. 这种情况必须尝试编译, 即可同步. 重启IDE或者徒劳地在IDE视窗更改是不行的.**






### 9.9.3 特殊的PC13


F103C8T6版本中, PC13引脚同时复用RTC TAMPER(防篡改)功能,

**CUBEMX会生成错误的代码**:

当启用RTC时, 即使没有选择output功能(RTC只用来计时和触发中断, 不对外输出)

![alt text](image-167.png)

PC13引脚仍然会被`MX_RTC_Init();`函数写入rtc的CR寄存器,  `hrtc.Init.OutPut` 写 `BKP->RTCCR`（把 alarm/second/calib 输出路由到 TAMPER 引脚）, 导致PC13强制拉低无法使用.

解决方式: 在`MX_RTC_Init();`之后运行下面的代码, 强制清除 BKP->CR 的 TPE 位(Tamper 使能位)，把引脚恢复为普通用途。



```C
#if 1
  /* ---------- 放在 MX_RTC_Init() 完成后 (USER CODE BEGIN RTC_Init 2) 或 main() 中 ---------- */
  /* 确保包含 HAL 头文件： stm32f1xx_hal.h */
  /* 1) 允许访问备份域并使能 BKP 时钟 */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();      // 允许访问 RTC/BKP 寄存器
  __HAL_RCC_BKP_CLK_ENABLE();

  /* 2) 关闭 RTC 输出到 TAMPER（PC13） —— 清除 RTCCR 的 CCO/ASOE/ASOS 位 */
  BKP->RTCCR &= ~(BKP_RTCCR_CCO | BKP_RTCCR_ASOE | BKP_RTCCR_ASOS);

  /* 3) 关闭 Tamper 功能（确保 TAMPER 引脚不再被当作 Tamper）*/
  BKP->CR &= ~BKP_CR_TPE;



  /* 4) 重新设置PC13普通GPIO: 示例为推挽输出reset拉高模式. */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  /* ---------- end ---------- */
#endif
```

### 9.9.4 无响应BUG

有时候会发生无响应打不开软件.

CUBEIDE的底层框架是Eclipse. 用这个框架的逻辑一点点排查.

CUBEIDE每次打开一个WORKPLACE都会写日志: `工作区地址/metadata/.log`

上一次失败查看日志, 发现是代理的问题. 似乎cubeide无法访问本地回环代理127.0.0.1:10809? 但是之前开着代理也没出问题呀.

但是关掉代理也没用了.

**重装**一次, 没用. (注意放在有中文字符路径会安装报错还不告诉你为啥)

**重启电脑**, 解决.


### 9.9.5 SPI不工作


**我们先补充一点SPI和SD的基本知识.**

SPI是按字节传输的.  

调用一次`HAL_SPI_TransmitReceive(&hspi1, &dummy, &resp, n, 200);`
即传输n bit数据.

在一次字节传输时, 主机发送8个SCK. 每个周期, 从机从`MISO`输出1bit, 主机从

* 调用`HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 1, 200);` 意味着:
  * 主机发送8个SCK. 
  * 每个周期, 从机将应答缓冲区`SR_out`从`MISO`输出1bit到主机`rx`.
  * 每个周期, 主机将`tx`从MOSI输入1bit到从机`SR_in`.
* 调用`HLA_Transimit(&hspi1, &tx, 1, 200)` 意味着:
  * 主机发送8个SCK. 
  * 每个周期, 从机将应答缓冲区`SR_out`从`MISO`输出1bit到主机, **但主机丢弃**.
  * 每个周期, 主机将`tx`从MOSI输入1bit到从机`SR_in`.
* 调用`HLA_Receive(&hspi1, &rx, 1, 200)` 意味着:
  * 主机发送8个SCK. 
  * 每个周期, 从机将应答缓冲区`SR_out`从`MISO`输出1bit到主机`rx`.
  * 每个周期, 主机将`0x1`从MOSI输入1bit到从机`SR_in`. 也即dummy字节, 8周期一共发送`FF`.
  * 换句话说, 下面两个命令是等价的:
    * `uint8 tx = 0xFF; HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, n, 200);`
    * `HAL_SPI_Receive(&hspi1, &rx, 1, 200);`

* 一个遵守SPI从机协议的系统需要有一个**能接收SCK并发送数据的缓冲区**. 其实就是个**被动移位寄存器**. 主机打几下SCK, 从机吐几bit数据. 这就是SPI的全部内容了. 
* 这也要求, 通过SPI协议工作的系统, 工作的最小周期为8个SCK(即使从机是很厉害的计算机可以在1SCK就做出很多事.)
* SPI协议本身不定义回应的数据意味着什麽. 

***
***
对于SD卡的情况:

SD卡约定:

没有准备好或者空闲时, 回应`0xFF`. 

SD卡在一个SCK收到命令后, 需要接下来几个SCK来处理.

SD 协议里**明确允许从机延迟响应**。
> After a command has been sent, the card may respond with 0xFF for an arbitrary number of bytes (each byte = 8 clock cycles), until it becomes ready to send the valid response token.

也就是说：
-   卡在内部要解析命令、执行判断（是否进入 SPI 模式、是否空闲、是否忙）. 这一切工作以8个SCK周期为最小粒度.
-   在这期间，**主机仍要持续发时钟**, SD卡因为没准备好, 发 `0xFF` 字节.     
-   一旦内部准备完毕，从机下一次移出的字节就会变成响应码（如 `0x01`）。


SD卡其他的MISO输出:

-   在写操作期间，卡可能在 MISO 上持续输出 0x00（低电平）直到内部写完成；主机需循环读取直到读到 0xFF 表示卡空闲。





问题解决log:

总之成功了!!!!!!!换用淘宝的十块钱的模块就可以了.

项目大概是这样的.

连接好物理引脚VCC, GND, MISO, MOSI, SCK, CS.

注意这个项目的CS用的是软件控制, 其实就是你自己配置个GPIO叫做SD_CS_PIN然后软件控制它高低.

MX配置:

启用FATFS;

配置`SPI1`外设, 以及一个GPIO当作CS.

注意点:
SPI外设的NSS要disable, 因为我们是软件控制CS
CPOL, CPHA要为Low, 1Edge.

![alt text](image-169.png)


你最头疼的代码配置:

. 


首先调用层级:

```
//SPI读写SD卡的调用关系:
//data_logger.c->ff.c->diskio.c->SDdriver.c->HAL

//ff.c(最高层文件系统)->
//diskio.c/user_diskio(接口适配层)->
//SDdriver.c(BSP层)->
//HAL(底层)


//BSP层。 它调用HAL库(HAL_GPIO_WritePin, HAL_SPI_TransmitReceive, HAL_Delay...), 直接操作SPI和GPIO.

//它定义然后暴露这些函数, 供上层(diskio.c)调用.
//uint8_t		 	SD_init(void);
//void 				SD_CS(uint8_t p);
//uint32_t  	SD_GetSectorCount(void);
//uint8_t 		SD_GETCID (uint8_t *cid_data);
//uint8_t 		SD_GETCSD(uint8_t *csd_data);
//int 				MSD0_GetCardInfo(PMSD_CARDINFO SD0_CardInfo);
//uint8_t			SD_ReceiveData(uint8_t *data, uint16_t len);
//uint8_t 		SD_SendBlock(uint8_t*buf,uint8_t cmd);
//uint8_t 		SD_ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);
//uint8_t 		SD_WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);


//void SPI_setspeed(uint8_t speed);
//uint8_t spi_readwrite(uint8_t Txdata);

```

要直接移植的文件:

* `项目/Middlewares`. 不要动. 好像这个是MX选择Fatfs后生成的, 其实都不用移植?
* `项目/Fatfs`.  
  * `项目/Fatfs/App`里的`fatfs.c, fatfs.h`别动.
  * `项目/Fatfs/Target`里的`user_diskio.c` 也不需要改. 
* `项目/Driver/SDdriver/SDdriver.c`是调用单片机的最底层. 它也定义了CS引脚的软件行为. 按理说也不需要改. 注意你需要在main.h中定义`#define SD_CS_Pin GPIO_PIN_4 #define SD_CS_GPIO_Port GPIOA`
* main.c中记得
```
#include "logger.h"
#include "SDdriver.h"
```

先确定SPI能正常工作:
回环测试即可. (略)

再确定SD卡可以正常SPI通信:

```C
 /* 简单 SPI / SD sanity test */
  uint8_t tx[10];
  uint8_t rx[10];
  for (int i=0;i<10;i++) tx[i]=0xFF;

  /* 拉高 CS, 片选[关]（假设 CS = GPIO_PIN_x, adjust accordingly） */
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
  HAL_Delay(1); /* 80 clocks will be sent below */

  /* 用SPI往MOSI发送 10 字节 0xFF.  同时从MISO读取10字节. 如果SPI主机时钟跑起来了, 但从机无响应(SD卡空通信, 没SD卡/片选关了)会返回 0xFF , 表示SPI硬件在工作, 但从机没有回答.*/
  HAL_StatusTypeDef st = HAL_SPI_TransmitReceive(&hspi1, tx, rx, 10, 200);
  printf("SPI txrx ret=%d rx[0]=%02X rx[1]=%02X ...\r\n", st, rx[0], rx[1]);
```

测试结果:

```
SPI txrx ret=0 rx[0]=00 rx[1]=00 ...
```

这她妈根本意味着啥也没有. 你妈的.

换了个spi-sd模块, 测试返回FF了.


然后尝试发送cmd0给SD卡, 看看能不能正常进入SPI模式.

```C
#if 1
   uint8_t resp;
   HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);//CS拉低, 选中
   uint8_t cmd0[] = {0x40,0x00,0x00,0x00,0x00,0x95};
   HAL_SPI_Transmit(&hspi1, cmd0, sizeof(cmd0), 200);//发送cmd0给SD卡


   // 连续读若干字节看有没有非0xFF
   for (int i = 0; i < 10; i++) {
       uint8_t dummy = 0xFF;
       HAL_SPI_TransmitReceive(&hspi1, &dummy, &resp, 1, 200);
       printf("resp[%d]=%02X\r\n", i, resp);
   }
   HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);

#endif
```

测试结果:
```
resp[0]=FF
resp[1]=01
resp[2]=FF
resp[3]=FF
resp[4]=FF
resp[5]=FF
resp[6]=FF
resp[7]=FF
resp[8]=FF
resp[9]=FF
```
说明SD卡成功花了1字节时间(8SCK)处理了cmd0并且返回了`01`正常返回值.

到这里一般就十拿九稳了.

现在做SD卡读写测试

```C
#if 1
  //SD test begin
  printf("SD Card test start.\r\n");

  // FatFs 需要的对象
  FATFS fs;
  FIL file;
  FRESULT res;
  UINT bytes_written;
  printf("fatfs obj created.\r\n");

  // 挂载文件系统
  res = f_mount(&fs, "0:", 1);
  if (res != FR_OK) {
      printf("f_mount failed: %d\r\n", res);
      Error_Handler();
  }
  printf("f_mount ok\r\n");

  // 打开（如果不存在则创建）文件 test.txt
  res = f_open(&file, "0:test.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (res != FR_OK) {
      printf("f_open failed: %d\r\n", res);
      Error_Handler();
  }
  printf("f_open ok\r\n");

  // 写入 "123\n"
  const char *text = "123\n";
  res = f_write(&file, text, strlen(text), &bytes_written);
  if (res != FR_OK || bytes_written == 0) {
      printf("f_write failed: %d\r\n", res);
      Error_Handler();
  }
  printf("f_write ok (%u bytes)\r\n", bytes_written);

  // 关闭文件
  f_close(&file);
#endif
```

测试结果:
```
SD Card test start.
fatfs obj created.
f_mount ok
f_open ok
f_write ok (4 bytes)
```

把卡拔了放进读卡器, 看到确实有文件`TEST.TXT`, 里面是123.

SD卡成功SPI读写.

现在你可以用我自己写的datalogger进行你想要的读写操作了.









###


# 0 上版流程 & STM32项目结构:

### 0.1 上板流程

#### 安装usb转串口驱动
即ch340. 驱动安装好后, 板子插`usb转串口`那个usb口. 上电后, 在电脑设备管理器-端口应当能看到ch340端口.不上电就没有. 废话!


#### 先安装keil(5.24以上版本,最好)
然后使用注册机破解它.
然后找要安装的包pack文件(即cmsis pack),双击安装.
然后就可以打开keil使用了.


#### 新建project流程:
keil里新建一个μvision project后, 在当前目录:
需要添加启动文件(`F:\aza\wokwok\STM32\野火【STM32F103开发板-指南者】资料\3-STM32官方资料(标准库固件包; hal固件包, 一些手册)\标准库固件包\STM32F10x_StdPeriph_Lib_V3.5.0\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm`)
需要添加自己的.c文件.
需要添加一个空的头文件.

#### 生成编译文件
写好后编译. 记得勾选生成hex文件.

#### (串口)下载
然后(这里用串口通信)使用串口下载软件(mcuisp):
	搜索串口,选择对应的ch340串口. bps就115200吧别太高了
	选择下载时的程序文件为生成的hex文件.(在项目文件夹的objects里)
	选择DTR低电平复位; RTS高电平进bootloader.(这个的原理可以参考野火文档-isp一键下载原理分析.)

### 0.2 标准库STM32项目结构
```
azazel@DESKTOP-NJKSK6O:/mnt/f/aza/WOKWOK/STM32/my_projects/ADC_DMA_TIM_interrupt$ tree
.
├── Libraries(官方库文件. 这些都是和内核相关的文件, 大部分情况下你几乎不会去修改, 只会include它们, 比如`#include stm32f10x.h`)
│   ├── CMSIS
│   │   ├── core_cm3.c
│   │   ├── core_cm3.h
│   │   ├── startup(.s文件是)
│   │   │   ├── startup_stm32f10x_cl.s
│   │   │   ├── startup_stm32f10x_hd.s
│   │   │   ├── startup_stm32f10x_hd_vl.s
│   │   │   ├── startup_stm32f10x_ld.s
│   │   │   ├── startup_stm32f10x_ld_vl.s
│   │   │   ├── startup_stm32f10x_md.s
│   │   │   ├── startup_stm32f10x_md_vl.s
│   │   │   └── startup_stm32f10x_xl.s
│   │   ├── stm32f10x.h(主头文件. 包含所有寄存器的定义.)
│   │   ├── system_stm32f10x.c(一些系统初始化函数)
│   │   └── system_stm32f10x.h(一些系统初始化函数)
│   └── FWlib(即Firmware Library, STM32F10x标准固件库)
│       ├── inc(外设的头文件)
│       │   ├── misc.h
│       │   ├── stm32f10x_adc.h
│       │   ├── stm32f10x_bkp.h
│       │   ├── stm32f10x_can.h
│       │   ├── stm32f10x_cec.h
│       │   ├── stm32f10x_crc.h
│       │   ├── stm32f10x_dac.h
│       │   ├── stm32f10x_dbgmcu.h
│       │   ├── stm32f10x_dma.h
│       │   ├── stm32f10x_exti.h
│       │   ├── stm32f10x_flash.h
│       │   ├── stm32f10x_fsmc.h
│       │   ├── stm32f10x_gpio.h
│       │   ├── stm32f10x_i2c.h
│       │   ├── stm32f10x_iwdg.h
│       │   ├── stm32f10x_pwr.h
│       │   ├── stm32f10x_rcc.h
│       │   ├── stm32f10x_rtc.h
│       │   ├── stm32f10x_sdio.h
│       │   ├── stm32f10x_spi.h
│       │   ├── stm32f10x_tim.h
│       │   ├── stm32f10x_usart.h
│       │   └── stm32f10x_wwdg.h
│       └── src(外设的实现文件)
│           ├── misc.c
│           ├── stm32f10x_adc.c
│           ├── stm32f10x_bkp.c
│           ├── stm32f10x_can.c
│           ├── stm32f10x_cec.c
│           ├── stm32f10x_crc.c
│           ├── stm32f10x_dac.c
│           ├── stm32f10x_dbgmcu.c
│           ├── stm32f10x_dma.c
│           ├── stm32f10x_exti.c
│           ├── stm32f10x_flash.c
│           ├── stm32f10x_fsmc.c
│           ├── stm32f10x_gpio.c
│           ├── stm32f10x_i2c.c
│           ├── stm32f10x_iwdg.c
│           ├── stm32f10x_pwr.c
│           ├── stm32f10x_rcc.c
│           ├── stm32f10x_rtc.c
│           ├── stm32f10x_sdio.c
│           ├── stm32f10x_spi.c
│           ├── stm32f10x_tim.c
│           ├── stm32f10x_usart.c
│           └── stm32f10x_wwdg.c
├── Listing(汇编清单文件)
│   ├── USART.map
│   └── startup_stm32f10x_hd.lst
├── Output(编译结果. 其中有大量.s(汇编源代码), .crf(keil专用中间文件), .d(记录源文件之间的依赖关系的文件), .o(目标文件), .axf(ARM处理器调试文件), .htm(网页报告)....以及最重要的.hex, 用于烧录的16进制可执行文件. 你最终要烧录的东西就是这个.)
│   ├── USART.hex(用于烧录的16进制可执行文件)
│   ├── USART.htm
│	.....
├── Project(keil工程文件.)
│   └── RVMDK（uv5）
│       ├── BH-F103.uvguix.azazel(用户个性化配置)
│       ├── BH-F103.uvoptx(项目的配置和编译选项)
│       ├── BH-F103.uvprojx(可以用keil打开的工程文件.)
│       └── DebugConfig(调试器的配置)
│           ├── ADC_DMA_STM32F103VE_1.0.0.dbgconf
│           └── USART_STM32F103VE_1.0.0.dbgconf
├── User(这里都是用户自己编写的内容. 用户主代码区! 一般在这里编写我们自己的代码.)
│   ├── adc(如果你要用一个外设比如adc, 用模块化的风格编程.下同...)
│   │   ├── bsp_adc.c
│   │   ├── bsp_adc.h
│   │   └── readme.txt
│   ├── tim
│   │   ├── bsp_tim.c
│   │   └── bsp_tim.h
│   ├── usart
│   │   ├── bsp_usart.c
│   │   └── bsp_usart.h
│   ├── main.c(主函数, 程序入口.)
│   ├── stm32f10x_conf.h
│   ├── stm32f10x_it.c(中断服务函数.)
│   ├── stm32f10x_it.h
├── keilkill.bat(看起来意思是个清理工具, 可能会帮你删除所有的编译文件吧? )
└── readme.md
```

# 1 各种外设

* STM32硬件资源描述图

![STM32硬件资源描述图](image-82.png)

* stm32f10系列引脚分类

![stm32f10系列引脚分类](image.png)

## 1.1 GPIO:

通用输入输出端口的简称. 也就是软件可以控制的引脚.
这个外设在APB2总线连接. 使用时复位和时钟信号要去APB2总线的对应控制寄存器打开.
一共有GPIOx, x=`ABCDE`五个端口, 每个端口有个16个引脚. 在板子上比如PB0即为GPIO的B端口0号引脚.



### 1.1.1 GPIO结构的发展史
GPIO在发展中, 支持的模式也在发展.

![alt text](image-130.png)


### 1.1.2 (old) GPIO的标准双向输入输出模式

GPIO从最早期的版本是**标准双向输入输出模式**:

![alt text](image-131.png)

其中的上拉电阻很大! 显然, 因为输出电路只有上一节`GPIO的输出电路`中的开漏输出结构, 没有直接连接VCC(通过很大的上拉电阻后电流极小无法驱动硬件)的部分, 所以该模式下没有`push`功能.

如图, 开漏输出没有办法直接直接对外输出电流, 需要接一个**VCC+小型上拉电阻**.
![alt text](image-136.png)


![alt text](image-132.png)


### 1.1.3 STM32的GPIO结构

STM的GPIO结构如图所示. 它分为输入电路和输出电路两部分.

![alt text](image-137.png)

#### 1.1.3.1 GPIO的输入电路:



![alt text](image-138.png)

其中的**TTL施密特触发器**可以把模拟信号转换为数字信号. 



STM32的输入电路结构提供了4种输入模式: 
* 上拉输入(悬空时读取高电平)
  * ![alt text](image-147.png)
* 下拉输入(悬空时读取低电平, 不常用.)
  * ![alt text](image-146.png)
* 浮空输入(最常用. 不易受干扰.)
  * ![alt text](image-145.png)
* 模拟输入(上下拉电阻和施密特触发器都断开, 输入信号直接传递到片上外设或者复用到adc外设. 最省电.)
  * ![alt text](image-149.png)


#### 1.1.3.2GPIO的输出电路:

![alt text](image-150.png)

可以看到, 通过排列组合

控制器通过控制`PMOS`和`NMOS`来改变GPIO的输出模式.

可以看到, 通过排列组合`PMOS`和`NMOS`(但是不能都打开会短路), 得到三种输出情况:
* PMOS打开NMOS关闭, GPIO的VCC给外部充电. 输出高电平(push)
* PMOS关闭NMOS打开, GPIO连接的外部电路的电源通过NMOS放电. 输出低电平(pull)
* 全部关闭: 外部影响不到GPIO. 浮空/高阻态
于是, 可以设计GPIO不同**输出模式**, 其实就是为了防止危险, 把上述三种输出情况分成几组允许的组合.

有4种输出模式:
* **推挽输出模式**: 
	只允许`push`和`pull`模式.

	能驱动高低电平, 但是不支持OR线与功能.
* **开漏输出模式**: 
	只允许`pull`和`浮空`模式. 该模式下`PMOS`永远关闭, 所以电路图可以退化为:

	![alt text](image-126.png)

	此时`NMOS`的漏极啥也没接所以称为开漏. 显然, 离开了VCC, 开漏输出本身不具有输出高电平的能力, 需要外接高电平.* 部分单片机已经配置好了电压源-上拉电阻.
	
	**开漏输出应用**:
	* 1. GPIO的高电平电压不匹配的时候, 可以用**开漏输出模式** + **外接合适的电压源** 实现输出要求电压的高低电平.

	![alt text](image-127.png)

	* 2. 可以支持**OR逻辑**, 多个GPIO同时控制一个输入:

		![alt text](image-128.png)
		![alt text](image-129.png)
	如上图, 如果两个GPIO使用`推挽输出`, 则一个push一个pull会形成通路烧毁.
	若两个GPIO都配置成开漏输出然后外接电压+上拉电阻, 就可以实现OR控制.
	**事实上, `I2C`, SMBUS等通信因为需要线与, 所以它们使用的都是开漏输出模式.**
* **复用推挽输出**
* **复用开漏输出**

### 1.1.4 STM控制GPIO的寄存器

GPIO的完整结构可以看成**寄存器-控制(驱动)器-针脚**. 前面讲的输入/输出电路图结构, 也成为**输入/输出驱动器**. 而前面的寄存器控制这套电路哪里断开哪里闭合. 用户通过软件设置这些寄存器.


![alt text](image-148.png)

这些**GPIO配置寄存器**为:
| GPIO配置寄存器名  | 含义                                           |
| ------ | ------------------------------------------------ |
| `CRL`  | 配置低 8 位引脚（Pin 0\~7）的控制寄存器。配置引脚的模式/功能等。           |
| `CRH`  | 配置高 8 位引脚（Pin 8\~15）的控制寄存器。                      |
| `IDR`  | 输入数据寄存器（Input Data Register），可读某个引脚当前输入的电平状态。    |
| `ODR`  | 输出数据寄存器（Output Data Register），写入这个寄存器来输出电平。      |
| `BSRR` | 位设置/复位寄存器（Bit Set/Reset Register），可以快速置位/清零某个引脚。 |
| `BRR`  | 位复位寄存器（Bit Reset Register），专门用来清零引脚。             |
| `LCKR` | 锁存寄存器（Lock Register），用来锁住引脚配置，防止修改。              |


#### 1.1.4.1 CRL和CRH

其中**CRL**和**CRH**配置了GPIO的模式.
GPIO引脚的**模式**通过 
* **GPIOx_CRL(控制 GPIOx的第 0～7 引脚 `Pin0~7`)**
* **GPIOx_CRH(控制 GPIOx的第 8～15 引脚 `Pin8~15`)**
寄存器配置.

这两个寄存器一共64bit宽, 每个引脚分到4bit: MODE[1:0] + CNF[1:0]

| CRL/CRH寄存器字段名   | 位数 | 作用                          |
| ----------- | -- | --------------------------- |
| `MODE[1:0]` | 2位 | 指定“输出模式下”的速率（输入模式时强制为 `00`） |
| `CNF[1:0]`  | 2位 | 指定该引脚的“具体模式”（比如推挽、开漏等）      |

它们对应的模式:
| `MODE[1:0]` | `CNF[1:0]` | 模式名称                       | 输入/输出 | 说明                 |
| ----------- | ---------- | -------------------------- | ----- | ------------------ |
| `00`        | `00`       | 模拟输入（Analog）               | 输入    | 供 ADC 等使用，低功耗      |
| `00`        | `01`       | 浮空输入（Floating Input）       | 输入    | 默认输入状态             |
| `00`        | `10`       | 上/下拉输入（Pull-up/down Input） | 输入    | 由 `ODR` 设置是上拉还是下拉  |
| `00`        | `11`       | ❌ 保留，禁止配置                  | ❌无效   | STM32F1 不允许这种组合    |
| `01/10/11`  | `00`       | 通用推挽输出（General Output PP）  | 输出    | MCU 能主动拉高和拉低电平     |
| `01/10/11`  | `01`       | 通用开漏输出（General Output OD）  | 输出    | 只能拉低电平，需外部上拉       |
| `01/10/11`  | `10`       | 复用推挽输出（AF Output PP）       | 输出    | 比如 SPI/USART/PWM 用 |
| `01/10/11`  | `11`       | 复用开漏输出（AF Output OD）       | 输出    | 比如 I2C 等需开漏输出的复用场景 |

#### 1.1.4.2 APB2总线 外设时钟使能寄存器(RCC_APB2ENR); APB2 外设复位寄存器 (RCC_APB2RSTR)
在AHB总线上. 地址为`0x4002 1000 - 0x4002 13FF`
首先是使能和复位信号. 如果想使用GPIO端口(在总线APB2连接), 则需要开启复位和时钟控制RCC. 外设的时钟和使能信号默认是关闭的.
当外设时钟没有启用时，软件不能读出外设寄存器的数值，返回的数值始终是0x0。

#### 1.1.4.3 IDR (input data reg)输入数据寄存器.

它是一个32bit寄存器. 高16位预留为0没用. 低16位保存端口0~端口15的实际物理电平, 0为低电平.

它储存引脚实际物理电平.

它的值用库函数
`GPIO_ReadInputDataBit()`来读取.

#### 1.1.4.4 ODR (output data reg)输出数据寄存器.

它是一个32bit寄存器. 高16位预留为0没用. 低16位控制端口0~端口15的输出电平, 0为低电平.

ODR寄存器只有**推挽输出模式**时有用. 当推挽驱动时, IDR被驱动为ODR的值.

它的值用库函数
`GPIO_ReadOutputDataBit()`来读取.

在GPIO的不同模式下:
| 模式     | 输入逻辑（IDR） | 输出逻辑（ODR）  |
| ------ | --------- | ---------- |
| 输入模式(上/下拉输入, 浮空输入, 模拟输入)   | ✅使能       | ❌关闭        |
| 推挽输出模式 | ✅仍然打开！    | ✅使能（推高/推低） |



### 1.1.5 GPIO的配置写法

比如要配置3个gpio为**推挽输出**模式, 来用于控制三个`LED外设`, 规范的做法:

```c
LED_GPIO_Config()	//在main.c 中调用配置函数. 具体的配置函数写在`bsp_led.c`中.


//bsp_led.h中配置宏名称(主要是要用的gpio名字). 实现硬件配置与程序逻辑解耦~
/* 如果你想换一个gpio用, 需要更改下面三个属性. port和clk必须依照你具体gpio对应的板上结构哦, 去看引脚吧. */
// R-红色: 这是一个属于`GPIOB`组的5号gpio, 是PB5引脚的复用. 使用apb2_gpiob的总线.
#define LED1_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED1_GPIO_PIN		GPIO_Pin_5			        /* 连接到SCL时钟线的GPIO */

// G-绿色
#define LED2_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED2_GPIO_PIN		GPIO_Pin_0			        /* 连接到SCL时钟线的GPIO */

// B-蓝色
#define LED3_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED3_GPIO_PIN		GPIO_Pin_1			        /* 连接到SCL时钟线的GPIO */

//假设LED1的GPIO你想换一个, 比如你想用PB8(gpio_pin_8), 则要查询: 它属于GPIOB端口, 则:
#define LED1_GPIO_PORT  GPIOB
#define LED1_GPIO_CLK   RCC_APB2Periph_GPIOB
#define LED1_GPIO_PIN   GPIO_Pin_8


//`bsp_led.c`中的配置
void LED_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体.*/
	GPIO_InitTypeDef GPIO_InitStructure;


    /************* 通用配置：适用于所有 LED 引脚的通用设置 *************/
	/*开启LED相关的GPIO外设时钟*/
	RCC_APB2PeriphClockCmd( LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE);

	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	


    /************* 单独配置每个 LED 的引脚 *************/
	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
	
	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;

	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
	
	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;

	/*调用库函数，初始化GPIOF*/
	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);


	//初始化默认状态设置(即将)
	/* 关闭所有led灯	*/
	GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
	
	/* 关闭所有led灯	*/
	GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	 

	/* 关闭所有led灯	*/
	GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}

```







## 1.2 时钟和RCC(reset and clock control)

STM32F103 的时钟系统使用 时钟树 进行管理，所有外设（GPIO、USART、ADC 等）都需要时钟才能工作。

**HSE(High-Speed External clock), 高速外部时钟**由外部晶振提供信号.
可以通过 **PLL（锁相环）** 倍频，把 STM32 的主频提升到 72MHz.


RCC是stm32的一个外设,  负责系统复位和时钟管理.
它控制gpio, adc, usart, apb1, apb2灯外设的始终开关.
比如，RCC->APB2ENR |= (1 << 9); 用于 开启 ADC1 时钟。

时钟树:
![时钟树](image-55.png)

* `HSE_OSC`震荡模块配合OSC_IN, OSC_OUT引脚输入输出, 产生HSE震荡时钟信号. (stm32本身不包含一个晶振模块. `HSE_OSC`模块只是利用驱动电路来配合外部的晶振工作, 产生信号给内部模块用.)
* 紧接着的`PLLXTPRE`(PLL extended pre-divider)预分频器模块.
  * ENABLE: 将HSE信号1/2分频, 然后送入`PLL选择器`.
  * UNENABLE: HSE直接送入`PLL选择器`.
* `PLL选择器`通过PLLSRC使能, 选择(分频的)`HSE`信号或HSI信号, 传给`PLL`.
* `HSI`是内部振荡器信号. 比较简洁, 不需要晶振和控制电路, 产生8MHZ的劣质信号...
* 





## 1.4 接口的重映射
![alt text](image-39.png)
例如`USART2_TX`, 当控制重映射的寄存器USART2_REMAP=0时, 它被分配到默认物理引脚PA2.
当USART2_REMAP=1时, 它被分配到物理引脚PD5.


## 1.5 DMA (data memory access) 直接存储器访问

功能: 在两处搬运数据, 不占用cpu.
比如串口输入时, 我们把sram中的字符串数据arr先搬运到cpu暂存reg, 然后搬运到USART的DR, 然后发送.
如果能使用DMA, 只需要cpu发个命令, 让数据arr自动搬运到目的地(USART_DR), cpu可以去做别的事.

stm32f103大容量版本有两个DMA外设`DMA1`, `DMA2`.
每个DMA有7个通道. 可以实现
* P->M
* M->P
* M->M

P:外设peripheral.  
M:

注意:ADC2没有DMA功能. 可以用ADC1或ADC3.

## 1.6 TIM(timer)定时器

![定时器分类](image-53.png)

### 1.6.1 基本定时器

![基本定时器](image-54.png)

##### 时基 time base
![时基](image-56.png)
![基本定时器框图](image-57.png)
* 计时器 **CNT**
* 预分频器 **PSC**(Prescaler)
  * ![预分频器时序图](image-60.png)
  * ![预分频器时序图2](image-61.png)
* 自动重装载寄存器 **ARR** : 计数器到达该寄存器值后自动清零.

* **影子寄存器**: 
  * PSC和ARR都有`影子寄存器`(框图中, PSC和ARR矩形有一个影子图案). 其实就是打拍. 比如ARR的影子寄存器称为称为`自动重装载预装载`, 使能在TIMx_CR1寄存器的7位`ARPE`.
    * ![影子寄存器](image-58.png)
    * ![ARR的自动重装载的使能寄存器](image-59.png)

计数器时钟`CK_CNT`频率: 
$CK\_CNT=\frac{72M}{PSC+1}$
于是计数器技术一轮需要的时间为$\frac{ARR}{CK\_CNT}$

时基结构体的初始化:
```c
typedef struct{
//PSC的分频因子
uint16_t TIM_Prescaler;

//计数模式. (基本计时器TIM7,TIM8不用配, 只能向上计数)
uint16_t TIM_CounterMode;

//ARR的值, 计数周期
uint32_t TIM_Period;

//外部输入的时钟分频因子.(基本计时器没有)

//重复计时器(高级定时器专用)
//如果开启相关功能, 可以在ARR满的时候不产生中断/DMA请求, 而是清零并且让重复计时器++. 感觉没啥用...你扩倍ARR设定不就好了.
uint8_t TIM_ClockDivision;
//

} TIM_TimeBaseInitTypeDef
```

### 1.6.2 高级定时器 TIM1, TIM8

三个功能:
* 定时
* PWM
* 捕获
  * 脉冲宽度
  * PWM

![高级TIM的GPIO分配](image-62.png)


![高级TIM功能框图](image-63.png)
![功能框图划分](image-64.png)



## 1.7 LED

指南者板子有三色led. 它们通过**灌电流**的方式连接到GPIO口. 灌电流即意味着LED先直接连接高电平, 然后接入GPIO口, 当GPIO口以**pull模式**(即内部只存在gpio-MOS-GND通路: )运行时

其电路参见指南者原理图:

![alt text](image-81.png)

可见, red连接的gpio口是`PB5`, 当`PB5`低电平时导通.


如果你要设置gpio的话如下
```c
void REDLED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 开启 GPIOB 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置 PB5 为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 默认PB5高电平, 关闭 LED
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
```


## 1.8 晶振

![晶振图](image-83.png)

* 实体晶振:
  * `Y1`处即HSE所用的8MHz晶振(通过PLL倍频为72MHz)
  * `Y2`处即LSE所用的32.768KHz晶振.

* 内部振荡器晶振:
  * 串口芯片`CH340C`自带一个12MHz晶振在内部. (对应`Y3`处没有晶振. 否则用的是`CH340`的话, 这里要焊一个12MHz实体晶振给CH340G用)
  * `HSI`. 在MCU内部. 8MHz. 和焊接的HSE(PLL倍频之前)一样, 但是精度要差得多.
  * `LSI`. 在MCU内部. 40kHz, 精度差.

## 1.9 key开关

查看原理图:
![KEY](image-123.png)

这是一个带防抖(0.1uF去抖电容)和上拉(4.7k上拉电阻)的按键电路.

按下key则对PA0输出高电平, 松开时PA0为低电平.

使用的四点式key按键模块有四个点触引脚, 实际上可以一个按键同时控制两个开关. 这里板子上的电路是把12和34端接在一起了. 这样就是一个简单的单刀单掷开关了.

# 2 几个例程:

### 指南者板子硬件设计:
从原理图查到:
![alt text](image-19.png)
注意PC0实际上被SPI接口占用了. 别用.




## 2.1 ADC实验
整个原理图:
![ADC原理图](image-3.png)
### 理论部分 


#### 引脚
VDDA(VDD(drain to drain) analog)即模拟模块的电源电压。
VSSA(VSS(source to source) analog)即模拟模块的地.
![原理图](image-1.png)

引脚分配图:
![ADC引脚分配](image-4.png)




可以使用这个分压结构测量更高的电压:
![分压结构](image-2.png)
很容易导出uout和uin的线性关系:
i1=uin/(R2+R3);
i2=3v3/(R1+R3);
uout=R3(i1+i2)=R3(uin/(R2+R3)+3v3/(R1+R3)). 线性关系.

每个ADC的端口:




先看adc的16个外部通道结构:
![alt text](image-6.png)
外部的16个通道在转换的时候又分为**规则通道**和**注入通道**.
其中规则通道最多有16路,注入通道最多有4路。
**规则通道**:顾名思意,规则通道就是很规矩的意思，我们平时
一般使用的就是这个通道。
**注入通道**:注入,可以理解为插入,插队的意思，是一-种不安
分的通道。它是一种在规则通道转换的时候强行插入要转换的
一种。这点跟中断程序很像,都是不安分的主。所以，注入通
道只有在规则通道存在时才会出现。


有三格寄存器来决定通道转换顺序.
#### 规则序列寄存器SQRx, x=1,2,3
  
![alt text](image-7.png)
举个例子: 如果需要adc模块转换通道3,8两个通道的模拟信号并且依次将转换结果存在寄存器里等待cpu读取, 那麽SQR寄存器要设置为:
	SQR3的[4:0],即SQ1部分设置为`00011`,即`3`,表示第一个要转换的通道是3.
	SQR3的[9:5],即SQ2部分设置为`00100`,即`8`,表示第二个要转换的通道是8.
	SQR3剩下的位和SQR2,SQR1除了`SQL`部分的位均默认为`0`, 表示不转换.
	SQL部分设置为`0010`,即`2`, 表示一共要转换的通道数为2.



#### 注入序列寄存器JQR
![alt text](image-8.png)


#### 触发源部分
![adc触发源部分](image-9.png)
需要先设置ADON寄存器让adc模块工作, 然后:
* 1.使用软件触发: 设置SWSTART和JSWSTART寄存器.
* 2.使用外部事件触发:
ADON寄存器控制adc模块工作电压是否开启(是否开启adc.)
SWSTART寄存器,控制规则通道可以开始转换了.
JSWSTART寄存器,控制注入通道可以开始转换了.

也可以用**外部事件触发**
例如,下面的模块就是规则通道的外部触发:
![alt text](image-10.png)
打开EXTTRIG寄存器开启规则通道的外部触发;然后
设置EXTSEL寄存器选择用哪个信号, 然后对应的那个计时器信号可以作为触发.
看模块可以知道, 可以用TIM1_CH1~TIM4_CH4这些计时器信号, 也可以用外部gpio信号`EXTI_11`.

这几个寄存器简单查一下发现都在32位寄存器`ADC_CR2`中.

#### 转换.
转换时间:

![alt text](image-11.png)




#### 转换结果:
规则组的数据放在`ADC_DR`寄存器.
注入组的数据放在`JDRx`寄存器.

![alt text](image-12.png)

* `独立模式`: 即单独使用adc1,2,3之一时.




#### 中断
![alt text](image-13.png)

##### EOC(end of complete)中断
##### JEOC()中断
##### AWD()中断
模拟看门狗的寄存器配置:
![alt text](image-14.png)


#### 数据换算关系
![alt text](image-15.png)


#### 初始化结构体:
![alt text](image-16.png)


### 实验部分

首先复制一个串口实验项目`USART1接发`为基础.

改文件夹名字为ADC,开始:

在User目录新建一个adc文件夹存放驱动.
在其中新建`bsp_adc.c`和`bsp_adc.h`板级支持包文件.

#### 1.编写bsp_adc.c和bsp_adc.h
在啊keil中指定头文件路径: 魔术棒->c/c++->include paths->new(insert), 选择adc文件夹, 于是添加这个路径到头文件寻找中.

写`bsp_adc.h`. 我们要在里面写条件编译. 可以从项目的bsp_usart.h中粘贴修改.
直接抄来例程中的bsp_adc.h.

```c
#include "bsp_adc.h"
//1.初始化adc要用的gpio.
void ADCx_GPIO_Config(void){
	//直接从固件库例程里抄吧~
	//地址在F:\aza\wokwok\STM32\野火【STM32F103开发板-指南者】资料\1-程序源码_教程文档\1-[野火]《STM32库开发实战指南》(标准库源码)【优先学习】\30-ADC—电压采集\1-ADC—单通道（中断读取）
	
	
	//定义一个初始化结构体变量.
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	// 配置 ADC IO 引脚模式
	// 必须为模拟输入
	//GPIO.pin是结构体GPIO_InitTypeDef的成员. 表示要配置的引脚. 此处ADC_PIN在头文件宏中定义为`GPIO_Pin_1`.
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//模拟输出模式
	
	// 初始化 ADC IO. ADC_PORT在宏中定义为`GPIOC`
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);		

}




//2.初始化ADC初始化结构体
void ADCx_Mode_Config(void){
	ADC_InitTypeDef ADC_InitStruct;
	
	//打开ADC时钟.(具体这个函数去rcc固件库里找.)
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE ); 
	
	
	//先配置结构体的第一个成员:ADC_Mode. adc模式选择. 我们现在只使用一个adc, 所以选择`独立模式`.(去固件库stm32f10x_adc.h, line94找. 先在FWLB文件夹找到stm32f10x_adc.c,然后在其include的前者跳转即可.
	//en, 初始化结构体的6个成员也要在stm32f10x_adc.h里查看结构体typedef哦.
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	
	//第二个成员是ADC_ScanConvMode(see stm32f10x_adc.h, line56). 是否扫描. 扫描是针对多通道的. 选择DISABLE(how can i know the options?? see .h pls.).
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	
	//第三个成员是ADC_ContinuousConvMode, 是否连续转换, enable.
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	
	
	//4th member: ADC_ExternalTrigConv, adc触发模式. choose 软件触发.(不使用外部触发, 外部触发是定时器/外部gpio)
	//选项参考.h文件, line123~138. 
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	
	//5th: dataalign.
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	//6th: channel nember.
	ADC_InitStruct.ADC_NbrOfChannel = 1;


	//初始化结束后, 调用adc_init.该函数的作用是: 按照上述配置好的参数, 去改写各种设置adc功能的寄存器, 实现你的配置.
	//(.h, line429: `void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);`)
	ADC_Init(ADC_x, &ADC_InitStruct);



	//3.1配置adc时钟. 前往固件库stm32f10x_rcc.c, line766:`void RCC_ADCCLKConfig(uint32_t RCC_PCLK2)`
	//函数的形参选择: 见stm32f10x_rcc.c, line760~764处说明. 此处选择8分之一分频.
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		
	
	
	//3.2配置`通道转换顺序`, `采样时间`
	//查看stm32f10x_adc.h, line442, 得到函数ADC_RegularChannelConfig.每个参数...
	//1.哪个adc, 
	//2.哪个通道, 
	//3.转换顺序(cause 我们这里只用一个adc通道所以它的rank就是1咯.)
	//4.采样时间. 见stm32f10x_adc.c, line579~588. 这里我们选择55.5个周期.
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
	
	
	
	//4.1使能adc转换完成中断, 配置adc优先级.
	//去adc.h中找到IT(interrupt)字样的函数: void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);
	//1.选的哪个adc
	//2.中断类型. ADC_IT_EOC, 规则通道.
	//3.enable/diable
	ADC_ITConfig(ADC_x, ADC_IT_EOC, ENABLE);
	
	
	//5.使能adc(就是打开adc), 准备开始转换.
	//(最快的编程方法: 把函数定义给ai看, 让她告诉你形参怎麽设置.)
	ADC_Cmd(ADC_x, ENABLE);
	
	
	//6.校准adc.
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC_x);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC_x));
	
	// ADC开始校准
	ADC_StartCalibration(ADC_x);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC_x));
	
	
	//7.软件触发adc, 开始转换.
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

//8.编写终端服务函数, 读取ADC转换的数据.
//这个函数的作用是配置 ADC 的 NVIC（嵌套向量中断控制器），也就是告诉 STM32，当 ADC 触发中断时，该如何处理。
static void ADC_NVIC_Config(void)
{
	////声明一个 NVIC_InitTypeDef 结构体变量，用来配置 NVIC.
  NVIC_InitTypeDef NVIC_InitStructure;
	// 优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	
	//选择要配置的中断源，这里是 ADC_IRQ（ADC 的中断）。
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
	
	//设置 抢占优先级 = 1，子优先级 = 1。
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	//启用 ADC 中断。
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	//调用 STM32 库函数 NVIC_Init()，把上面的配置写入 NVIC。
  NVIC_Init(&NVIC_InitStructure);
}



//包装一下上面我们写的初始化函数.
void ADCx_Init(void){
	ADC_NVIC_Config();
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}


```


#### 2.编写中断服务函数.
在USER目录中编辑文件`stm32f10x_it.c`(it即interrupt. stm32官方库默认中断处理函数都定义在这个文件.)
因为这个例子里我们的项目文件是拷贝的USART接发, 所以该文件写入了串口终端服务函数. 注释掉, 写上我们要用的adc中断服务函数.
记得包含头文件`bsp_adc.h`

#### 3.编写main函数.
在USER目录中编辑文件`main.c`.








# 3 中断, NVIC模块, EXTI模块

stm32使用模块`NVIC`(nested vectored interrupt controller,内嵌向量中断控制器)管理中断.
![中断系统](image-40.png)
右边的框图解释:
`中断`的输入量:
* 1.外设影响某个io口电平.
  * 边沿检测电路检测到上升/下降(这个自己设置,也可以是双边)后, 会给`请求挂起寄存器`信号. 
* 2.内部软件中断事件.
  * 软件中断事件寄存器也可以给请求挂起寄存器发信号. 
`请求挂起寄存器`用或门连接软件/io口信号, 即都可接收.
然后:
* 当`中断屏蔽寄存器`允许时, `请求挂起寄存器`信号发送给`EXTI`.

**EXTI**(external interrupt controller,外部中断控制器). 管理外部中断, 然后给信号到NVIC. "秘书的秘书".

stm32拥有19个`EXTI`模块, 比如EXTI0管理的i/o口为PA0,PB0,PC0,PD0,PE0.

* 注意: stm32中, 相同编号的io口(如PA0和PB0两个引脚)使用相同的EXTI线. 不能同时设置为不同的外部中断, 会失效.


**中断向量**: 即中断服务程序所在的位置.(这里的`向量`其实就是个**指针**)
**中断嵌套**: 优先级更高的中断请求打断了正在进行的中断服务.

**优先级**:
对每一个中断请求, 需要配置其
* 抢占式优先级(如果已经有正在处理的中断请求, 抢占优先级更高的中断可以打断它.)
* 响应式优先级(用来判断cpu正常运行没有中断请求的时候的优先级.) 
两种优先级都是一个数字, 数字越低(1)越高级.

#### 中断函数的例子:
```c
//中断服务函数ISR应当写在`stm32f10x_it.c`中.

//TIM6外设的中断函数. 其中有库函数和宏:
//TIM_GetITStatus,...: stm32f10x_tim.c中的库函数.
//定时器硬件模块选择
//#define	BASIC_TIM		TIM6
//...
void  BASIC_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	
		//一个全局变量.
		time++;
		//清除中断标志位
		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);  		 
	}		 	
}
```

# 5. 标准库...编程经验

## 5.1 头文件
---
根据我们的经验, 头文件大致结构为:
1. 定义初始化结构体;
2. 用宏给出结构体的成员的值的右值选项(其实就是用别名代替一个个寄存器)
3. 固件库函数. 
   1. 会有init函数, 也就是调用来初始化结构体(把填的设置写到寄存器里)
   2. ...其他常用函数. 

固件库编程中, 常用的两类结构体:
* [外设名]_InitTypeDef
  * 这个结构体用来对某个特定外设([外设名]_TypeDef类的对象)进行配置. 它储存设置用的寄存器赋值. 由用户声明, 然后作为形参传入init函数进行初始化.
* [外设名]_TypeDef
  * 这个结构体
  * 该结构体的实例由 STM32 固件库(stm32f10x.h)提供，对应 STM32 内部 USART 硬件外设的寄存器。一般用户不涉及声明新的结构体哦.
   
---



## 5.3 标准库...初始化结构体, 以USART为例
有两个结构体. 它们在固件库FWLB的stm32f10x_usart.c中

```c
typedef struct
{
  uint32_t USART_BaudRate;            /*!< This member configures the USART communication baud rate.
                                           The baud rate is computed using the following formula:
                                            - IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->USART_BaudRate)))
                                            - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */

  uint16_t USART_WordLength;          /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref USART_Word_Length */

  uint16_t USART_StopBits;            /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref USART_Stop_Bits */

  uint16_t USART_Parity;              /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref USART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */
 
  uint16_t USART_Mode;                /*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref USART_Mode */

  uint16_t USART_HardwareFlowControl; /*!< Specifies wether the hardware flow control mode is enabled
                                           or disabled.
                                           This parameter can be a value of @ref USART_Hardware_Flow_Control */
} USART_InitTypeDef;

```

如果不用使用同步功能, 不需要初始化下面这个时钟结构体咯.
```c
typedef struct {
uint16_t USART_Clock; // 时钟使能控制
uint16_t USART_CPOL; // 时钟极性(空闲的时候是高电平还是低电平.)
uint16_t USART_CPHA; // 时钟相位(第一个边沿触发还是第二个边沿触发)
uint16_t USART_LastBit; // 最尾位时钟脉冲(当 STM32 的 USART 处于 同步模式（Synchronous Mode） 时，USART 需要输出一个时钟信号 (USARTx_CK) 来同步通信。USART_LastBit 决定是否在数据帧的 最后一位 发送时再输出一个额外的时钟脉冲。)
} USART_ClockInitTypeDef;

```

## 5.4 固件库函数, 以USART为例
它们在固件库FWLB的stm32f10x_usart.c中
```c
void USART_DeInit(USART_TypeDef* USARTx);//default init. 就是把某个usart硬件`USARTx`初始化为默认值.
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);//初始化函数. 接受某个usart硬件`USARTx`和用户写的初始化设置结构体`USART_InitStruct`, 然后把前者用后者进行初始化.
void USART_StructInit(USART_InitTypeDef* USART_InitStruct);//把传入的初始化结构体设置为默认值.
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct);//用时钟初始化结构体, 来配置某个usart的时钟. (适用于同步模式. 我们一般都使用异步模式...不需要配置时钟.)
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct);//把传入的初始化结构体设置为默认值.
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);//
void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);//中断配置. usart发送数据寄存器为空/完成发送/接受数据寄存器为空..时, 都可以产生中断.
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState);
void USART_SetAddress(USART_TypeDef* USARTx, uint8_t USART_Address);
void USART_WakeUpConfig(USART_TypeDef* USARTx, uint16_t USART_WakeUp);
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint16_t USART_LINBreakDetectLength);
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
uint16_t USART_ReceiveData(USART_TypeDef* USARTx);
void USART_SendBreak(USART_TypeDef* USARTx);
void USART_SetGuardTime(USART_TypeDef* USARTx, uint8_t USART_GuardTime);
void USART_SetPrescaler(USART_TypeDef* USARTx, uint8_t USART_Prescaler);
void USART_SmartCardCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SmartCardNACKCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_OverSampling8Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_OneBitMethodCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_IrDAConfig(USART_TypeDef* USARTx, uint16_t USART_IrDAMode);
void USART_IrDACmd(USART_TypeDef* USARTx, FunctionalState NewState);
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);//获取usart的状态标志位. 
// USART_FLAG_TXE	发送数据寄存器空，表示可以发送新数据
// USART_FLAG_RXNE	接收数据寄存器非空，表示有新数据可以读取
// USART_FLAG_TC	发送完成
// USART_FLAG_ORE	过载错误
// USART_FLAG_FE	帧错误
void USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG);//手动清除 USART 标志位.
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, uint16_t USART_IT);//检查 USART 的中断标志位 是否被触发，判断是否进入某个 USART 中断状态。
// USART_IT_TXE	发送寄存器空中断
// USART_IT_RXNE	接收数据寄存器非空中断
// USART_IT_TC	发送完成中断
// USART_IT_ORE	过载错误中断
// USART_IT_FE	帧错误中断
void USART_ClearITPendingBit(USART_TypeDef* USARTx, uint16_t USART_IT);//清除 USART 的中断挂起标志，即 手动清除某个中断状态，避免中断重复触发。
```


## 5.5 编程任务和流程
* 01-初始化串口需要用到的GPIO
* 02-初始化串口, USART _InitTypeDef
* 03-中断配置(接收中断,中断优先级)
* 04-使能串口
* 05-编写发送和接收函数
* 06-编写中断服务函数





## 5.6 程序移植: 如何复用自己写的外设bsp?

1. 复制USER的外设文件夹(一般包含`bsp_外设.c`, `bsp_外设.h`), 并在keil项目中添加这些文件到1.项目和2.链接器中
2. 在项目的`stm32f10x_conf.h`中将对应的外设的库文件头文件包含(取消注释)
3. 在main.c中包含`bsp_外设.h`, 即可开始在main.c中使用该外设了.

## 5.7 delay 延时



最简单粗暴的空转延时函数法:
```c
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 
```
`Delay(0x400000);`大概会在72MHz的STM32上延时1s左右.


## 5.8 两种检测方式: 轮询扫描&设置中断



| 特性        | 轮询扫描方式              | 中断方式                    |
| --------- | ------------------- | ----------------------- |
| 检测方式      | 程序主动调用 `Key_Scan()` | 配置 `EXTI` 中断函数自动触发      |
| 响应效率      | 低，需频繁调用函数           | 高，按下立即跳转中断              |
| 占用 CPU 时间 | 高，占用主循环资源           | 低，只在触发时响应               |
| 适合应用场景    | 简单项目、低速按键、不怕阻塞      | 实时性要求高、功耗优化等            |
| 代码复杂度     | 低，易于上手              | 高，需要配置 `EXTI` + NVIC 中断 |
| 去抖动方式     | 可在 while 里等待稳定      | 通常需加软件延时或定时器过滤          |

### 1.轮询扫描
比如设置按钮key1的轮询扫描来检测按钮:

在main的无限循环中调用一个自定义`Key_Scan`函数:
```c
 while (1)
  {	
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON)
		{		
			//do sth		
		}
	} 
```
而`Key_Scan`是调用`GPIO_ReadInputDataBit`函数实现的. 它就是读取`GPIO`的**输入数据寄存器IDR**的值. 在推挽输出模式下, 其值即为设置的输出电平.


```c
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 1;	 
	}
	else
		return 0;
}
```

然后我们记得配置key为


# 6.电源管理

stm32芯片的电源模块:
![alt text](image-65.png)

特别的,stm13f103v6(指南者)型号的usb供电模块原理图:
![alt text](image-66.png)


## 6.1 三种模式

模式名称	        | CPU状态	      | 时钟状态				 | 电流典型值（@72 MHz） | 说明
|------------------|--------------|--------------------|-----------------------|------|
运行模式（Run）     | 正常运行         | 所有开启（HSE、PLL） | 20～30 mA | CPU 主频全速运行，外设全开
睡眠模式（Sleep）   | CPU停止，外设继续 | 外设时钟正常 | 6～12 mA | 用 __WFI()，中断可唤醒
停止模式（Stop）    | CPU停止          | 主时钟关闭，仅低速时钟保留 | 10～100 µA | RAM 保持，唤醒稍慢，可通过 RTC 唤醒
待机模式（Standby） | 全部停止         | 所有时钟关闭 | 1～10 µA | RAM丢失，需复位唤醒，功耗最低

### 1.睡眠模式(sleep)

通过调用_WFI()或_WFE()进入睡眠.

睡眠有两个模式: 立即和退出时. 它们主要是通过WFI()睡眠时区分的. 通过配置内核寄存器`SLEEPONEXIT`设置.

![alt text](image-67.png)

### 2.停止模式(stop)

![alt text](image-68.png)
![alt text](image-69.png)


### 3.待机模式(standby关机)

![alt text](image-71.png)


注意: 如果有备份供电(纽扣电池), `备份域`内的RTC都可以正常运行, 寄存器都可以被正常保存.

## 6.2 电源控制(POWER CONTROL, PWR)的寄存器(PWR_CR)和库函数

它们是arm cc编译器支持的一些指令, 是最底层的指令了. 我们不能再进一步跳转...
![alt text](image-74.png)
![alt text](image-72.png)

我们可以去看`Cortex-M3 权威指南`:
![alt text](image-75.png)




#### WFI和WFE命令  

![alt text](image-73.png)
## 6.3 低功耗实验

我们通过软件使寄存器:
* SLEEPDEEP = 1
* PWR_CR->PDDS = 1
* PWR_CR->WUF = 0
然后调用`__WFI()`/`__WFE`, 进入待机.

唤醒: 给`WKUP`引脚上升沿信号.

但是具体版上哪个引脚是`WKUP`引脚呢?

我们看到`数据手册 Table 5. High-density STM32F103xx pin definitions (continued)`中, `WKUP`引脚被接在`PA0` 引脚.

我们看到`指南者原理图`中, `K1`按键按下后, `PA0`引脚会通过电阻R15接入高电平.
![alt text](image-70.png)



## 7. RTC

用rtc实现一个中断：

先看bsp_rtc.c
```c
#include "bsp_rtc.h"

#include "stm32f10x.h"

#include <stdio.h>

#include "stm32f10x_exti.h"
#include "delay.h"

//rtc闹钟触发标志. 复习:__IO是stm一个宏, ==volatile, 表达这个变量可能被硬件中断或改变, 不允许优化和缓存偷懒, 时刻监视.
__IO uint8_t rtc_alarm_triggered = 0;



//rtc外设配置中断, 即在NVIC中注册RTC中断通道
void RTC_NVIC_Config(void)
{
	

    NVIC_InitTypeDef NVIC_InitStructure;	//NVIC配置结构体


	  EXTI_InitTypeDef EXTI_InitStructure;	//EXIT配置结构体
	    
	
	
    EXTI_ClearITPendingBit(EXTI_Line17);// 清除 EXTI Line17 的中断挂起标志
	  // 连接 EXTI Line17 到 RTC Alarm
	  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//设置 中断优先级分组策略在 不使用 HAL、完全裸机风格（比如你现在的库） 中，这个配置 是必须的，否则后面 NVIC_Init() 设置的优先级可能会被忽略或不生效。
	
		//注意这不是 RTC_IRQn 而是 RTCAlarm_IRQn）
    NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
	//享有最高优先级.
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


//初始化rtc外设.
void RTC_Configuration(void)
{
		//打开 PWR 和 BKP 外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
		//允许访问后备寄存器（BKP 寄存器）
    PWR_BackupAccessCmd(ENABLE);
	
		//复位RTC和BKP寄存器的备份域, 好习惯.
    BKP_DeInit();
		
		//启用LSE
    RCC_LSEConfig(RCC_LSE_ON);
	
		//等待LSE稳定(起振)
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
		//设置RTC的时钟源为LSE
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
		//使能RTC时钟
    RCC_RTCCLKCmd(ENABLE);

	  // 等待 RTC 寄存器同步完成（必须）
    // 因为 APB 时钟快、RTC 慢，需要同步后才允许操作 RTC
    RTC_WaitForSynchro();
		
		//等待上一条命令完成（RTC 是低速外设，需等待确认）
    RTC_WaitForLastTask();

		//使能 RTC 的闹钟中断（Alarm 中断）.RTC有三种中断来源, 用三个寄存器控制.
    RTC_ITConfig(RTC_IT_ALR, ENABLE);
		
		//再次等待上条写命令完成
    RTC_WaitForLastTask();


		//设置 RTC 的分频器，使其以 1Hz 计数
    // LSE = 32768 Hz → 32767 分频后每秒进 1（1 Hz）
    RTC_SetPrescaler(32767);
    RTC_WaitForLastTask();
}


//配置一个seconds秒后触发的闹钟.
void RTC_SetAlarmInSeconds(uint32_t seconds)
{
		//RTC_GetCounter()库函数读取当前RTC计数器的值, 即UNIX时间戳.
    uint32_t current_time = RTC_GetCounter();
	
		//RTC_SetAlarm()函数对RTC外设*设置闹钟*(请参见RTC外设产生中断的三种方式, alarm就是其中一种). 届时,RTC会产生一个中断信号.
    RTC_SetAlarm(current_time + seconds);
	
		//等待上面函数的寄存器写操作完成.
    RTC_WaitForLastTask();
}



//RTC的中断服务函数. 在it.c中转发这个函数, 实现解耦的效果.
void BSP_RTCAlarm_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
				//清除中断标志
        RTC_ClearITPendingBit(RTC_IT_ALR);
			
				//清除外部中断线
        RTC_WaitForLastTask();
			
				//设置闹钟触发变量为1

        rtc_alarm_triggered = 1;
			
		/*****************red闪烁******************************/
		//GPIO_ResetBits(GPIOB, GPIO_Pin_5);				Delay(0x0FFFFF);
		//GPIO_SetBits(GPIOB, GPIO_Pin_5);					Delay(0x0FFFFF);
		//GPIO_ResetBits(GPIOB, GPIO_Pin_5);					Delay(0x0FFFFF);
		//GPIO_SetBits(GPIOB, GPIO_Pin_5);					Delay(0x0FFFFF);
		/*****************red闪烁******************************/
						
				EXTI_ClearITPendingBit(EXTI_Line17);
    }
}

```

然后在main.c中可以这样:
```c
//2025.6.22.azazel
 //这是来自USART1接发项目文件夹的基础上进行的adc单通道中断读取项目的基础上的阈值检测.
 //的基础上的USART1_ADC_DMA.
 //的基础上的USART1_ADC_DMA_TIMled.
 //的基础上的rtc休眠
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_tim.h"
#include "bsp_led.h"
#include "bsp_rtc.h"
#include "bsp_beep.h"
#include "delay.h"

extern __IO uint16_t ADC_ConvertedValue;
extern uint8_t led_timer_count;

//局部变量.用来保存转换计算后的电压值
float ADC_ConvertedValueLocal;
#define THRESHOLD_VOLTAGE 0.05f



int main(void)
{	
	
	/****************************除了RTC外的初始化***************************************/
  USART_Config();  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
	ADCx_Init();	//初始化adc
	LED_GPIO_Config();	//初始化LED
	RED_LED_GPIO_Config();	//初始化RED LED
	BasicTIM_Init();		//初始化TIM
	BEEP_GPIO_Config();			/* BEEP GPIO 初始化 */
	/*************************************************************************/
	
	
	/****************************RTC初始化*********************************************/	
	RTC_NVIC_Config();	// RTC 中断通道配置
	RTC_Configuration();	// RTC 外设初始化（LSE、分频器、Alarm 中断使能）
	/*************************************************************************/


  while(1)
	{	
		rtc_alarm_triggered = 0; // 清标志		
		RTC_SetAlarmInSeconds(5);	//设定工作闹钟: 工作时间为5秒
		
		
		/******************************要进行的工作循环*******************************************/
		while(!rtc_alarm_triggered){
			//电压值=寄存器值/4096*3.3
			ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 

			if (ADC_ConvertedValueLocal >= THRESHOLD_VOLTAGE)//如果超过阈限
			{
					// 将 LED 置低，点亮 LED
					GPIO_ResetBits(GPIOB, GPIO_Pin_0);
					// 重置计时器为 x（ x * 中断触发频率(0.1s) = x/10s）
					led_timer_count = 5; 
				
					BEEP( ON ); 			  // 响
					Delay(0x0FFFFF);	
					BEEP( OFF );		  // 不响
					Delay(0x0FFFFF);
			} 
		/*************************************************************************/

		}
		
		/* —— 工作闹钟触发后进入这里 —— */
		rtc_alarm_triggered = 0; // 清标志
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		/* —— 设定休眠闹钟. 休眠时间为x秒 —— */
		RTC_SetAlarmInSeconds(5);
		

		

		/* —— 进入 Stop 模式，等待 RTC 中断自动唤醒 —— */
		PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

		/* —— 唤醒后要重配置系统时钟 —— */
		
		SystemInit();
		/*除了RTC外的初始化******************************************************************/
		USART_Config();  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
		ADCx_Init();	//初始化adc
		LED_GPIO_Config();	//初始化LED
		BasicTIM_Init();		//初始化TIM
		
		//亮灯表示休眠结束
		/*****************red闪烁******************************/
		int i=5;
		while(i--){
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);				Delay(0x05FFFF);
		GPIO_SetBits(GPIOB, GPIO_Pin_5);					Delay(0x05FFFF);
		}
		/*****************red闪烁******************************/
	}	
}
/*********************************************END OF FILE**********************/

```


## 8.杂项

### 8.1. 去哪里找引脚?
* 使用外设(如串口usart, ADC, i2c, spi)...引脚(绑定到哪个gpio口), 去`数据手册`里找(pinouts and pin descriptions). 注意`参考手册`里没有. 它主要是介绍外设的功能和原理图以及寄存器说明.

	比如, 我们使用adc, 先看`参考手册`:
	![alt text](image-3.png)
	现在我们想知道外部通道ADCx_IN0~ADCx_IN15这16个外部通道接哪个gpio引脚. 

	现在去查`数据手册`, 找到
	![alt text](image-5.png)


	发现文件一处错误: stm32f10xx中文说明手册，寄存器映射GPIOF基地址错了，别问我怎么知道的，这是一个痛苦的故事。


	出现flash download failed cortex-M3，然后将debug中setting中的debug中的connect设置为under reset， reset设为SYSRESETEQ后就可以了.


	编译出现4个错误20个警告的话，魔术棒target里右上选version5版本.

* stm32的引脚上电后如果不初始化, 默认是浮空输入模式. 可以无视.
* 你可以用gpio口推挽输出高低电平来驱动低功耗的外设, 但是这不规范, 最好还是用stm32的GND和3.3V两个引脚来驱动.

### 8.2. 常用英文缩写说明

-   **LDO**：Low Drop-Out regulator，低压差线性稳压器. 比DC-DC降压模块,比如buck低能, 就是纯电阻分压.
    -   工作方式：在输入和输出之间串联一个“可变电阻”（通常是一个晶体管），通过调节它的导通度来丢弃多余电压。
    -   优点：电路简单、响应快、输出噪声低。  
    -   缺点：效率低——当输入电压高于输出很多时，剩余能量全以热量形式浪费掉；尤其 ΔV（输入−输出）越大，损耗越严重。


    
-   **MCU**：Microcontroller Unit，微控制器单元
    
-   **ADC**：Analog‑to‑Digital Converter，模数转换器
    
-   **VDDA**：Voltage Digital‑to‑Analog，模拟电源输入
    
-   **VSSA**：Voltage Serial Signal Analog，模拟地
    
-   **VBUS**：USB 总线供电电压

### 8.3. 通信协议

### 3.0 通信协议的电平标准

| 电平标准                                          | 典型电压范围                 | 逻辑0电平     | 逻辑1电平     | 常见使用场景/协议                   |
| --------------------------------------------- | ---------------------- | --------- | --------- | --------------------------- |
| **TTL (Transistor-Transistor Logic)**         | 0V \~ 5V| < 0.8V    | > 2.0V    | UART（单片机）、SPI、I2C（主流MCU级通信） |
| **LVTTL (Low Voltage TTL)**                   | 0V \~ 3.3V(兼容TTL)        | < 0.8V    | > 2.0V    | 新一代 MCU/FPGA（STM32, Zynq 等） |
| **CMOS**                                      | 0V \~ Vcc（1.8V\~5V 不等）(兼容TTL) | < 0.3×Vcc | > 0.7×Vcc | SPI、I2C、数字逻辑芯片内部传输          |
| **RS-232**                                    | -12V \~ +12V（±5\~15V）  | > +3V     | < -3V     | PC串口通信（老式串口）、调试串口等          |
| **RS-485 / RS-422**                           | 差分信号 ±1.5V \~ ±5V      | 差分        | 差分        | 工业总线、485串口、多点通信             |
| **CAN Bus**                                   | 差分信号（典型2.5V为中点）        | 差分        | 差分        | 汽车通信、工业控制                   |
| **USB**                                       | 差分信号（低速3.3V）           | 差分        | 差分        | USB 1.1\~3.0 接口             |
| **Ethernet (PHY)**                            | 差分信号 ±1.0V \~ ±2.5V    | 差分        | 差分        | 局域网通信（RJ45网口）               |
| **LVDS (Low Voltage Differential Signaling)** | ±350mV（中心电压1.2V）       | 差分        | 差分        | 高速传输，LCD屏、FPGA高速接口          |
| **MIPI D-PHY / C-PHY**                        | 低电压差分，<1V              | 差分        | 差分        | 手机摄像头/屏幕的 CSI/DSI 总线        |



* RS-232电平: 15v表示0, -15v表示1. RS-232标准串口一般用于工业设备直接通信. 电平转换芯片常用的有`MAX3232`,`SP3232`.

![alt text](image-20.png)

RS-232长这样子:(九根针)

![alt text](image-21.png)

* *可以在指南者上拓展出RS232接口:*
购买一个232转TTL:

![alt text](image-23.png)

四个引脚,vcc和gnd分别接stm32提供的一个vcc和gnd即可;
rxd(收端)和txd(发端)

![alt text](image-22.png)
### 3.1 UART串口通信

#### 3.1.0 UART概述
**串口**（Serial Port），通常指的是 UART（Universal Asynchronous Receiver/Transmitter，通用异步收发器），它是一种数据传输协议。

实现起来超级简单, 就是两根线, 约定一个传输速度然后一根发送一根接收.

缺点:
* 不能远距离传输信号;
* 通信速度比spi, i2c等通信协议慢;
* 不能一对多.
  



#### 3.1.1 USART的硬件部分:

**在 STM32 和大多数单片机上，UART 通信默认使用 TTL(0v 5v) 电平.**

###### USB转UART:
实际中我们经常使用USB数据线, 想要使用UART通讯, **需要进行两个协议的转换**.

>usb协议(即universaal serial bus,通用串行总线协议), 信号传输方式是差分信号, 且需要复杂的协议通信(枚举, 握手, 数据帧...)

具体来说, stm32板子和电脑连接;

stm32指南者板子上, 信号要通过板子上的ch340芯片, 然后通过连接的usb转串口数据线. 

**引脚连接方式:(如图)**
![alt text](image-25.png)
查原理图得:(查原理图第一图, 即下方标有`stm32f103vet6`, 此为cpu的引脚. 可看到PA9分配到USART1_TX, 后者即为stm32的UART发送引脚, PA10分配到USART1_RX.)
看板子上PA9和PA10分别连接RXD, TXD, 这部分属于J11框图. 于是在原理图中搜索J11框图, 查到这是usb串口转换模块, 而TXD和RXD是芯片ch340的引脚, 它们通过两个跳帽和PA9(USART1_RX), PA10(USART1_TX)相联.
* PA9表示一个物理上的唯一的引脚. 而引脚可以复用, 抽象意义的引脚USART1_RX被分配在PA9.

![cpu原理图](image-26.png)

![J11](image-27.png)

STM32 TX（即USART1_TX, 即PA9. TTL电平） → CH340 RX(即J11板块上的RXD)
STM32 RX（即USART1_RX, 即PA10. TTL电平） → CH340 TX(即J11板块上的TXD)
STM32 GND → CH340 GND
* 用 CH340 连接 STM32 和电脑时，它在做两个转换

	* USB ⇄ UART（协议转换）
	电脑用的是 USB，但 STM32 只会 UART，CH340 充当“协议翻译官”。
	* USB ⇄ TTL（电平转换）
	电脑的 USB 是 0V / 5V，STM32 的 UART 是 3.3V，CH340 还能帮忙匹配电平。
安装ch340驱动后, windows可以认识ch340, 给它分配一个com口.
TTL转usb电平的芯片常有`ch340`.
![alt text](image-24.png)

**实际电路图:**
实际上就是ch340电路图.
![usb转串口的ch340电路图](image-49.png)

![实物图](image-50.png)

由上图, 默认ch340芯片的RXD和TXD连接的时PA9和PA10, 这两个引脚被分配在USART1的TX和RX. 所以默认使用的是USART1.

如果想要使用USART2, 可以拆掉两个跳帽, 把CH340的RXD和TXD连接到USART2外设的TX和RX上. 查表(`Table 5. High-density STM32F103xx pin definitions`)得, 
![查表结果](image-51.png)
它们是:`PA3`, `PA2`. 于是我们可以用杜邦线把RXD和TXD连接到PA2和PA3.

同理, PB10, PB11默认和wifi连接, 是USART3.






#### 3.1.2 UART的软件部分:
![alt text](image-28.png)
![alt text](image-29.png)
![alt text](image-30.png)
![alt text](image-32.png)
![alt text](image-31.png)
![alt text](image-33.png)


#####  3.1.3 UART串口通信-功能框图

##### 1.引脚部分
![alt text](image-34.png)

每个USART串口有五个常用引脚(IRDA_OUT和IRDA_IN是红外功能, 很少用):

![alt text](image-35.png)
nRTS中的n表示低电平有效.
这些引脚对应的gpio: 去查询`2-STM32F103xCDE_数据手册-英文`的`pinouts and pin descriptions, table 5`.
![alt text](image-38.png)
就这个表. high-density即高容量. 指南者就是高容量.
LQFP144指的是144脚的芯片,
指南者是LQFP100型号的.
但是前六列这些数字的意义? 是PCB封装引脚号.
例如如果我想找usart1的cts, 我应该搜索这个引脚名:`USART1_CTS`
然后搜索数据手册(`pinouts and pin descriptions, table 5`):
这是查询的结果.
![alt text](image-36.png)


* 我们可以看到stm32指南者有5个串口模块, 其中uart1挂载在总线APB2(72M时钟)上,比较快. 剩下四个挂载在APB1总线(36M时钟).  同时也要注意写程序的时候记得打开总线时钟.

* 前三个是`USART`, 即通用同步/异步收发器. 后两个是`UART`,即通用异步收发器. 少了`SCLK`, `CTS`, `RTS`三个引脚.

##### 2.数据寄存器部分
这部分自己看寄存器文档即可.

`USART_DR`. 9位有效. 
![alt text](image-41.png)
![USART_DR寄存器](image-42.png)

由USART_CR1的M位决定具体用几位.
![USART_CR1寄存器](image-43.png)



停止位由USART_CR2寄存器的相关位配置:
![USART_CR2寄存器](image-44.png)
最常用:1个停止位.


想要使用usart, 首先UE(usart enable, 在CR1寄存器的13位)要置1. 如果接收, RE(也在CR1寄存器)=1. 同理发送需要TE=1.

* SR寄存器:存储状态用的. 

![TXE和TC](image-45.png)


`RXNE`: 读数据寄存器非空. 该位表示:
![alt text](image-46.png)

`TXEIE`: 

`USART_BRR`是波特率寄存器.
![USART_BRR](image-47.png)


![alt text](image-48.png)

##### 3.1.4 USART编程
stm32f10x_usart.h.

### 3.2 I2C协议


就像SPI解决了UART因为没有CLK线传输导致不稳定和很慢的问题一样,
I2C的思想是通过`总线`的提出, 解决UART无法和一对多通信问题. 当然, `总线`结构比UART, SPI要复杂得多.

I2C有两条线, 主机连出一条clk(称为**SCK, serial clock**)一条data(**SDA, serial data总线**). 每个从机都挂载在这两根线上(如下图). 每个从机有独立ID, 主机通过选定ID来实现和想要通信的从机通信.

---

UART无法一对多通信. 每个外设都需要占用MCU一个UART接口.
![alt text](image-134.png)
而I2C可以一对多通信.
![alt text](image-135.png)

---

* 串行通信的意思: 实际数据是一位一位地串行传输. UART, I2C, SPI都是串行通信. 虽然I2C的总线结构从电路上来说, 从机是"并联"在SLK和SDA上的.

### 3.3 SWD(serial wire debug)

SWD是arm公司开发的专门为**调试**和**烧录** MCU 设计的 通信协议.

🔍 和 UART/I2C/SPI 的区别是什么？

| 项目       | **SWD**                        | **UART/I2C/SPI**                            |
| -------- | ------------------------------ | ------------------------------------------- |
| 📌 目的    | 💻 调试、单步运行、烧录程序                | 📡 数据通信（传感器、模块、MCU之间）                       |
| 📎 引脚    | SWDIO（双向数据）、SWCLK（时钟）          | UART: TX/RX；I2C: SDA/SCL；SPI: MISO/MOSI/SCK |
| 📦 通信角色  | 主机 = ST-Link<br>从机 = STM32 MCU | 多为 MCU 与外设模块通信                              |
| 💡 通信模式  | 串行调试协议（带握手、寄存器访问等）             | 简单收发数据（无调试概念）                               |
| 🛠️ 控制功能 | 调试寄存器、查看内存、设置断点、复位等            | 通常只是收发用户数据                                  |
| 🧱 使用范围  | 仅用于支持 SWD 的 ARM MCU            | 几乎所有嵌入式设备                                   |
| 📂 协议复杂度 | 较高，包含协议层、AP、DP 等内部机制           | 相对简单（尤其 UART）                               |

SWD 是 “调试员”和 MCU 之间的远程控制协议，能“偷看” MCU 大脑里的内容。

UART/I2C/SPI 是 “两个普通人”之间的信息交流协议，他们只管互相传消息。

### 8.4. 施密特触发器
 

施密特触发器是比较器的负反馈改进(加了一个反馈电阻). 它用于将模拟信号转换为数字信号.

直接比较器也可以做这个活:

![alt text](image-139.png)

但是不这样做, 原因: 输入电压不一定是平滑的, 如果有锯齿/抖动, 则输出的信号会是多次高低电平:

![alt text](image-140.png)

为了防抖动, 对比较器加入一个负反馈电阻, 就成了**迟滞比较器/施密特触发器**.

![alt text](image-141.png)

#### 施密特触发器的电路分析:

我们从初始时输入0V开始分析. 首先此时比较器正极电压即`R1`和`R2`分压后`R2`的分压, 它高于0V, 所以比较器输出VCC高电平. 我们称这个电压为**低参考电压**(马上会知道为什麽)

![alt text](image-142.png)

然后分析反馈的效果: 此时R1和R3并联然后串联R2, 所以此时比较器实际的正极电压即`R1//R3`和`R2`分压后`R2`的分压. 我们称其为**高参考电压**(马上会知道为什麽)

![alt text](image-143.png)

如果此时输入慢慢增大, 直到超过上述**高参考电压**电压后, 比较器将会输出低电平. 此时又回到R1和R2分压了. 比较器正极电压会变为**低参考电压**.

此时, 即使输入发生抖动, 但只要不低于**低参考电压**. 比较器都不会反应.

然后正如一开始的过程, 当输入从小变大的时候, 只要不高于**高参考电压**, 比较器都不会反应. 于是迟滞比较器实现了!


### 8.5. status和state区别?

| 单词       | 意思                                    | 适用场景（常见）                                  |
| -------- | ------------------------------------- | ----------------------------------------- |
| `stat`   | statistic/statistics 的缩写，或 status 的缩写 | 通常用作简写变量名（如 `DSTATUS stat;`）表示当前设备状态      |
| `status` | 当前状态/状态码（返回值常用）                       | ✅ 函数返回状态（成功/失败/错误码）最常用的词                  |
| `state`  | 状态（偏向“内部状态机”的状态）                      | 状态机 / 模块的状态值，常用于逻辑控制，比如 `"idle"`、`"busy"` |




### 8.6. 一些概念

#### 8.6.1**HID(human interface device)**, 人机接口设备. 是一种usb接口的设备.
名字的意思是用来和人交互的设备, 比如键盘/鼠标/手柄/触摸屏...但是一些调试工具比如CMSIS-DAP调试器也是HID.
* 特点: 即插即用(不需要安装驱动!)(封装了report机制)
* 传输协议简单, 即USB协议.

### 7.

### 8. STM的启动文件

`startup_stm32f10x_hd.s`是stm的启动文件. 使用ARM汇编语言**Assembly**.

一般在项目的`.\STARTUP\`下.

#### 8.1 栈空间

在`startup_stm32f10x_hd.s`中可以修改栈空间.
栈的作用是保存程序运行的时候产生的局部变量. 如果程序很臃肿或者要调用很多复杂的大型图像/字库资料, 或者涉及到递归, 应该改大一点防止运行的时候爆栈.

```assembly
Stack_Size      EQU     0x00000400  ;当前设置为1KB
```




## 9 SD卡

注意: `SD总线协议(简称SD)`和USB协议一样, **区分主机(控制器)和从机**.
F10X系列只有大容量才有SDIO外设.

指南者板子(F103VET6)搭载了SDIO外设, 可以和SD卡交互.

核心板F103C8T6没有.


* `SDIO协议`: 是扩展自`SD总线协议`的扩展标准, 它允许主机通过 `SD卡接口` 访问非存储类外设, 比如SDIO-wifi芯片之类的. *SDIO协议在电气和时序上与SD存储协议兼容，但增加了对非存储设备的命令集。*

* stm32的SD接口外设为SDIO, 所以同时支持SD卡交互和SDIO外设交互.

### 9.0 常见的储存设备


| 名称                       | 通信协议| 常见容量范围        | 说明                                                          |
| ------------------------ | ---------------- | ------------- | ----------------------------------------------------------- |
| **TF卡（micro SD）**        | SD/SPI  | 1GB ～ 1TB+    | 又叫 microSD 卡，最初命名为 TransFlash，后被纳入 SD 协会标准。可通过卡套转换为 SD 大卡。  |
| **SD卡**                  | SD/SPI  | 1GB ～ 2TB     | 标准尺寸 SD 卡，也叫“大卡”。支持 SPI/SD 通信。                            |
| **CFE A 卡（CFexpress A）** | PCIe/NVMe 协议   | 64GB ～ 1TB+   | 专业高速卡，采用 PCIe 接口，NVMe 协议，适用于高端相机（如 Sony A7S III），尺寸小、速度快。   |
| **CFE B 卡（CFexpress B）** | PCIe/NVMe 协议   | 128GB ～ 4TB+  | 更大更快的版本，常用于 Canon R5/R3、Nikon Z9 等高端设备。                     |
| **SPI Flash**            | SPI    | 512KB ～ 256MB | 片上 NOR Flash，MCU 程序/网页/固件存储用，常见如 W25Q64、GD25Q128。由主控自行读写管理。 |
| **EEPROM**               | 常为 I²C（部分 SPI） | 128B ～ 512KB  | 电可擦写存储器，适合少量数据频繁写入（如配置/参数保存）。常见如 AT24C32、CAT24C256。         |

* 
  -   **SD 卡**像是“U盘”一样，**自己带控制器、地址管理、写保护、缓存**；   
  -   **SPI Flash**像是“裸硬盘芯片”，**由你自己决定怎么存数据、怎么读块**。

* SDIO卡: 指的是可以使用SDIO协议(比如其中9个I/O口可以用作UHS-I协议的SDIO通讯, 或者其中17个I/O口可以用作UHS-II协议的SDIO通讯)的外设芯片. 注意指的是外设芯片, 而不是存储设备. 比如一块wifi芯片.

* 
	**SPI Flash 是一种使用 SPI 接口的 NOR 型闪存**；  
	**SD 卡内部通常是 NAND 闪存结构**，由控制器管理读写。


#### 9.0.1啥是NOR闪存/NAND闪存?

首先它们都是非易失性
| 类型             | 名字来源                     		| 是否直接由某种门电路构成 |
| -------------- | ------------------------ 			| ------------ |
| **NAND Flash** | 由大量 **NAND 结构的晶体管阵列** 组成 | ✅ 是的         |
| **NOR Flash**  | 由大量 **NOR 结构的晶体管阵列** 组成  | ✅ 是的         |


---------------------------
##### NAND
---------------------------
![alt text](image-94.png)

- NAND 串行访问结构架构. 擦除得更快, 但是读取很慢. 是以块为单位读取/写入的(一般是512bit).
- 很便宜而且储存密度大.
- 适用在储存卡, 硬盘...
-   **单元连接方式**：
      -   多个存储单元（通常32-64个）**串联成一条链（NAND String）**，共享位线。          
      -   必须**逐页（Page）访问**（典型页大小4KB-16KB），无法直接读取单个单元。         
      -   **结构类似硬盘**，按块/页管理。      
-   **读取原理**：    
    -   需要先选中整页数据，再通过串行接口逐位输出。        
    -   **随机读取延迟高**（~25-50μs），但连续读取吞吐量高（因页缓存机制）。

------------
* NAND微观结构:
 
![alt text](image-116.png)

NAND闪存用**浮栅晶体管**串联制作. 这种晶体管是改进版的MOSFET, 相比MOSFET多了两层: `浮栅层`和`绝缘层`, 实现了非易失性存储.

* 浮栅晶体管结构(NMOS):

![alt text](image-114.png)


* **读取原理**: G,D极通阈值电压, 有S->D电流就是1.
  * **浮栅晶体管的`浮栅`内存储“0”和“1”的定义**
  -   **逻辑“0”**：浮栅中存储**大量电子**（负电荷）。 
  -   **逻辑“1”**：浮栅中**电子较少或无电子**（中性或正电荷）。

  * **`浮栅`结构存储的电荷对MOSFET的沟道产生影响:**
  -   **浮栅存储电子（逻辑“0”）**：
      -   电子在浮栅上形成**负电场**，会**排斥**沟道中的电子（N-MOSFET 的载流子是电子）。       
      -   相当于**抬高了沟道的阈值电压（Vth）**，需更高栅压才能开启沟道。      
      -   若施加**标准阈值电压（Vth\_normal）**，沟道仍处于**关闭状态**（无电流）。      
  -   **浮栅无电子（逻辑“1”）**：
      -   浮栅无额外负电荷，沟道阈值电压**保持设计值（Vth\_normal）**。       
      -   施加阈值电压时，沟道**正常导通**，产生电流。

* **写入原理**: 高压电长迫使电子穿过绝缘层实现.
  * ***(1) 热电子注入（NOR Flash常用）**
    -   **电压配置**：   
        -   **控制栅（G）**：高压（~10V），吸引电子。      
        -   **漏极（D）**：中压（~5V），形成强横向电场。      
        -   **源极（S）**：接地（0V）。       
    -   **物理过程**：  
        1.  沟道中的电子在高压下获得高能量（“热电子”）。    
        2.  热电子越过绝缘层势垒（SiO₂），被浮栅捕获。     
    -   **特点**：
        -   速度快，但功耗高（需大电流）。    
        -   主要用于NOR Flash的字节级写入。       
  * **(2) Fowler-Nordheim隧穿（NAND Flash常用）**
    -   **电压配置**：
        -   **控制栅（G）**：高压（~15-20V）。      
        -   **源/漏极**：接地或浮空。     
    -   **物理过程**：  
        1.  强垂直电场使电子直接隧穿绝缘层（量子隧穿效应）。     
        2.  电子被浮栅捕获。      
    -   **特点**：  
        -   功耗低（无需大电流），但速度较慢。     
        -   适用于NAND Flash的页写入（一次写多字节）。


* 格式化: 格式化闪存即全部写1, 释放所有浮栅内的电子.
----------------------------
---------------------------
##### NOR
---------------------------

![alt text](image-93.png)

- NOR 并行访问架构, addresses the entire memory range, 所以可以访问其中任意一个字节, 所以读取非常快; 

- 但是NOR逻辑门单元更大(CMOS![alt text](image-95.png)), 也更昂贵.
- NOR首次启动需要更大的功率, 但是启动之后功耗会小于NAND.
- 适合高速随机读取.
- 适用在计算芯片的内存.
-   **单元连接方式**：
    -   每个存储单元（Memory Cell）**直接并联到位线（Bit Line）**，类似“独立开关”。
    -   通过**字线（Word Line）**选中一行单元，所有位线可同时被读取（并行访问）。
    -   **结构类似SRAM**，支持随机寻址。   
-   **读取原理**：
    -   直接通过位线检测电流（无需整页读取）。       
    -   **随机读取延迟低**（~50-100ns），适合代码执行（XIP, Execute-In-Place）。





---------------------------

存储卡的速度指标:

| 指标         | 说明                    | 主要影响的应用场景             | 举例                                  |
| ---------- | --------------------- | --------------------- | ----------------------------------- |
| **顺序写入速度** | 连续写入大块数据的速度（如写大文件）    | **视频录制、数据采集、单个大文件备份存储, 下载游戏内大型资源**    | 行车记录仪、安防录像、STM32采集传感器数据并写文件         |
| **顺序读取速度** | 连续读取大块数据的速度（如读大文件）    | **媒体播放、系统启动、读取单个大文件, 玩游戏** | 播放高清视频、读取音频文件、加载游戏大地图               |
| **随机写入速度** | 在随机地址写入小数据块的能力（4KB以下） | **数据库、小文件日志写入**       | Android App 写配置、STM32日志写入FATFS多个小文件 |
| **随机读取速度** | 从随机地址读取小数据块的能力（4KB以下） | **操作系统响应, 索引检索, 玩游戏** | 启动系统、读取配置、打开一堆小图片或文档                |


一般情况下, 顺序读取 > 顺序写入 >> 随机读取 > 随机写入. 比如一张普通质量的TF卡:

| 类型        | 典型速度范围（MB/s）    |
| --------- | --------------- |
| 顺序读取      | 60–100+ MB/s（快） |
| 顺序写入      | 20–80 MB/s（略慢）  |
| 随机读取（4KB） | 5–15 MB/s（小文件）  |
| 随机写入（4KB） | 0.5–5 MB/s（最慢）  |


一般标在表面的是顺序读取速度. 而且是极端完美工况下.

最低速度标识:

![alt text](image-90.png)

![alt text](image-91.png)

![alt text](image-92.png)


### 9.1 SD卡的标识

| 标识               | 含义                        | 举例         | 是否代表速度？     |
| ---------------- | ------------------------- | ---------- | ----------- |
| **UHS-I/II/III** | 总线标准. UHS-II协议通过17个引脚, 而UHS-I只有9个引脚. UHS-II卡可兼容UHS-I卡槽, 但降速运行.| 决定理论带宽     | ✅ 是         |
| **U1/U3**        | 最小顺序写入速度（10MB/s / 30MB/s） | 写视频时的最低保证  | ✅ 是         |
| **V30/V60/V90**  | 视频速度等级（最低写入速率）            | 适合视频录制     | ✅ 是         |
| **A1/A2**        | App 性能等级（随机 IOPS）         | 适合运行应用或多任务 | ✅ 是（但是随机性能） |


* 举个例子说明：

	你买了一张：

	-   **UHS-II / U3 / V60 / A2** 的 SD 卡；
		
	-   插入普通 UHS-I 相机中使用；
		
	-   结果：   
		-   它只能运行在 UHS-I 速度；        
		-   最大读取速度 ≈ 90 MB/s；        
		-   但它仍能保证 **V60：最小写入 60 MB/s**，适合录 4K 视频；        
		-   App 加载快，因为是 A2 卡。


* 因为尺寸太小, 没有办法容纳第二排UHS-II接口引脚, 所以**所有的micro SD(TF)卡都最多只支持UHS-I**, 只有9个针脚"金手指". 具体引脚说明见9.4 SD卡结构.


#### 9.1.3 读写速度测试?

CDM(crystal disk mark)软件测速

### 9.2 SD卡通讯的最简单的协议: SPI协议



SPI模式也称为SD卡的兼容模式. UHS-I和UHS-II协议都向下支持, 只使用四根引脚通信:
| 名称         | 说明         |
| ---------- | ---------- |
| DAT0–DAT3  | 数据线（4-bit） |
| CMD        | 命令线        |
| CLK        | 时钟线        |
| VCC/GND/CD | 电源、地、检测    |

而SDIO模式是更高速的协议, UHS-I和UHS-II分别使用9个和17个引脚实现(后者多了一排并行所以更快).


### 9.3 SD卡容量

不同容量的SD卡支持的SD协议版本不同. 也就是说SPI协议下工作都一样, 但是SDIO协议不同, 表现不一样.

| 类型       | 全称                           | 容量范围        | 文件系统    | 指令集/协议 	 | 标识            |
| -------- | -------------------------------- | ------------- | ----------- | ------- 		| ------------- |
| **SDSC** | Secure Digital Standard Capacity | ≤ 2 GB        | FAT12/FAT16 | SD v1.x 				| 通常写 “SD”      |
| **SDHC** | Secure Digital High Capacity     | 4 GB ～ 32 GB  | FAT32       | SD v2.0 				| 带 “HC” 标志     |
| **SDXC** | Secure Digital eXtended Capacity | 64 GB ～ 2 TB  | exFAT       | SD **v3.0** 				| 带 “XC” 标志     |

stm32F4系列只能支持SD2.0协议, 也就是说如果用SDIO协议, 只能选择SDSC, SDHC卡.

### 9.4 SD卡物理结构

![alt text](image-96.png)


![alt text](image-97.png)

其中, 图中的`控制单元`和`存储单元接口`之间有一个FIFO数据缓冲器. 

#### 9.4.1.SD卡的寄存器结构:

| 名称      | 位宽  | 中文说明                                    | 通俗解释                                                            |
| ------- | --- | --------------------------------------- | --------------------------------------------------------------- |
| **CID** | 128 | 卡识别号 (Card Identification Number)       | **卡的身份证**。包含厂商ID、产品名、生产日期等。**唯一标识一张卡**。主机可以用它识别不同卡。             |
| **RCA** | 16  | 相对地址 (Relative Card Address)            | 主机分配给SD卡的“通信地址”。多卡系统中每张卡靠它来区分。初始化时主机设置。                         |
| **DSR** | 16  | 驱动器寄存器 (Driver Stage Register)          | 用于配置卡的输出驱动强度，**可选寄存器**，实际中**很少使用**。                             |
| **CSD** | 128 | 卡特定数据 (Card Specific Data)              | 包含 SD 卡的**容量、读写速度、块大小、最大电流**等信息，是**主机最重要的参考信息之一**。              |
| **SCR** | 64  | SD配置寄存器 (SD Configuration Register)     | 说明 SD 卡的**版本、总线宽度支持情况（1-bit / 4-bit）、命令支持情况等**。主机用它来判断能否启用4线模式。 |
| **OCR** | 32  | 操作条件寄存器 (Operation Conditions Register) | 包含 SD 卡的**电压范围支持信息**，以及卡是否准备好。初始化阶段主机会不断读取这个寄存器来判断卡是否ready。     |
| **SSR** | 512 | SD状态 (SD Status)                        | 包含更详细的卡运行状态，比如**写保护、擦除单元大小、当前速度模式等**。                           |
| **CSR** | 32  | 卡状态 (Card Status)                       | 是主机每次发送命令（CMD）后 SD 卡反馈的“简要状态码”，包含是否出错、是否就绪等。可以理解为“最近一次操作的执行结果”。 |

![alt text](image-98.png)

#### 9.4.2 SD总线协议:

>请注意区分. 这里的**SD总线协议**指的是**标准SD协议**. 

>**SDIO协议**是它的超集. 但是它们共享物理层, 讨论原理和物理结构的时候几乎是一回事(SDIO有更丰富的指令集而已)


SD协议使用9条数据线:

| 引脚名           | 方向     | 功能                 |
| ------------- | ------ | ------------------ |
| **CLK**       | 主机 → 卡 | 时钟线                |
| **CMD**       | 双向, 半双工 		| 命令/响应线             |
| **DAT0~3**    | **4根并行数据线**. 双向, 单工但方向可变(这严格称为**时分双向**)     | 数据线                |
| **VDD, VSS (GND)**      | -      | 电源（通常 3.3V）和地        |
| **CD/DAT3**   | 双用     | 卡检测/CD 线（低电平表示卡插入）(DAT3也可复用为卡检测输入) |



* 刚上电时, 因为除了VDD和VSS, 只用DAT0进行主机和SD卡之间的交互. 所以交互是1bit位宽.

* 主机通过 **CMD0 → CMD8 → ACMD41** 等初始化流程确认卡类型（SD1.0/SD2.0/SDHC...）后, 可以通过 **CMD55 + ACMD6** 来告诉 SD 卡切换到 **4-bit 模式**, 使用DAT0~3四条线.


* 使用4bit模式时，每根数据线都必须有**起始位**、**终止位**以及**CRC位**，CRC位每根数据线都要分别检查，并把检查结果汇总然后在数据传输完后通过D0线反馈给主机。

* SD卡数据包有两种格式，一种是常规数据(8bit宽)，它先发低字节再发高字节，而每个字节则是先发高位再发低位，4线传输示意如下图：

![alt text](image-163.png)

![alt text](image-102.png)
  - 起始位: 4根线同时传输一个`0`
  - 终止位: 4根线同时传输一个`1`
- SD宽数据包


#### 9.4.3 SD协议 使用的9个数据线(引脚):

前面已经介绍了SD协议的九条数据线:

CLK, CMD, DAT0~3, VDD&GND, CD, 复用DAT3




![引脚](image-99.png)



* 在stm32上的对应引脚: (你也可以自己去`中文手册`那个什麽`pinout & discription table5`查)

| **SD卡引脚** | **引脚功能**       | **STM32F103V 引脚** | **备注**                             |
|--------------|--------------------|---------------------|--------------------------------------|
| 1            | CD/DAT3            | PC11 (SDIO_D3)      | 数据线3，兼作卡检测（可选）         |
| 2            | CMD                | PD2 (SDIO_CMD)      | 命令/响应线                         |
| 3            | VSS1 (GND)         | GND                 | 接地，需连接至开发板GND             |
| 4            | VDD (3.3V)         | 3.3V                | 供电，接开发板3.3V电源              |
| 5            | CLK                | PC12 (SDIO_SCK)     | 时钟信号                            |
| 6            | VSS2 (GND)         | GND                 | 接地（标准SD卡有双GND引脚）         |
| 7            | DAT0               | PC8 (SDIO_D0)       | 数据线0（必需）                     |
| 8            | DAT1               | PC9 (SDIO_D1)       | 数据线1（4位模式需连接）            |
| 9            | DAT2               | PC10 (SDIO_D2)      | 数据线2（4位模式需连接）            |


### 9.5 SD的CMD线: `命令` 和 `响应`


`SD命令`由主机发出. `响应`则由SD卡发出.

它们通过`CMD`引脚半双工通信.

 [常规命令]分为四种命令.
| 缩写       | 全称                              | 特点                                    |
| -------- | ------------------------------- | ------------------------------------- |
| **bc**   | Broadcast Command               | 广播命令，**所有卡都接收**，**无响应**（如 CMD0）       |
| **bcr**  | Broadcast Command with Response | 广播命令，所有卡接收，但主机**会等待响应**（如 CMD1）       |
| **ac**   | Addressed Command               | **指定某张卡**（通过 RCA 地址），主机会等待响应（如 CMD17） |
| **adtc** | Addressed Data Transfer Command | 指定卡 + **带有数据传输**（如 CMD18、CMD24 等）     |


![alt text](image-111.png)


- **命令格式**:  ![alt text](image-103.png)
- ![alt text](image-107.png)
  - ![alt text](image-108.png)



#### 9.5.1 SD命令的命令号部分

可以看到SD命令用6bit表示`命令号`. 6bit可以表示2^6=64条命令, 即`CMD0`~`CMD63`.

它们是:


🧾 标准命令（CMDx）列表（部分保留命令会标明）
| CMD编号 | 名称                     | 功能说明                       |
| ----- | ---------------------- | -------------------------- |
| CMD0  | GO\_IDLE\_STATE        | 软件复位卡，进入 idle 状态（上电后第一条命令） |
| CMD1  | SEND\_OP\_COND         | 启动卡初始化（MMC专用，SD不用）         |
| CMD2  | ALL\_SEND\_CID         | 请求卡的CID号（卡唯一识别号）           |
| CMD3  | SEND\_RELATIVE\_ADDR   | 设置/获取 RCA（卡相对地址）           |
| CMD4  | SET\_DSR               | 设置 DSR 寄存器（很少用）            |
| CMD5  | IO\_SEND\_OP\_COND     | SDIO专用，初始化IO卡              |
| CMD6  | SWITCH\_FUNC           | 切换函数（高速模式等）或读取支持函数         |
| CMD7  | SELECT/DESELECT\_CARD  | 选择/取消选择卡                   |
| CMD8  | SEND\_IF\_COND         | 检测电压范围、确认是否支持SDHC/SDXC     |
| CMD9  | SEND\_CSD              | 读取CSD寄存器                   |
| CMD10 | SEND\_CID              | 读取CID寄存器                   |
| CMD11 | READ\_DAT\_UNTIL\_STOP | （保留）                       |
| CMD12 | STOP\_TRANSMISSION     | 停止多块数据传输                   |
| CMD13 | SEND\_STATUS           | 获取卡当前状态                    |
| CMD14 | BUSTEST\_R             | 总线测试（保留）                   |
| CMD15 | GO\_INACTIVE\_STATE    | 让卡进入“休眠”状态，断开总线            |




📤 数据传输相关命令：
| CMD编号 | 名称                      | 功能说明              |
| ----- | ----------------------- | ----------------- |
| CMD16 | SET\_BLOCKLEN           | 设置数据块长度（标准为512字节） |
| CMD17 | READ\_SINGLE\_BLOCK     | 读取单个数据块           |
| CMD18 | READ\_MULTIPLE\_BLOCK   | 读取多个数据块           |
| CMD19 | BUSTEST\_W              | 总线测试（保留）          |
| CMD20 | WRITE\_DAT\_UNTIL\_STOP | （保留）              |



📥 写入相关命令：
| CMD编号 | 名称                     | 功能说明       |
| ----- | ---------------------- | ---------- |
| CMD24 | WRITE\_BLOCK           | 写入单块       |
| CMD25 | WRITE\_MULTIPLE\_BLOCK | 写入多块       |
| CMD26 | PROGRAM\_CID           | 编程CID（只一次） |
| CMD27 | PROGRAM\_CSD           | 编程CSD（只一次） |

⛔ 擦除、写保护等：
| CMD编号 | 名称                          | 功能说明               |
| ----- | --------------------------- | ------------------ |
| CMD28 | SET\_WRITE\_PROT            | 设置写保护（硬件）          |
| CMD29 | CLR\_WRITE\_PROT            | 清除写保护              |
| CMD30 | SEND\_WRITE\_PROT           | 查询是否写保护            |
| CMD32 | ERASE\_WR\_BLK\_START\_ADDR | 设置擦除开始地址           |
| CMD33 | ERASE\_WR\_BLK\_END\_ADDR   | 设置擦除结束地址           |
| CMD38 | ERASE                       | 执行擦除命令（搭配CMD32/33） |



🧾 应用命令（ACMDxx）列表（需先发送 CMD55）：
| ACMD编号 | 名称                         | 功能说明                     |
| ------ | -------------------------- | ------------------------ |
| ACMD6  | SET\_BUS\_WIDTH            | 设置总线宽度（1-bit / 4-bit）    |
| ACMD13 | SD\_STATUS                 | 读取SD状态寄存器（512bit）        |
| ACMD22 | SEND\_NUM\_WR\_BLOCKS      | 返回最后一次写入的块数              |
| ACMD23 | SET\_WR\_BLK\_ERASE\_COUNT | 设置一次擦除的块数                |
| ACMD41 | SD\_SEND\_OP\_COND         | SD卡初始化命令，检测电压范围，卡是否ready |
| ACMD42 | SET\_CLR\_CARD\_DETECT     | 控制pull-up电阻使能            |
| ACMD51 | SEND\_SCR                  | 读取SCR寄存器（卡支持的特性）         |

![alt text](image-110.png)


例如, `CMD0`是一个`bc`(boardcast)命令, 所有卡都接受且无`响应`. 它的内容是:
- 起始位: 0(1bit, 固定为0)
- 传输位: 1(1bit, 表示方向为主机->SD卡从机)
- 命令号: 0(6bit, 0号命令0x0即000000)
- 参数: 0(32bit, CMD0不需要参数默认都为0)
- CRC7校验位: 1001010(CRC7校验位是对前面加起来40bit进行运算(略)得到的.)
- 停止位: 1(1bit, 固定为1)

合起来就是一个48位SD命令帧:
`0 1 000000 00000000000000000000000000000000 1001010 1`
十六进制表示就是`0x40 0x00 0x00 0x00 0x00 0x95`



#### 9.5.2 SD命令的参数部分 

SD命令有32bit用来表达命令参数. 包括块地址、扇区编号、偏移量、标志位、状态配置值等.

注意SD卡访问的地址是`块地址`, 即扇区编号. 每块512B.

#### 9.5.3 SD响应

响应由SD卡向主机发出. 部分命令要求SD卡作出响应.

| 响应类型 | 位宽      | 用途说明                      |
| ---- | ------- | ------------------------- |
| R1   | 48 bit  | 卡状态响应（最常见）                |
| R1b  | 48 bit  | R1 + 卡忙信号（需等待）            |
| R2   | 136 bit | 读取CID/CSD寄存器              |
| R3   | 48 bit  | OCR（电压条件）响应               |
| R4   | 48 bit  | **未在 SD 卡中使用. 用于 SDIO.** |
| R5   | 48 bit  | **未在 SD 卡中使用. 用于 SDIO.**       |
| R6   | 48 bit  | RCA 地址响应（如 CMD3）          |
| R7   | 48 bit  | 电压检测和接口条件（CMD8）           |

![alt text](image-112.png)
![alt text](image-113.png)


### 9.6 SD卡的命令层: 两种`操作模式`: `卡识别模式` 和 `数据传输模式`, CPSM

SD卡有两种操作模式.

-   **SD卡初始化模式（Identification Mode）**
    
    -   在复位（CMD0）后，SD卡进入**初始化模式**，此时通信使用**低速时钟（通常400kHz以下）**。
        
    -   主机发送 **CMD8（Send Interface Condition）** 检查卡支持的电压范围。
        
    -   接着发送 **ACMD41（SD\_SEND\_OP\_COND）** 进行初始化，并协商工作电压和主机能力（如高容量支持）。
        
    -   在初始化完成后，SD卡会分配一个**相对地址（RCA, Relative Card Address）**，并进入**数据传输模式**。
        
-   **SD卡数据传输模式（Data Transfer Mode）**
    
    -   初始化完成后，SD卡进入**数据传输模式**，此时可以使用**高速时钟（最高可达208MHz，取决于SD卡版本）**。
        
    -   主机通过 **CMD7（Select/Deselect Card）** 选择某张卡（使用RCA），使其进入**传输状态（Transfer State）**。
        
    -   在该模式下，主机可以执行读写操作（如 **CMD17/18/24/25** 等）访问存储数据。


事实上, 两种操作模式下还有不同的`状态`.

状态转换图: (即一个**状态机)**

![alt text](image-117.png)

负责实现这个状态转换的逻辑电路模块即称为**CPSM, Command Path State Machine, 命令路径状态机**. SD卡的状态改变由两部分组成:
* 一部分是**CPDM**. 它只管发命令、等待响应. 它MCU内部的`SDIO外设`的一部分, 存在在MCU芯片内部. 
* 一部分是SD卡的**内部状态机**逻辑电路. **CPDM**通过`CMD`线向**内部状态机**发送命令, 实现改变SD卡的状态. 



---

 **表1：初始化模式（Identification Mode）下的状态**
| **状态**              | **触发条件**               | **关键命令**                                                                 | **说明**                                                                 |
|-----------------------|---------------------------|-----------------------------------------------------------------------------|-------------------------------------------------------------------------|
| **Idle State**        | 上电或收到 `CMD0`          | `CMD0`（强制复位）、`CMD8`（检查电压）、`ACMD41`（初始化）                   | 卡未激活，仅响应少数命令。                                               |
| **Ready State**       | `ACMD41` 初始化成功        | `CMD2`（读取CID）、`CMD3`（分配RCA）                                        | 卡已准备好被识别，等待CID读取和RCA分配。                                 |
| **Identification State** | 收到 `CMD3`（分配RCA）   | `CMD7`（选择卡，进入传输模式）                                              | 卡已获得RCA，但未进入数据传输模式。                                      |

---

 **表2：数据传输模式（Data Transfer Mode）下的状态**
| **状态**              | **触发条件**               | **关键命令**                                                                 | **说明**                                                                 |
|-----------------------|---------------------------|-----------------------------------------------------------------------------|-------------------------------------------------------------------------|
| **Stand-by State**    | 收到 `CMD7`（选择卡）      | `CMD7`（取消选择）、`CMD9`（读取CSD）、`CMD16`（设置块大小，仅SDSC有效）     | 卡已被选择，但未进行数据传输。                                           |
| **Transfer State**    | 从 Stand-by 进入           | `CMD17/18`（读单块/多块）、`CMD24/25`（写单块/多块）、`CMD12`（终止传输）    | 卡可执行读写操作，正常状态。                                             |
| **Sending-data State** | 收到 `CMD17/18`（读数据） | 无额外命令（自动返回Transfer State）                                         | 卡正在向主机发送数据。                                                   |
| **Receive-data State** | 收到 `CMD24/25`（写数据） | 无额外命令（自动返回Transfer State）                                         | 卡正在接收主机数据。                                                     |
| **Programming State** | 数据写入闪存（如 `CMD24`） | `CMD13`（查询状态，检查`BUSY`位）                                            | 卡正在编程（写入数据），可能不响应其他命令。                             |
| **Disconnect State**  | `CMD7`（RCA=0）取消选择    | `CMD7`（重新选择卡）                                                         | 卡被临时取消选择，可快速恢复。                                           |


* `Transfer State`是`Sending-data State`和`Receive-data State`的前置, 后两者可以看成`Transfer State`的子状态.


### 9.7 SD卡的数据传输层, DPSM

数据传输层的运作被类似`CPSM`地称为`DPSM`. 这个电路模块是`SDIO外设`的一部分, 存在在MCU芯片内部.

写入过程:

* 用户调用库函数写入命令(比如`f-write()`)-> 
* MCU 的 CPSM 发出写命令 → 
* SD 卡状态机准备接收 → 
* MCU 配置 DPSM 并写入 FIFO → DPSM 控制 FIFO 数据通过 DAT 线发出 → 
* SD 卡缓存接收数据 → 
* SD 卡内部状态机控制写入 Flash。

* **FIFO(first in first out)**

FIFO是一个带有寄存器和简单控制模块的外设, 是MCU内部SDIO外设的一部分. 

SD 卡的数据 FIFO 是其内部控制器用于缓冲数据的“队列”，
主要目的是：协调主控和 NAND 之间的数据速率、减小延迟、提高效率。

数据FIFO(先进先出)部件是一个数据缓冲器，带发送和接收单元。
控制器的FIFO包含宽度为32bit、深度为32字的数据缓冲器和发送/接收逻辑。
SDIO状态寄存器(SDIO_ STA)的TXACT位用于指示当前正在发送
数据，RXACT位指示当前正在接收数据，这两个位不可能同时为1。
●当TXACT为1时， 可以通过AHB接口将数据写入到传输FIFO。
●当RXACT为1时， 接收FIFO存放从数据路径部件接收到的数据。




### 9.8 SD卡的寻址

#### **1. SDSC（标准容量卡，≤ 2GB）—— 以字节（Byte）为单位寻址**

-   **物理寻址方式**：SDSC 使用 **字节寻址（Byte Addressing）**，即主机可以指定任意字节偏移量（32位地址，最大支持 2^32 = 4GB，但实际最大容量为 2GB）。
    
-   **读写单位**：
    
    -   虽然SD协议允许按字节寻址，但 **实际读写仍以块（Block）为单位**（典型块大小是512字节）。
        
    -   例如：
        
        -   `CMD17 (READ_SINGLE_BLOCK)` 读取一个块（如512字节）。
            
        -   `CMD24 (WRITE_BLOCK)` 写入一个块。
            
    -   **即使主机只请求1字节，SD卡仍会读写整个块**，再由主机控制器或软件处理所需字节。
        

* * *

#### **2. SDHC（高容量卡，2GB~32GB）和 SDXC（扩展容量卡，32GB~2TB）—— 以块（Block）为单位寻址**

-   **物理寻址方式**：SDHC/SDXC 使用 **块寻址（Block Addressing）**，地址直接指向块号（32位块地址，每块固定为512字节，最大支持 2^32 × 512B = 2TB）。
    
    -   例如：
        
        -   地址 `0` = 第0块（0x000~0x1FF）。
            
        -   地址 `1` = 第1块（0x200~0x3FF）。
            
-   **读写单位**：
    
    -   所有操作必须按整块进行，**不支持字节级寻址**。
        
    -   例如：
        
        -   `CMD17` 读取一个块（512字节），即使主机只需要其中几个字节。
            
        -   `CMD24` 写入一个块，必须完整写入512字节。






* * *

### 9.8 stm32F103x的SDIO外设


![alt text](image-118.png)

  * 这里的`SDIOCLK`有误. 实际上在F1系列是

  ![alt text](image-119.png)
  所以应该是72MHz.
  在F2~4系列里SDIOCLK是单独的时钟线, 是48MHz.


其中SDIO_D[7:0]的前四位是DATA0~3数据线, 剩下的是MMC卡要用的线, SD卡不需要

-----
-----
#### **指南者STM32的SDIO(即SD卡槽)原理图:**

![alt text](image-122.png)

其中9/15引脚的作用是SD卡检测.1 检测到SD卡输出高电平. 可以绑定到一个普通GPIO比如`PA2`上实现检测.

-----
-----

### 9.9 SDIO编程

**标准库**函数对SDIO外设建立了三个初始化结构体，分别为

* **SDIO初始化结构体SDIO_ InitTypeDef**			(初步初始化SDIO外设)
* **SDIO命令初始化结构体SDIO_ CmdInitTypeDef**  (初始化CPSM)
* **SDIO数据初始化结构体SDIO_ DatalnitTypeDef** (初始化DPSM)

配置好后就可以用函数`SDIO_DataConfig()`发送数据.

初始化结构体成员用于设置SDIO工作环境参数，并由SDIO相应初始化配置


SD卡不能工作在超过24MHz, 所以一般可以设置寄存器BYPASS=0; CLKDIV=3, 
用MCU的主频`SDIOCLK`=72MHz的/3后=24MHz的`SDIO_CK`来驱动SDIO外设.

![alt text](image-121.png)

* * *


### 9.10 FATFS


只有加入文件系统才能使得单片机写入数据后的SD卡可以被读卡器+电脑读取.


ATFS 是一个轻量级的 FAT 文件系统库，专门用于在嵌入式系统（如 STM32）中读写文件系统（FAT12/16/32）格式的存储设备，如 SD 卡、U盘、SPI Flash 等。










# 10 SPI Flash

## 10.0 SPI协议

SPI可以看作USART的改进:

UART传输速度慢而且不稳定, 因为即使约定好了传输速率, 双方的实际波特也会有浮动. 一般只能在使用同一个系统时钟两个挨得很近的双方使用. 

这导致即使使用最常用的最大波特率`115200`, UART速度也非常慢(11.25KB/s)

SPI协议的思想就是拉一根CLOCK, 让其中一方A发送clock给另一方B, 这样就不需要约定传输速度了. B在每次收到CLOCK上升沿的时候接收1bit数据即可. 传输速度轻松突破10M/s.

![alt text](image-133.png)

---
SPI是由motorola公司开发的一种通用数据总线. 使用四根通信线:

| 信号线      | 全称                  | 方向       | 说明         |
| -------- | ------------------- | -------- | ---------- |
| **MOSI** | Master Out Slave In | 主机 → SD卡 | 主机发数据（命令）  |
| **MISO** | Master In Slave Out | SD卡 → 主机 | SD卡返回数据    |
| **SCLK** | Serial Clock        | 主机 → SD卡 | 时钟信号       |
| **CS**   | Chip Select（片选）     | 主机 → SD卡 | 拉低使能选中的SD卡 |

SPI协议是同步全双工的.

支持总线挂载多个从设备.

W25Q64: 一款常见的NOR Flash存储器芯片, 使用SPI接口.
| 属性       | 说明                             |
| -------- | ------------------------------ |
| **型号**   | W25Q64                         |
| **容量**   | 64 Mbit = **8 MB**             |
| **类型**   | NOR Flash                      |
| **接口**   | SPI（支持标准SPI、Dual SPI、Quad SPI） |
| **电压范围** | 2.7V \~ 3.6V                   |
| **厂家**   | Winbond（华邦电子）                  |




# 12 OLED

一般用作调试显示屏, 方便我们调试程序.

![alt text](image-104.png)

常见两种协议, 4阵脚的是I2C, 7针脚的是SPI

![alt text](image-105.png)


江协科技up主写的驱动函数:
前两个参数是行/列.
![alt text](image-106.png)


![alt text](image-120.png)


##

# 13 文件系统

FarFS(FAT(File Allocation Table) File System), 文件分配表-文件系统.



##
# 14 常见问题(bug log)


## 14.1 库函数未声明(this func is decleared implicit)

检查:
* 1.有没有include头文件
  * #include "stm32f10x.h"(总头文件)
    * 注意: 该总头文件通过这段代码来include `stm32f10x_conf.h`:
```c
#ifdef USE_STDPERIPH_DRIVER
#include "stm32f10x_conf.h"
#endif
```
  * 而`stm32f10x_conf.h`中含有#include "stm32f10x_rtc.h"...(各种外设的头文件, **它们是你最终要include的头文件!**), 但是它们可能是默认被注释的, 需要把对应的头文件去注释哦!
* 2.有没有意外的中文路径名
  * 比如因为复制来的项目导致的文件夹叫做`xxx-副本`


## 14.2 C项目的.c和.h; 


### 14.2.1 预处理阶段和编译阶段

在编译之前首先是预处理阶段. 这个阶段, 预处理器会分别处理**project list**的每个.c文件. 预处理器开始阅读这个.c代码, 并且将所有`#`命令进行代码替换. 当遇到`#include`时, 预处理器在`including path`中寻找该文件, 然后将该文件的所有代码插入到此处.

**.h头文件在且仅在在这个阶段被使用.** 预处理完了后, .h文件已经融入到**project list**的每个.c文件, 不复存在. 
这些预处理好的.c文件一般后缀为.i

然后是编译.
编译器会将**project项目列表**中的**每个**.c文件(已被预处理为对应.i文件)编译为对应.o文件, 然后使用这些.o文件进行link.

记住黄金法则：  
**「.c文件是工人（需要雇佣），.h文件是工具（需要知道在哪拿）」**

* **project list项目列表**只会看到.c文件, 忽略.h文件.
由于上述说明的, 编译实际上是以一个个.c文件为最小单元处理然后link, 所以不应当#include 任何.c文件. 这意味着你在预处理阶段将一个.c文件复制到另一个.c文件中去了. 这两个.c文件都会参与编译, 这在link阶段一定会导致重复定义函数错误.

(除非你用static关键字...**每个static符号获得唯一内部标识**. 可以有同名的static函数或static全局变量，它们被视为不同的实体，不会在链接时发生冲突。).

* `static`关键字即意味着该函数/变量的作用域是**当前编译单元**, 即: link的时候, 当前这个.i文件自己可以用, 其他参与link的.i文件不可以.
* 目标文件(.o)的`symbol`指的是函数名和全局变量名（非static的）。

* 关于全局变量和静态变量:
在一个project list中, 对每个.c文件, (不考虑函数内部形参变量) 函数和变量只有全局和静态之分, 默认是全局, static是静态.

* extern关键字的作用: *给**编译器**的承诺.*
  * **编译阶段**: 告诉编译器：这个符号不在当前**翻译单元(.i文件)**定义，而是在其他某个地方定义，所以不要为它分配存储空间（对于变量）或者不要报错（对于函数. 
    * 它会相信这个符号在其他地方定义，因此不会在当前目标文件（.o）中为该符号分配空间（对于变量）或生成代码（对于函数），也不会因为找不到定义而报错。它会在目标文件中生成一个未解决的外部符号（unresolved external symbol）的引用。
    * 只有变量需要extern, 函数的声明默认是`extern`的，所以通常我们写函数声明时不需要显式加上`extern`。但是，如果你希望强调它是外部链接，也可以加上。
  * **链接阶段**：链接器会收集所有目标文件.i，然后解析这些未解决的引用。如果链接器在所有目标文件中都找不到这个符号的定义，就会报“未定义的符号”错误.

### 14.2.2 link阶段的编译错误: .o文件中未定义符号/

在keil5中编译SD的fatfs项目时3, 因为在ffconf.h中配置了
```c
//系统使用的字符编码(Code Page)
#define _CODE_PAGE	437

//使用长文件名模式
#define	_USE_LFN	1
```
而没有在左侧project栏添加`..\fatfs\options\unicode.c`一起编译, 从而会报错:
```
*** Using Compiler 'V5.05 update 2 (build 169)', folder: 'D:\Keil_v5\ARM\ARMCC\Bin'
Build target 'sdio_sdcard'
compiling ff.c...
compiling diskio.c...
compiling main.c...
linking...
..\..\Output\Template.axf: Error: L6218E: Undefined symbol ff_convert (referred from ff.o).
..\..\Output\Template.axf: Error: L6218E: Undefined symbol ff_wtoupper (referred from ff.o).
Not enough information to list image symbols.
Not enough information to list load addresses in the image map.
Finished: 2 information, 0 warning and 2 error messages.
"..\..\Output\Template.axf" - 2 Error(s), 0 Warning(s).
Target not created.
```

这种错误是因为编译器在编译好所有.c文件后, 在link阶段发现其中一些symbol(其实就是.o文件中的函数)找不到定义. 这是因为缺少了一些.c文件没有编译后参与到link阶段.

 * 类似的link阶段报错还有symbol重复定义. 这是由于所有参与link的.o文件中, 存在重复的函数定义. 这一般是由于你在项目的project中含有两个同名函数的.c文件参与编译, 或者.h文件


### 14.2.3 IDE的**project list**和**including path**

包括keil的IDE有两个列表:
* project list: 在这个虚拟路径下的所有.c文件会参与预处理和编译, **.h文件则被忽略.**
* including path: 这是一个地址列表. 在预处理时, 预处理器会在这个列表里寻找include命令指向的文件.


keil的话, 点击魔术棒(options)选项-> C/C++ -> include paths, 选择添加要include的头文件的路径. 注意路径没有包含关系, 必须是同级目录.

### 14.2.4 官方的头文件怎麽也报错"xxx not decleared/ implicit declearation"?

因为官方头文件可能并不是被设计用来单独使用的. 
比如CMSIS的"core_cm3.h". 这个头文件直接使用了很多变量/枚举类型, 比如`IQR_Type`, 直接包含它, 会在编译的时候因为没有定义`IQR_Type`而报错. 事实上, "core_cm3.h"期望它被一个具体的MCU头文件给包含, 而前者已经定义好了具体的
`IQR_Type`等变量/宏 作为一款具体型号的MCU的接口.

### 14.2.5 CMSIS和stm32f10x标准库

**CMSIS（Cortex Microcontroller Software Interface Standard）**是 ARM 官方为所有 Cortex-M 系列 MCU 提供的**底层标准接口/核心库**. 包括所有的stm32的MCU都是Cortex-Mx(x=3,4,7)架构的.


**使用CMSIS为基础编写的stm32f10x标准库**的文件结构:

![**使用CMSIS为基础编写的stm32f10x标准库**的文件结构:](W{@90@]_AKS4OUQ`MP0QEH6.png)

可以看到, 主要是4个库文件在项目列表分支`CMSIS`中, `stm32f10x.h` 是STM32F10x MCU 的**设备头**. 它include了`core_cm3.h`. 使用**stm32f10x标准库**的时候只需要在需要用到标准库的模块`#include "core_cm3.h"`即可.



## 14.3 中断函数怎麽写好?

关于一些中断函数写在哪里? 它们用的全局变量呢? 经验:
| 方式                                       | 说明          | 是否推荐           |
| ---------------------------------------- | ----------- | -------------- |
| 在 `it.c` 直接 `extern` 各个变量                | 简单，但维护成本高   | ❌ 不推荐          |
| 把变量声明写在各自 `.h` 文件中，在 `it.c` 中 `#include` | 易维护，结构清晰    | ✅ 推荐           |
| 中断函数内部「转发」到对应外设模块处理                      | 解耦、利于大型项目扩展 | ✅✅ 强烈推荐（中大型项目） |


## 14.4 F12找不到定义跳转?

如果当前没有报错或警告, 可能是你还没有编译. 编译一下再F12.

也有可能是编译器内建的扩展关键字（attribute）. 如:


```c
//__weak是Keil MDK (armcc)和ARM GCC（比如 STM32CubeIDE）的编译器内建的扩展关键字（attribute）. 表示这个函数是弱符号（Weak Symbol）
//如果程序中没有其他地方定义这个函数, 那就用这个定义. 否则就无视这个定义! get_fattime()这个函数可能很容易在其他地方重复声明, 我们不希望影响到那些地方.
__weak DWORD get_fattime(void) {
```


## 14.5 添加新的bsp文件步骤:

可以自己去USER新建, 也可以在keil界面直接选择USER文件夹然后add a new file...但是记得改一下路径, 看一眼你创建在哪里了. 记得维持清爽的USER文件夹下面各种外设文件夹的结构! 不然还得花时间维护include path设置

![alt text](image-80.png)

因为这样做生成的文件




























## 14.6 利用DAP(Debug Access Port)进行烧录/调试


#### .6.0 DAP接口
DAP 是 ARM Cortex-M 系列微控制器中用于调试和编程的一种接口机制. 它可以接受来自外部调试器（比如 ST-Link、J-Link、CMSIS-DAP）的访问请求，通过支持不同协议与外界通信。

野火DAP下载器:
1. 遵循ARM公司的CMSIS-DAP标准,支持所有基于Cortex-M内核的单片机
2. 属于**HID设备**,跟鼠标键盘-样，无需安装驱动
3. 支持XP/WIN7/WIN8/WIN10 这四个操作系统
4. 支持JTAG和SW下载模式,可在线调试和硬件仿真

注意: 高速版DAP下载器支持JTAG和SW模式,全速版只有SW模式.

* **SW(SWD, serial wire debug)**
  -   是 ARM 推出的替代 JTAG 的简化版调试接口。
  -   **只需两根线（SWDIO+SWCLK）**，适合引脚紧张的 MCU。 
  -   用于断点调试、寄存器查看、烧录程序等。
🔧 你常用的 ST-Link 和 J-Link 就是通过 SWD 跟 MCU 通讯的。


| 引脚名            | 功能说明              |
| -------------- | ----------------- |
| **SWDIO**      | 数据输入输出（双向）        |
| **SWCLK**      | 时钟信号（单向）          |
| **GND**        | 地线（必须连接）          |
| **VCC**        | 目标电源参考电压（通常3.3V）  |
| （可选）**nRESET** | 系统复位引脚，用于远程复位（可选） |


* **JTAG(joint test action group)**
  -   是一种更通用的、**4~5线的调试接口**（TDI/TDO/TCK/TMS/\[TRST\]）。 
  -   支持多芯片级联调试。   
  -   功能比 SWD 更复杂，但在 STM32 上你不一定需要这么多。


| 引脚名        | 功能说明                     |
| ---------- | ------------------------ |
| **TDI**    | Test Data In（数据输入）       |
| **TDO**    | Test Data Out（数据输出）      |
| **TCK**    | Test Clock（时钟信号）         |
| **TMS**    | Test Mode Select（模式选择）   |
| **TRST**   | Test Reset（可选，不是所有芯片都需要） |
| **GND**    | 地线                       |
| **VCC**    | 目标板参考电压                  |
| **nRESET** | 系统复位（可选）                 |

* **SWJ(SWD JTAG Switching)**

也就是JTAG+SWD功能.


大多数 Cortex-M 系列 MCU（比如 STM32）在芯片启动时会默认启用 SWJ 全部功能，也就是JTAG + SWD 都打开。
通过设置特定寄存器（一般是 AFIO_MAPR 或 SYSCFG->CFGRx，视芯片而定）可以关闭 JTAG，仅保留 SWD，以节省引脚资源。


* V-UART(Virtual UART)
  -   是 CMSIS-DAP 或 DAPLink 提供的一种虚拟串口。 它在 USB 中模拟出一个 UART（串口），你可以在电脑上看到一个 COM 口
MCU 与电脑通过这个虚拟串口进行 日志输出、命令输入、调试通信
  - 好处：不需要接额外串口芯片（如CH340、MAX232），直接在调试器中就有
  -   用来实现 **USB转串口** 的调试输出。
  -   比如你插上一个 DAPLink 调试器后，电脑上出现一个 COM 口，就是 V-UART 提供的，通常被用来做 `printf` 输出调试。


####  .6.1**keil中用DAP烧录/调试**
------------
使用SWD或JTAG连接好DAP调试器后, 在keil中选择debugger为CMSIS-DAP Debugger:

![alt text](image-164.png)


记得utilities勾选启用debugger

![alt text](image-152.png)

在接入DAP调试器后, 在debug->setting中可以看到具体的调试器型号, 以及连接到的MCU型号

![alt text](image-153.png)

![alt text](image-154.png)

其中的序列号:
FS-xxxx: 野火full speed版
HS-xxxx: 野火high speed版(支持JTAG)


Reset选项:
![alt text](image-155.png)

| 选项          | 使用场景                  |
| ----------- | --------------------- |
| SYSRESETREQ | 推荐的默认方式，适合绝大多数情况      |
| HW RESET    | MCU跑飞、Flash错烧时清除状态最彻底 |
| VECTRESET   | 内核异常恢复，但不影响外设状态       |

选择flash download. (烧录到哪个flash中. 复杂的开发板可能识别出好多flash设备)

![alt text](image-157.png)

记得勾选eraser sector(擦除全片有点慢), 还有reset and run(软件烧录后自动reset+run, 就和硬件reset一样的)
![alt text](image-158.png)

#### .6.2 ST-LINK

使用CUBE_IDE的工程不能直接使用DAP下载器, 必须使用ST-LINK下载器...





## 14.7 使用ST-LINK烧录/调试













## 14.7 跨芯片移植程序


流程大概是:
Project → Options → device, 更改芯片选型.

确定`CMSIS/stm32f10x.h`中那堆芯片选型的#ifdef只选中了一个. 如果多个芯片选型的#ifdef被命中, 会拼接出错误的语法.

Project → Options → C/C++ → Defines, 这里是**家族宏**. `,`分隔的字符串, 每个字符串`str`都被当作在当前所有文件开头`#define str`. 查看家族宏有没有问题. 

比如f103VET6->F103C8T6时, 家族宏里`STM32F10X_HD`要改成`STM32F10X_MD`.

然后修改一个个bsp_xxx






##

##














# 16. 汇编语言(Assembly)




https://www.bilibili.com/video/BV1KG411T77D/?spm_id_from=333.337.search-card.all.click&vd_source=4665566d92a194d3e4867c96fe3441c0


### 0. 简介

汇编语言没有统一的标准版本，因为它是针对**具体处理器架构**编写的，每种处理器（或架构）都有自己的汇编语言指令集。我们通常说“汇编语言版本”时，实际是指 所使用的指令集架构（ISA）：
| 架构                      | 常见使用者             | 示例汇编指令             |
| ----------------------- | ----------------- | ------------------ |
| x86 / x86-64            | Intel, AMD PC端    | `MOV EAX, 1`       |
| ARM (如 ARMv7, Cortex-M) | STM32、手机CPU等嵌入式系统 | `MOV R0, #1`       |
| RISC-V                  | 新兴开源架构，清华Ysyx等    | `addi a0, zero, 1` |
| MIPS                    | 教育领域、早期嵌入式        | `li $v0, 1`        |

汇编语言源代码一般以.asm结尾.

汇编器(Assembler)将**汇编代码**翻译为**机器码(比特流)**.
不同工具链的汇编器也会有版本，例如：
| 汇编器                  | 常见平台             | 示例版本命令                    |
| -------------------- | ---------------- | ------------------------- |
| `GNU as` (gas)       | Linux、ARM GCC工具链 | `as --version`            |
| `Keil ARM Assembler` | Keil MDK for ARM | 自动随 IDE 集成                |
| `NASM`               | x86/PC平台         | `nasm -v`                 |
| `MASM`               | Windows x86      | Microsoft Macro Assembler |

例如,**Keil + STM32**调试环境中使用的是:
  -   使用的是 **ARM Cortex-M 架构的汇编语言（ARM Thumb 指令集）** 
  -   汇编器是 **Keil MDK 的 ARM 汇编器**（内部支持 `.asm` 或 `.s` 文件）



## 1. ASM的vscode插件

在vscode里用这个插件写汇编.

![alt text](image-159.png)



插件配置主要改这两个:使用的**汇编工具**/使用的**DOS环境模拟器**就可以了

![alt text](image-160.png)


dosbox: 弹出类似cmd的黑框框.
msdos pleayer: 直接在终端运行


代码写好之后, 右键可以看到:
![alt text](image-161.png)

* open emulator(打开dos环境)
* run ASM code(运行当前程序, 汇编+linking+running)
* debug ASM code(调试当前程序, 汇编+linking+debugging)



### 1.1 调试ASM code
![alt text](image-162.png)

指令:
* `t`: trap单步执行
* `r`: 查看寄存器
* `G`: 程序运行到底
* `D [addr]`: 查看内存中的内容. 如D


## 2. 基础指令&通用寄存器 (intel 8086架构)

* 通用寄存器（General Purpose Registers）
这些寄存器可以用于算术运算、存储临时变量、函数调用参数等。它们也可以细分为高8位和低8位，例如 AX 可以拆成 AH（高）和 AL（低）。
| 名称   | 含义                   | 用途                                                   |
| ---- | -------------------- | ---------------------------------------------------- |
| `AX` | Accumulator 累加器      | 最重要的寄存器之一，很多指令默认使用 AX 作为操作数。例如乘法、除法等指令结果默认存入 AX      |
| `BX` | Base Register 基地址寄存器 | 常用于作为基址，配合变址寄存器组成内存访问地址                              |
| `CX` | Count Register 计数器   | `LOOP` 指令自动使用 CX 作为循环计数器；也用于 `SHIFT`、`ROTATE` 类的重复次数 |
| `DX` | Data Register 数据寄存器  | 通常用于存放 I/O 地址、数据或乘除法时的高位操作数或结果                       |

* 指针和变址寄存器（Pointer and Index Registers）
主要用于处理 内存地址计算，特别是在字符串操作、数组访问等。
| 名称   | 含义                        | 用途                                          |
| ---- | ------------------------- | ------------------------------------------- |
| `SP` | Stack Pointer 栈指针         | 指向栈顶；入栈 `PUSH` 和出栈 `POP` 操作会自动改变它           |
| `BP` | Base Pointer 基指针          | 用于访问栈帧中的数据（函数局部变量/参数）；常和 `SP` 配合使用          |
| `SI` | Source Index 源变址寄存器       | 字符串复制或数组读取时用于指向源数据地址，常用于 `MOVS`, `LODS` 等指令 |
| `DI` | Destination Index 目标变址寄存器 | 同上，但用于目标地址，常用于 `STOS`, `SCAS` 等指令           |

* 段寄存器（Segment Registers）
  在 8086 中内存寻址采用段:偏移（Segment:Offset）方式。每个段寄存器控制一类内存区域的起始位置。
| 名称   | 全称                       | 用途                                    |
| ---- | ------------------------ | ------------------------------------- |
| `CS` | Code Segment 代码段         | 指向当前正在执行的程序代码的段地址                     |
| `DS` | Data Segment 数据段         | 默认的数据存储段                              |
| `SS` | Stack Segment 栈段         | 当前栈的段地址，配合 `SP` 使用                    |
| `ES` | Extra Segment 附加段        | 一般用于字符串操作（比如 `MOVS`, `LODS`）中作为附加的数据段 |
| `IP` | Instruction Pointer 指令指针 | 当前正在执行指令的偏移地址，配合 `CS` 定位下一条要执行的指令     |

* 标志寄存器（FLAGS Register）
  用于保存 CPU 当前的状态，比如上次运算结果是正/负/零/溢出等，供条件跳转和判断使用。
| 名称   | 含义              | 功能                       |
| ---- | --------------- | ------------------------ |
| `ZF` | Zero Flag       | 上一操作结果为 0，则 ZF = 1       |
| `SF` | Sign Flag       | 上一操作结果为负数，则 SF = 1       |
| `CF` | Carry Flag      | 进位标志（无符号数运算时有溢出则 CF = 1） |
| `OF` | Overflow Flag   | 溢出标志（有符号数运算时溢出）          |
| `PF` | Parity Flag     | 检查结果的最低字节中有偶数个1，则 PF = 1 |
| `AF` | Auxiliary Carry | 半字节进位（用于 BCD 运算）         |
| `IF` | Interrupt Flag  | 是否响应外部中断                 |
| `DF` | Direction Flag  | 字符串处理指令的方向（自增或自减）        |


MOV:

```Assembly
MOV	AX, Y ;

```




Operand(操作数): 指令作用的对象.`MOV AX BX`指令中, 
常见操作数可以是：
| 类型       | 示例            | 说明         |
| -------- | ------------- | ---------- |
| **寄存器**  | AX, BX, CX... | 寄存器内的值     |
| **立即数**  | 5, 0x1F       | 直接写的数值     |
| **内存地址** | `[1234H]`     | 某个内存位置     |
| **偏移地址** | `[BX+SI]`     | 用寄存器计算出的地址 |

```asm
MOV AX, 1234H     ; 将立即数 1234H 放入 AX
MOV BX, AX        ; 将 AX 的值复制到 BX
MOV [2000H], BX   ; 将 BX 的值写入内存 2000H

```










