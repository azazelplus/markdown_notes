; ========== 数据段 ==========
;定义变量可以有名字也可以没有名字.
DSEG SEGMENT           ; 定义一个名为DSEG(名字自定义)的数据段（Data Segment）

; 定义一个变量叫MESS.它是一个以 '$' 结尾的字符串,
; 0DH 是回车符（Carriage Return）
; 0AH 是换行符（Line Feed）
; 24H 是 DOS 的字符串结束符，供 INT 21H 的 AH=9 功能使用
        MESS DB 'Hello, World!', 0DH, 0AH, 24H     

        my_var DB ? ; 定义1byte变量 my_var，未初始化
        my_array DB 100(?) ; 定义长度为100的1byte数组 my_array，包含100个未初始化的元素                 

DSEG ENDS              ; ENDS(end segment)指数据段结束



; ========== 堆栈段 ==========
SSEG SEGMENT PARA STACK  ; 定义一个叫做SSEG的堆栈段（Stack Segment），对齐方式为段（paragraph）
        DW 256 DUP(?)            ; 分配 256 个 word（512 字节）的栈空间，未初始化
SSEG ENDS                ; 堆栈段结束




; ========== 代码段 ==========
;CSG不需要重新装载, 但DSEG和SSEG需要重新装载.
CSEG SEGMENT             ; 定义一个叫做CSEG的代码段（Code Segment）

        ASSUME CS:CSEG, DS:DSEG, SS:SSEG ; 指定各段寄存器的段的逻辑名称（告诉汇编器(不认识你自己取的名字DSEG, SSEG, CSEG)各段的用途）

    BEGIN:                   ; 程序入口标签（Entry point）. `label_name: [命令] END label_name`被称为一个标签段（Label）, 必须有一个主label.

        MOV AX, DSEG             ; 将`数据段DSEG的地址`加载到 AX 寄存器中
        MOV DS, AX               ; 将AX的值（数据段地址）加载到 DS 寄存器中

        MOV DX, OFFSET MESS      ; 将字符串 MESS 的偏移地址加载到 DX 中
        MOV AH, 9                ; 设置 AH = 9，调用 DOS 功能号 09H（打印字符串）
        INT 21H                  ; 调用 DOS 中断 21H，执行打印操作（直到遇到 $ 结束）

        MOV AH, 4CH              ; 设置 AH = 4CH，调用 DOS 功能号 4C（退出程序）
        INT 21H                  ; 调用 DOS 中断 21H，退出程序并返回控制权给 DOS

CSEG ENDS                ; 代码段结束
END BEGIN                ; 指明程序入口点为 BEGIN（程序从这里开始执行）

































