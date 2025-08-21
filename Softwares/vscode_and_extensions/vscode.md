
# 1.vscode的配置结构(c/c++):

共有三个配置文件:
* 1.**settings.json**
  * 控制的是你的**VS Code 编辑器的表现**，比如：
    -   缩进用空格还是 Tab？
    -   C++ 代码格式化规则？
    -   是否自动保存？
    -   是否启用某个插件的功能？   
    🧠 属于 VS Code 的“偏好设置”。


* 2.**launch.json**
  * 控制的是**怎么调试程序**，比如
  -   启动哪个程序（比如 `main.exe`）？
  -   传什么参数？ 
  -   使用 GDB 还是 LLDB？   
  -   是否自动停止在 main 函数处？
  🌟 是你点「开始调试（F5）」时 VS Code 怎么运行程序的说明书。


* 3.**tasks.json**
  * 这个文件是用来定义你点「运行任务（Ctrl+Shift+B）」或自动构建时，VS Code 应该：
    -   执行哪条 shell 命令？       
    -   编译用哪个编译器（`g++`、`clang++`）？   
    -   把哪个 `.cpp` 编成什么名字？ 
    -   是否需要清理旧文件？
💡 是你「构建项目」时怎么干活的说明书。


## 1.settings.json
优先级: 默认设置(--)<全局设置(`C:\Users\<YourUsername>\AppData\Roaming\Code\User\settings.json`)<工作区设置<文件夹设置.



### 1\. **全局用户设置 (User Settings)**

这个文件存储了你所有的全局配置，适用于你所有的 VSCode 项目和工作区。修改这个文件就会影响你 VSCode 的整体行为。

-   **Windows**：
    
    -   `C:\Users\<YourUsername>\AppData\Roaming\Code\User\settings.json`
        
-   **Linux**：
    
    -   `/home/<YourUsername>/.config/Code/User/settings.json`
        
-   **macOS**：
    
    -   `/Users/<YourUsername>/Library/Application Support/Code/User/settings.json`
        

### 2\. **工作区设置 (Workspace Settings)**

这个文件是为当前项目（工作区）定制的配置。它只会影响当前打开的项目，其他项目不受影响。你可以在每个项目的 `.vscode` 文件夹下找到这个文件。

-   **路径**：
    
    -   `<你的项目目录>/.vscode/settings.json`
        

### 3\. **文件夹设置 (Folder Settings)**

你可以为某个文件夹（不一定是完整的工作区）配置特定的设置。这些设置会优先级高于全局用户设置，但低于工作区设置。

-   **路径**：
    
    -   `<你的文件夹路径>/.vscode/settings.json`
        

### 4\. **默认设置 (Default Settings)**

这个文件不太可能被你直接编辑，但是 VSCode 内部有一份“默认设置”，所有的设置都会从这个文件继承。你只能通过在用户设置文件（`settings.json`）中覆盖这些默认值来修改它。

-   **路径**：
    
    -   内部存储，不需要关心
        

### 如何快速定位这些文件？

1.  **通过 VSCode 直接打开**：
    
    -   按 `Ctrl + Shift + P`，然后输入 `Preferences: Open Settings (JSON)` 打开全局 `settings.json`，或者选择 `Preferences: Open Workspace Settings (JSON)` 打开当前工作区的设置。
        
2.  自己去对应路径找: 全局设置(`C:\Users\<YourUsername>\AppData\Roaming\Code\User\settings.json`), 工作区和文件夹的就在文件夹主目录.vscode.




## 2. launch.json

优先级: 只有一个全局默认(不可见)和一个当前文件夹自定义(可选).



## 3. tasks.json

优先级: 只有一个全局默认(不可见)和一个当前文件夹自定义(可选).

## 4. 推荐配置 by azazel


