/* 学习用的带注释链接脚本. GNU ld 支持 C 风格注释；中文注释本身不是语法错误. */



/*================================================
ld链接脚本的骨架:

ENTRY([符号])          定义入口 
PHDRS {               定义程序头
[段名称] [p_type]
}      

SECTIONS {            section布局（核心）
. = ...               设置位置计数器
[section name] : {    输出节布局...
[section contents]    一般是收集输入文件的section, 移动位置计数器, 一些符号赋值.
} : [program header]  将该section挂到某个程序头.

}
符号 = 表达式;       符号赋值，可出现在顶层或 SECTIONS 里

它们都是一条条给linker的指令.
=================================================*/



/* 指定elf头的入口符号.
`ENTRY(_start)` 这条指令告诉linker: 生成elf文件时, 把elf head中的入口字段`e_entry`设置为符号`_start`的地址.
符号_start应当在启动汇编文件Start.S中定义.
*/
ENTRY(_start)


/* 程序头(program headers).
这些指令告诉linker: 给生成的elf文件定义两个程序头:
名字分别为text,data, 段类型都为PT_LOAD(加载段)
不过elf文件中的段不会记录名字. 这两个名字只是方便编写ld脚本...
*/
PHDRS { 
  text PT_LOAD;    /* Loadable text segment (code) */
  data PT_LOAD;    /* Loadable data segment */
}


/*
section布局.
. = [表达式]  设置位置计数器. `.`就是位置计数器. _pmem_start, _entry_offset这些符号也是
*/
SECTIONS {
  /* _pmem_start and _entry_offset are defined by compiler LDFLAGS */
  . = _pmem_start + _entry_offset;  /* Set output address to memory start + entry offset */
  
  /* `.text`输出段的布局定义. 创建一个名为.text的输出section,*/
  .text : {
    *(entry)      /* 收集所有名为entry的输入section, 将内容组织到此处. `*(x)` 通配所有参与编译文件中名为x的输入section. */
    *(.text*)     /* 收集所有名称以.text开头的输入section, 将内容组织到此处. */
  } : text        /* `: text` 把该输出段挂到程序头`text`. 维护相应地址偏移等数据.  */
  etext = .;      /* 定义符号etext和_etext, 作为.text section的末尾. 这两个符号是留给一些需要知道代码区边界的工具使用的. 为了兼容GNU工具链中各种可能使用的命名惯例. */
  _etext = .;
  
  /* Read-only data section (constants, string literals, etc) */
  .rodata : {
    *(.rodata*)   /* Link all read-only data sections */
  }
  
  /* Initialized data section (global variables with initializers) */
  .data : {
    *(.data*)     /* Link all .data* sections */
  } : data        /* Assign to data segment */
  edata = .;      /* Mark end of data section */
  _data = .;
  
  /* Uninitialized data section (BSS - Block Started by Symbol) */
  .bss : {
	_bss_start = .;  /* Mark start of BSS for zero-initialization */
    *(.bss*)       /* Link all .bss* sections (uninitialized globals) */
    *(.sbss*)      /* Small BSS sections */
    *(.scommon)    /* Common symbols */
  }
  
  /* Stack layout: reserve 32KB (0x8000) stack space */
  _stack_top = ALIGN(0x1000);       /* Align stack top to 4KB boundary */
  . = _stack_top + 0x8000;          /* Reserve 32KB for stack growth */
  _stack_pointer = .;               /* Initial stack pointer (top of stack) */
  
  /* Heap layout: starts after BSS, aligned to 4KB */
  end = .;                          /* End of program */
  _end = .;
  _heap_start = ALIGN(0x1000);      /* Heap starts here, aligned to 4KB */
}
