# python语法和数据结构

### tuple & list & mapsto

()内的是tuple. []内的是list. {}内的是map.
tuple和list可以相互转化. 反正都是列表, 前者不可变而已.
表示tensor的shape的时候, 可以用tuple也可以用list: 这个tuple/list的每一维数字表示tensor的一个角标的取值.(见pytorch章节的tensor)

比如使用工厂函数new_ones()时:
x.new_ones([8, ])表示创建一个一维的分量为8的每个值都是1的tensor. 它来自tensor`x`, 意味着它的数据类型和设备都和`x`相同.

`8, ]`表示一个列表, 只有`8`这一个元素.

`[x_T.shape[0], ]`表示一个列表, 只有`x_T.shape[0]`这一个元素.


### with 上下文管理器
with语句用于创建一个**上下文管理器**:
```python
with expression as variable:
    #使用 variable进行一些操作
    ...
```

最常见的用法是处理文件操作, 例如:
```python
with open("myfile.txt","w") as file:
    file.write("hello,world!")#这一句话即with语句块.

#open("myfile.txt","w")返回一个文件对象, 方式为writable可修改. w模式下, 如果当前目录中没有该名字的文件, 会创建一个新空文件, 并且将其返回.
#with 将这个文件对象赋给变量名file, 执行语句块后, 删除这个变量名, 很简洁.
#当with语句块结束时, 文件对象会自动关闭, 无需显式调用file.close().
```


可以自定义一个上下文管理器. 原理上, 只要能实现**打开文件**和**退出文件**两个**方法**即成为了一个上下文管理器.
```python
class MyContextManager:
    def __enter__(self):
        print("entering the context")
        return self

    def __exit__(self. exc_type, exc_value, traceback):
        print("exiting the context")

with MyContextManager() as manager:
    print("inside the contest")

```


### f-string
这是 Python 中的一种字符串格式化方式，称为 f-string 或 格式化字符串字面量。它在 Python 3.6 中引入，用于在字符串中嵌入表达式，使得字符串格式化更加简洁和直观。

#### f-string 的作用:
- 插值: f-string 允许在字符串中直接嵌入变量或表达式，并自动格式化它们的值。
- 简洁: 相比传统的字符串格式化方法（如 % 操作符或 str.format() 方法），f-string 更加简洁和易读。
#### 语法
f-string 通过在字符串前加一个小写字母 f 来表示。变量或表达式被放在大括号 {} 中。这些表达式在运行时被计算，并将结果嵌入到字符串中。

#### 示例
```python
name = "Buddy"
age = 4

#### 使用 f-string 格式化字符串
message = f"{name} is {age} years old."

print(message)  # 输出: Buddy is 4 years old.
```
在这个例子中：
f: 表示这是一个格式化字符串字面量（f-string）。
{name}: 表示 name 变量的值将被插入到这个位置。
{age}: 表示 age 变量的值将被插入到这个位置。

#### 复杂表达式
f-string 还支持嵌入更复杂的表达式和函数调用：

```python
import math

radius = 5
area = math.pi * radius ** 2

#### 嵌入计算表达式
message = f"The area of a circle with radius {radius} is {area:.2f}."

print(message)  # 输出: The area of a circle with radius 5 is 78.54.
```
{radius}: 直接嵌入变量的值。
{area:.2f}: 嵌入计算结果，并格式化为小数点后有两位数字。




























































### 迭代器iter
在python中特有的一个对象.
迭代器是实现了 迭代器协议 的对象，这意味着它必须实现以下两个方法：

- __iter__(): 该方法返回迭代器对象本身。这使得对象可以被多次遍历。
- __next__(): 该方法返回容器的下一个元素。当容器内没有更多元素时，应当引发 StopIteration 异常，结束迭代.  (其实next(iterator) 和 iterator.__next__() 的效果是完全相同的，都是从迭代器中获取下一个元素。next()是python的内置迭代器函数.)
- 
- **可迭代对象（Iterable）:**