```json
{
    //by azazel. settings.json v0.1.  2025/4/20
    //这是vscode的全局配置. 


    //这些是大量关于 LaTeX-Workshop 插件的内容：
    //设置自动编译的行为。当前值为 "never"，表示不会自动编译，需要手动触发。改为 "onSave"（每次保存文件时自动触发编译）可能更方便。
    "latex-workshop.latex.autoBuild.run": "never",

    //是否显示上下文菜单。启用上下文菜单后，可以通过右键快速操作。
    "latex-workshop.showContextMenu": true,

    "latex-workshop.intellisense.package.enabled": true,

    //是否显示错误和警告信息。你将它们设置为 false，这意味着任何错误或警告都不会弹出。
    "latex-workshop.message.error.show": false,

    "latex-workshop.message.warning.show": false,

    //定义了多个工具链（如 xelatex、pdflatex 等），并配置了它们的具体命令和参数。
    "latex-workshop.latex.tools": [
        {
            "name": "xelatex",
            "command": "xelatex",
            "args": [
                "-synctex=1",
                "-interaction=nonstopmode",
                "-file-line-error",
                "%DOCFILE%"
            ]
        },
        {
            "name": "pdflatex",
            "command": "pdflatex",
            "args": [
                "-synctex=1",
                "-interaction=nonstopmode",
                "-file-line-error",
                "%DOCFILE%"
            ]
        },
        {
            "name": "latexmk",
            "command": "latexmk",
            "args": [
                "-synctex=1",
                "-interaction=nonstopmode",
                "-file-line-error",
                "-pdf",
                "-outdir=%OUTDIR%",
                "%DOCFILE%"
            ]
        },
        {
            "name": "bibtex",
            "command": "bibtex",
            "args": [
                "%DOCFILE%"
            ]
        }
    ],

    "latex-workshop.latex.recipes": [
        {
            "name": "XeLaTeX",
            "tools": [
                "xelatex"
            ]
        },
        {
            "name": "PDFLaTeX",
            "tools": [
                "pdflatex"
            ]
        },
        {
            "name": "BibTeX",
            "tools": [
                "bibtex"
            ]
        },
        {
            "name": "LaTeXmk",
            "tools": [
                "latexmk"
            ]
        },
        {
            "name": "xelatex -> bibtex -> xelatex*2",
            "tools": [
                "xelatex",
                "bibtex",
                "xelatex",
                "xelatex"
            ]
        },
        {
            "name": "pdflatex -> bibtex -> pdflatex*2",
            "tools": [
                "pdflatex",
                "bibtex",
                "pdflatex",
                "pdflatex"
            ]
        },
    ],

    "latex-workshop.latex.clean.fileTypes": [
        "*.aux",
        "*.bbl",
        "*.blg",
        "*.idx",
        "*.ind",
        "*.lof",
        "*.lot",
        "*.out",
        "*.toc",
        "*.acn",
        "*.acr",
        "*.alg",
        "*.glg",
        "*.glo",
        "*.gls",
        "*.ist",
        "*.fls",
        "*.log",
        "*.fdb_latexmk"
    ],
    "latex-workshop.latex.autoClean.run": "onFailed",

    //定义默认使用的编译方案（这里为 lastUsed，即上次使用的方案）。
    "latex-workshop.latex.recipe.default": "lastUsed",

    //配置 PDF 同步功能的快捷键。这里设置为 double-click（双击 PDF 时同步跳转到源文件对应的位置）。
    "latex-workshop.view.pdf.internal.synctex.keybinding": "double-click",

    //指定了默认的 Python 解释器路径。当前设置指向了 Anaconda3 环境中的 python_yolo 环境。
    "python.defaultInterpreterPath": "C:\\Users\\azazel\\Anaconda3\\envs\\python_yolo\\python.exe",


    "cmake.showOptionsMovedNotification": false,


    "code-runner.languageIdToFileExtensionMap": {
        "bat": ".bat",
        "powershell": ".ps1",
        "typescript": ".ts"
    },

    //vscode界面字体大小设置为 18，适合喜欢大字体的用户。
    "editor.fontSize": 18,


    "security.workspace.trust.untrustedFiles": "open",




    "workbench.settings.applyToAllProfiles": [
        
    ],



    //配置要为markdown语言代替的设置.
    "[markdown]": {

        //控制自动补全功能。将 "comments"、"strings" 和 "other" 都设为 "on"，确保在 Markdown 文件中启用代码片段的触发。
    "editor.quickSuggestions": {
        "comments": "on",
        "strings": "on",
        "other": "on"
    },
    "editor.wordWrap": "on",

    //禁用 Unicode 高亮功能，以避免 Markdown 中某些特殊符号被误判。
    "editor.unicodeHighlight.ambiguousCharacters": false,
    "editor.unicodeHighlight.invisibleCharacters": false,


    "diffEditor.ignoreTrimWhitespace": false
    },




    //配置要为verilog语言代替的设置.
    //不同语言的每一条设置（如 [verilog]、[markdown] 等）确实可以与其他全局设置共享同一层级(放在最大{}里)。
    //但是，为了清晰和管理的方便，通常建议将每种语言的配置放在对应的语言块中，这样会更加整洁、易于理解，也方便以后维护。例如，所有与 verilog 相关的配置都放在 [verilog] 区域，而其他全局的配置项（如格式化、主题、快捷键等）则可以放在最大的括号中。
       
    //指定了 Verilog 的linter为 xvlog(xlinx verilog). 现在我要换成svls(system verilog).xvlog功能太少了!for循环都没有!
    //
    "verilog.linting.linter": "iverilog",

    // 启用 svls 作为语言服务器
    "verilog.languageServer.svls.enabled": true,

    // 指定 svls 的路径
    "verilog.languageServer.svls.path": "F:/aza/code_chors/linter/svls-v0.2.12-x86_64-win/target/x86_64-pc-windows-msvc/release/svls.exe",

    // 指定 svls 的参数
    "verilog.languageServer.svls.arguments": [],


    "[verilog]": {


        //设置自动缩进。将 "formatOnType" 和 "formatOnPaste" 都设为 true，表示在输入代码时和粘贴代码时都会自动格式化。
        "editor.formatOnType": true,
        "editor.formatOnPaste": true,

        "editor.defaultFormatter": "mshr-h.veriloghdl",

    },
    "verilog.linting.iverilog.includePath": [
        "F:\\devtools\\iverilog\\bin\\iverilog.exe"
    ],
    "chat.editing.alwaysSaveWithGeneratedChanges": true,
    "editor.linkedEditing": true,
    "cline.vsCodeLmModelSelector": {

        //补全信息:
        "provider": "openai", //cline 插件大多数时候是用 OpenAI API 兼容格式，SiliconFlow 也采用了类似的 API，所以设为 "openai"。 
        "apiBaseUrl": "https://api.siliconflow.com/v1/chat/completions",  
        "apiKey": "sk-rwtawlthdlsoufjwluymoshmnnhidmgvvavivehqihvpjchy",  
        "defaultModel": "deepseek-coder",
        "temperature": 0.7,  
        "maxTokens": 4096
    },
    "digital-ide.welcome.show": false,
    "remote.SSH.remotePlatform": {
        "scnet_azazel": "linux",
        "qdai.scnet.cn": "linux",
        "ksai.scnet.cn": "linux",
        "211.87.232.42": "linux"
    },



    //配置 Vim 插件的设置

    //启动easymotion插件
    "vim.easymotion": true,

    //启用增量搜索，允许你在输入搜索内容时即时高亮匹配项。
    "vim.incsearch": true,

    //允许 Vim 插件访问系统剪贴板（复制粘贴时与其他应用共享剪贴板内容）。
    "vim.useSystemClipboard": true,

    //启用 Ctrl 键的常用快捷键（例如 Ctrl + C, Ctrl + V）进行拷贝和粘贴。
    "vim.useCtrlKeys": true,

    //启用搜索高亮显示
    "vim.hlsearch": true,

    //定义在插入模式下的键绑定
    "vim.insertModeKeyBindings": [
        //按下 jj 时，会退出插入模式（相当于 <Esc>）。
        {
        "before": ["j", "j"],
        "after": ["<Esc>"]
        }
    ],

    //定义在普通模式下的键绑定：
    "vim.normalModeKeyBindingsNonRecursive": [
        {
        "before": ["<leader>", "d"],
        "after": ["d", "d"]
        },
        {
        "before": ["<C-n>"],
        "commands": [":nohl"]
        },
        {
        "before": ["K"],
        "commands": ["lineBreakInsert"],
        "silent": true
        }
    ],
    //设置leader键为空格.
    "vim.leader": "<space>",
    //此处禁用vim的C-a和C-f热键.
    "vim.handleKeys": {
        "<C-a>": false,
        "<C-f>": false
    },
    "vim.commandLineModeKeyBindings": [

    ],

    //可以防止输入中文的时候抖动
    "editor.experimentalEditContextEnabled": true,
    "editor.wordWrap": "on"

}
```

