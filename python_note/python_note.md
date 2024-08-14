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