任何可以返回一个迭代器的对象都称为可迭代对象。常见的如列表、元组、字符串、字典、集合, 还有迭代器本身等。
可迭代对象拥有 __iter__() 方法，通过调用该方法返回一个迭代器。
例如:
```python
# 创建一个可迭代对象（如列表）
my_list = [1, 2, 3, 4]

# 获取该对象的迭代器
iterator = iter(my_list)

# 使用迭代器逐个访问元素
print(next(iterator))  # 输出: 1
print(next(iterator))  # 输出: 2
print(next(iterator))  # 输出: 3
print(next(iterator))  # 输出: 4

# 当没有更多元素时，再调用 next() 会引发 StopIteration 异常
# print(next(iterator))  # 会引发 StopIteration 异常
```

- 迭代器的特点
    - 惰性计算:

    迭代器会在每次调用 next() 时生成一个值，并不会像列表那样一次性加载所有元素，这使得迭代器特别适合用于处理大量数据或无限序列。
    - 一次性:

    迭代器在遍历完成后不能重置或重新开始遍历。
    - 轻量级:

迭代器只在需要时才生成元素，因此占用的内存比列表等数据结构少。






## python项目的架构(以项目文件夹A为例)
一个(可能有两三个?)子文件夹(一般也叫A)里存了很多.py, 为项目主体. 可以理解成C中的各种.c, .h要被调用.
同时还有readme, setup.py(它的作用同requirement.txt)
在含有setup.py的目录下运行`pip install -e .`. pip命令自动运行当前目录(`.`)的`-editable`(开发模式), “可编辑”模式的意思是 pip 并不是把源码的副本复制到你的 Python 包目录下，而是建立一个链接（通常是一个符号链接或引用），直接指向你当前的源码目录。这有几个好处：

即时更新： 如果你修改了源码，这些改动会立刻在你安装的包中生效，无需重复安装。

开发调试： 对于正在开发和调试中的项目，这种方式非常方便，因为你可以直接编辑源码，然后运行程序看到最新效果。


### 包（Package）

包是一个文件夹，其中包含一个特殊的 `__init__.py` 文件（在 Python 3.3 以后可以省略，但仍然常用），以及一个或多个模块。包允许你以层次化的方式组织和管理代码，就像 C 语言中的文件夹结构一样。例如，一个包可能包含多个相关的功能模块。

### 库（Library）：
库通常是指一个完整的、提供某种功能的代码集合，这个集合可以由多个包、模块、工具和文档组成。库是一种更大的概念，它可能包含一个或多个包。例如，`numpy` 可以看作是一个库，其中包含多个模块和可能的子包，提供数值计算功能。

# pytorch框架

## 1.张量

一个shape为[k1,k2,k3,k4]的4维(rank)张量
数学意义上, 它们都是有4个下角标的四维张量:
$$x_{abcd}, \\其中a=1,...,k1; \\b=1,...,k2; \\c=1,...,k3; \\d=1,...,k4.$$

不过注意事实上编程的时候所有角标都-1, 因为python中角标是从0开始数的.

也就是说实际上是一个映射:
$$
    \mathbb{R}^4  \to  \mathbb{R}, \\
    (a,b,c,d,e) \mapsto  x_{abcde}
$$

其代码表示很丑.采用嵌套方式:
tensor(
    [(第一维度)
        [第二维度
            ...
        ]
    ]
)

例如, 一个shape为[2,1,1,1]的张量, 实际上只有$x_{1111}和x_{2111}$两个分量. 即:
```python
[
    [[[x_1111]]]          #1xxx
    [[[x_2222]]]          #2xxx
]
```