```json
{
    //by azazel. launch.json v0.1.  2025/4/20
    "version": "0.2.0",
    "configurations": [
        {
            "name": "调试当前文件...using `launch.json` v0.1 by azazel :>",           // 在 VS Code 选择调试配置时显示的名字
            "type": "cppdbg",               // 使用 C++ 调试器插件（vscode-cpptools）
            "request": "launch",            // 表示“启动”调试（还有一种是"attach"）
            
            // 下面这行很关键：调试当前打开的 cpp 文件自动生成的 .exe. `{fileDirname}`即意为“当前文件所在目录”，`{fileBasenameNoExtension}`即意为“当前文件名（不带扩展名）”
            "program": "${fileDirname}/${fileBasenameNoExtension}.exe", 
            //"program": "${workspaceFolder}/test.exe",


            "args": [],                     // 如果你需要给程序传参数，就写在这里（比如 ["arg1", "arg2"]）
            "stopAtEntry": false,           // 是否在 main 函数前暂停
            "cwd": "${fileDirname}",        // 工作目录（默认为当前源文件所在目录）

            "environment": [],              // 环境变量设置
            "externalConsole": true,        // 是否使用外部控制台运行（建议设为 true，看输出更方便）

            "MIMode": "gdb",                // 指定使用 gdb 作为调试器(你要是有lldb就填lldb...)
            "miDebuggerPath": "C:/Users/azazel/mingw64/bin/gdb.exe", // GDB 路径（怎麽查? powershell终端`Get-Command gdb`, 或者只看.source成员:`(Get-Command gdb).source`）

            "setupCommands": [              // 启用 GDB 的“漂亮打印”，让你能看清 vector/map 等复杂结构
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],

            "preLaunchTask": "编译当前文件"  // 调试前自动运行的任务. 它会调用task.json中的label为`编译当前文件`的task.
        }
    ]
}
```

