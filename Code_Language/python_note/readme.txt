this computer has install python at:


C:\Users\azazel>where python
C:\Users\azazel\Anaconda3\python.exe
C:\Users\azazel\AppData\Local\Microsoft\WindowsApps\python.exe

as you can see the first path is real. the second path
"...\Local\Microsoft\WindowsApps\python.exe"
is just" Windows 应用商店版本的 Python 占位符（可能并没有真正安装 Python）。
如果你在 Windows 应用商店里安装过 Python，这个路径可能会有效，但通常不建议使用它。"

plus: anaconda安装了多个版本的python. 默认版本base现在是3.11. 还有什麽test39, yolo两个版本, 以前用在两个项目上的时候装的.


python中的pip工具是管理第三方库的, 用pip install xxx和pip show xxx安装和查看是否存在某个第三方库. 但是standard library自带, 不能pip, 会报错而且不提示为啥. 
so, 遇到问题先确定一下是否是标准库还是第三方库.