又如, 一个shape为[3,2,2,4]的张量格式:
```python
[
    [
        [[.][.][.][.][.]]

        [[.][.][.][.][.]]       
    ]

    [
        [[.][.][.][.][.]]

        [[.][.][.][.][.]]       
    ]
               
    [
        [[.][.][.][.][.]]

        [[.][.][.][.][.]]       
    ]       
]




## 🧠 首先：“保存模型参数”到底是指什么？

在 PyTorch 里，当你训练一个模型时，其实你是在「修改」模型内部的参数，让它能更好地做出预测。比如卷积核、权重、偏置项... 都是参数。

这些参数就是一堆 **tensor（张量）**，你可以用：

python

CopyEdit

`model.state_dict()`

来访问这些参数，看到的是一个 `OrderedDict`，形如：

python

CopyEdit

`OrderedDict([   ('layer1.weight', tensor(...)),   ('layer1.bias', tensor(...)),   ... ])`

👉 所以所谓“保存模型”，指的是：

> 把这些 **训练好的参数（state\_dict）** 存到磁盘上，以后可以加载回来继续用。

* * *

## 📦 示例：保存 & 加载模型参数

python

CopyEdit

`# 保存 torch.save(model.state_dict(), 'my_model.pth')  # 加载 model.load_state_dict(torch.load('my_model.pth')) model.eval()  # 如果是推理用，记得切成 eval 模式`

* * *

## 🔧 那什么叫底层？我们可以一步步拆：

### 🧩 PyTorch 模型本质是什么？

PyTorch 里的模型通常是继承 `nn.Module` 的一个类，比如：

```python
class MyNet(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(784, 128)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(128, 10)

    def forward(self, x):
        x = self.fc1(x)
        x = self.relu(x)
        return self.fc2(x)
```
这个 `MyNet` 其实是一个 **有结构的计算图容器**，它：

-   包含了一堆「模块」(比如 Linear, ReLU)
    
-   每个模块有参数 (比如权重 weight、偏置 bias)
    
-   所有参数都被记录在 `state_dict` 中
    

* * *

## 2.模型的保存和加载

### 保存: `torch.save(my_model.state_dict(), 'model_weights.pt')`

对于一个模型实例, 比如

```python
class UNet(nn.net)

my_model = UNet(parameters...)
```

`my_model.state_dict()` 方法返回一个字典, 包含该模型中所有参数(权重w+偏置b).
`torch.save`方法用于保存这个参数文件.

```python
torch.save(my_model.state_dict(), 'model_weights.pt')
```
该命令将你的模型的参数字典保存到磁盘上，保存在文件 model_weights.pt 中。

### 加载: `my_model.load_state_dict(model_weights)`

先使用.load方法:
```python
model_weights = torch.load('model_weights.pt', map_location=device)
```
先使用该命令, 将权重文件`model_weights.pt`中的内容加载到内存中的一个变量`model_weights`中，以便后续使用。 

其中第二个参数map_location用来控制加载时将模型权重加载到哪个设备上。如果是GPU, 那麽这个变量`model_weights`的地址在显存上.


然后使用.load_state_dict方法:
```python
my_model.load_state_dict(model_weights)
```
该命令将`my_model`模型的参数用`model_weights`加载. `.load_state_dict`会检查模型中每个参数名，并将对应的权重载入到正确的参数位置。如果模型的结构完全匹配，就会成功应用这些参数。如果有不匹配的参数（例如模型结构改变了），则会报错。



### ✅ `model.state_dict()`：

-   返回模型中所有的 **训练参数 + buffer值**（包括 `register_buffer` 注册的）
    

比如：

```python
net = nn.Linear(3, 1)
print(net.state_dict())
# OrderedDict([('weight', tensor([...])), ('bias', tensor([...]))])
```

### ✅ `model.load_state_dict(...)`：

-   把保存好的参数重新「塞回」模型里。
    

* * *

## 🤖 那么训练函数和保存有什么关系？

你训练的过程只是：

> 用数据更新 `model.parameters()` 中的值

而保存模型是：

> 把当前 `parameters()` 的值写进文件。

比如你训练完一轮后这样写：

python

CopyEdit

`train_one_epoch(...) torch.save(model.state_dict(), 'after_one_epoch.pth')`

* * *

## ✨ BONUS：为什么需要 `register_buffer()`？

因为不是所有重要的数据都要训练，比如：

-   扩散过程中的 `alphas_bar`
    
-   BatchNorm 的 running mean 这些变量不训练，但需要被保存（state\_dict 里也能找到它们！）





#