```json
{
    //by azazel. tasks.json v0.1.  2025/4/20
    "tasks": [
        {
            "type": "cppbuild",

            //"label": "C/C++: g++.exe 生成活动文件",
            "label": "编译当前文件",

            "command": "C:\\Users\\azazel\\mingw64\\bin\\g++.exe",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "${file}",
                "-o",
                "${fileDirname}\\${fileBasenameNoExtension}.exe"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "调试器生成的任务。"
        },
        {
            "type": "cppbuild",
            "label": "C/C++: g++.exe 生成活动文件",
            "command": "C:\\Users\\azazel\\mingw64\\bin\\g++.exe",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "${file}",
                "-o",
                "${fileDirname}\\${fileBasenameNoExtension}.exe"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": "build",
            "detail": "编译器: C:\\Users\\azazel\\mingw64\\bin\\g++.exe"
        }
    ],
    "version": "2.0.0"
}
```







# 2.vscode的插件

## 2.1 C/C++
使用三个插件:
C/C++


## 2.2 Markdown

使用了两个插件:
markdown all in one: 提供语法高亮和实时预览.
markdown pdf: 提供界面右键选择导出.md;.html文件为.pdf;.png;.html等.

注意, 如果需要正确地导出pdf时内嵌latex公式也进行渲染, 需要对markdown pdf插件进行小改动:
* 找到markdown pdf的插件本地位置(`C://Users/<username>/.vscode/extensions/yzane.markdown-pdf-x.x.x/template/template.html
`), 改动template.html这个文件, append上这样一段:
```html
<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({ tex2jax: {inlineMath: [['$', '$']]}, messageStyle: "none" });
</script>
```
即可. 原理是啥?

markdown pdf插件首先将目标文件渲染为html, 然后才渲染为pdf/png的.在这个基础上, 插件先使用`template.html`作为模板, 将md文件内容转化后插入头部/样式/JS库. 但是markdown pdf默认模板没有添加处理$$包裹数学公式渲染的库MathJax, 需要手动添加.
追加的这一段html代码:
从"http://cdn.mathjax.org"将MathJax库下载, 然后注入html文件中. 注意这样做并不支持离线渲染哦.


如果想实现离线渲染md文件, 也可以不用这个基于html的插件, 使用**pandoc**. 这个程序直接**调用你本地的latex编译器**. 先下载pandoc这个程序, 然后在vscode里使用vscode-pandoc调用. 当然也可以直接命令行`pandoc myfile.md -o output.pdf --pdf-engine=xelatex` . 还没尝试过以后再说吧.






