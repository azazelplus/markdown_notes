- 
```cpp
#include <iostream>
using namespace std;//如果不声明使用的函数命名空间,则需要在使用std内的函数和保留字时格式为:std::cin >>n    std::endl(类似于python中的是否import 某函数  from 某module一样.)
```



### 常量
C++定义常量const int a=3.14和C中的预处理#define a 3.14不同:后者在编译预处理时把整个程序的a全部替换成3.14,a不存在了,不是一个只读变量名.(不过实际使用区别不体现.)







### 条件运算符:
(条件表达式)?(为真时表达式的值):(为假时表达式的值)
如x==3?0:1










### I/O流控制    	
流,即输入或输出的那一系列字节.
```cpp
cin >> n  //等价于scanf(%d,&n)
cout << a<< b<<c   //等价于printf("%d%d%d",a,b,c),其中a,b,c也可以为"\n"即换行,等价于endl(保留字end of line)
C++默认的流输出数值有效位为6.
输出流的格式控制:用控制符manipulators实现,需要#include<iomanip>
int pi=3.14159;
cout<<setprecision(2)<<pi;//setprecision(n)设置后方流的输出有效数字.结果为两位有效位输出得3.1
cout<<setw(n)<<setfill('char')<<pi;//按照n宽度输出,用char(默认为空格)补充空位.若要输出的值本身超过了n,则放弃该格式控制而输出整个值(这点和py的{:*5}.format(pi)一样.)
cout<<setfill('*')
       <<setw(10)
       <<pi
```
输






* n++表达式若看作一个函数,进行的是n=n+1,返回值是n(相加前的数)
* ++n表达式做看作一个函数,进行的是n=n+1,返回值是n+1(相加后的书)

### 头文件
* 在C中.h去掉前面加上c即可,如:<string.h>在c++中换成<cstring>

### 循环变量
c++中允许在for循环中定义循环变量,即可以写
```cpp
for(int i=0;i<5;i++){}
```     
这点和python中
```python
for i in range(n)很像.
```
string类型字符串变量:
是动态数组，可以声明string a;
和python中的char类似,允许进行拼接a+b
函数getline(test_string)相当于gets_s(a),a为对应C中的字符串数组.

字符串面向对象:
```str.length()```返回字符串str的长度,一个整数.
```str.substr(m[,n])```截取:返回字符串str的序号为m位开始,长度为n(默认则包含后面所有的)的子串.


C++允许在定义结构体```struct Student{xxxx}```后,直接```Student A```;来创建结构体变量而不加`struct`关键字.



### 察看变量类型:
`typeid(d).name()`返回值是一个`char const *`
当d为int/double/char时打印出来是int/double/char
当d为string时打印出来```class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >一大坨...```




### 类class
* 注意类成员函数在类外声明时,需要写在类的下方,写在类的上方会报错.
#### 构造函数&析构函数:
- 是类的特殊成员函数.
- 函数名和class名相同, 没有返回值.
- 在创建对象时自动调用该函数.
##### 类的构造函数写法:(这是为了方便初始化)
```cpp
class Student
{
public:
    Student(string _name="null",int _age=0,int _score=0)	//类的构造函数(方便初始化)
    {
        name = _name;
        age = _age;
        score = _score;
    }
    void display()	//展示打印信息的函数
    {
        cout << "name=\"" << name << "\""
            << ",age:" << age
            << ",score:" << score;
    }
protected:
    string name;
    int age;
    int score;
};
```
然后我们可以对一个Student Student1("mike",19,100);来初始化声明一个对象Student1.

* 注意类成员函数在类外声明时,需要写在类的下方,写在类的上方会报错.






### 指针和引用
在指针定义中,一个*只能表示一个指针.即``int * ptr1,ptr2;``
该语句声明了一个整形指针ptr1和一个整形ptr2.









### 堆heap
C++程序的内存格局通常分为四个区:
>**全局数据区data area**		存放全局变量,静态数据,常量
>**代码区code area**		存放类成员函数,非成员函数
>**栈区stack area(弹夹	)**	存放运行函数分配的局部变量,函数参数,返回数据,返回地址
>**堆区(自由存储区)heap area(队列)**	






### 函数system 
功能： 发出一个DOS命令 
用法： ```int system(char *command);```
system函数已经被收录在标准c库中，可以直接调用。
例:```system("pause");```该语句使程序在此处暂停，按任意键继续.





### 双冒号::
叫做用域区分符.指明一个函数/数据属于哪个类.






### typedef
为一个已有的类型名提供同义词.
例:
```cpp
typedef int INT,integer;//这样就可以用INT或者integer来代替int啦
```





### 注意事项
对于数组,结构体(数组)等(连续的一段内存空间)当作形参传入函数时,使用其开头指针(一个结构体名字就是他的第一个成员变量的地址,一个结构体数组的名字就是它第一个结构体的第一个成员变量的地址.)就可以了.
例:
```cpp
struct Stu
{
string name;
int score;
};	//声明结构体

 Stu Test[5]={ {"jone",123},{"david",144},{"jasen",91},{"peter",120},"mike",150};	//初始化一个结构体数组
//构建一个显示该类型结构体数组信息(显示学生名单)的函数
void show(Stu* stu,int n)	//传入一个Stu类型的结构体变量指针stu和数组长度n
{
	for(int i=0;i<n;i++)
		{
		cout<<stu[i].name<<" "<<stu[i].score<<endl;
		}
}

show(Test,5);	
/*该语句可以打印出:
jone 123
david 144
jasen 91
peter 120
mike 150
*/	
```

#### cin
cin慢是有原因的，其实默认的时候，cin与stdin总是保持同步的，也就是说这两种方法可以混用，而不必担心文件指针混乱，同时cout和stdout也一样，两者混用不会输出顺序错乱。正因为这个兼容性的特性，导致cin有许多额外的开销，如何禁用这个特性呢？只需一个语句
```std::ios::sync_with_stdio(false);```
这样就可以取消cin于stdin的同步了。



* 注意按值传递时,参数列表有一个int a,可以写成const int &a.对于一个很大的vector更经常如此:本来可以写成





⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️注意事项^^^^


### 继承
//现有学生作为父类，研究牲作为子类。
class Student
{
//...
};

class GraduatStudent:public Student//继承了父类的结构和函数
{
}





### 重载

运算重载符

example://引例:定义了一个复数类,欲实现复数的"+"运算.
struct Complex
{
double real,image;
//进行运算符重载.
bool operator < (const Student &)
};





### STL
#### 容器
通用的方法:
容器.size()返回容器的大小.


#### list(链表)





####　pair()
当想要将两个元素绑在一起作为一个合成元素,又不想要因此定义结构体时,使用 pair 可以很方便地作为一个代替品.
在头文件#include<iostream>中.
创建:pair<数据类型first,数据类型second> mypair[50];//创建了一个全是(0,0)的pair数组.
访问:mypair[i].first或second;//表达式返回的是相应的第一个元素或者第二个元素.一般为了方便可以#define x first;#define y second;


### vector(动态数组,可变长数组)
在头文件#include<vector>中.
创建:vector <int> a;		//建立一个空的int可变数组a,此时a.size()=0
vector <int> a(n[,x0])		//建立一个size为n的都为x0(默认为0)的数组.
a[1]=5;//动态数组的访问和数组一样.
vector的方法:
a.size()			//返回vector的大小
a.resize(n);		//重新分配数组大小,此时a.size()=n
a.push_back(新元素值);	//压入一个新元素，size也+1
*二维vector，即vector<vector<int>>erweiv;
*vector的传递：直接传递vector的名字即可实现按值传递.如果用const vector<int>& myvec则为常引用,在函数体内参数变量是只可读的(适用于写类似void show()的函数.)


### map(地图,键值对)
在头文件#include<map>中;map内部有序(自动排序，单词时按照字母序排序),查找时间复杂度为O(logn)。
创建: map<键数据类型,值数据类型> mymap;//该语句创建一个键值对数据类型确定的空的map.
操作方法:
mymap.insert(key,value);//往mymap中加入一个(key,value)的键值对.
mymap.find(key);//该表达式返回mymap中键key对应的value.



### priority_queue(优先序列,也称为堆,一个容器.本质是用一个数组去模拟的完全二叉树.可以拿出优先级最大的元素.)
头文件为#include<queue>
定义:priority_queue <数据类型,容器类型,比较方法>,默认容器类型为vector<数据类型>,默认比较方法为less<数据类型>
greater<数据类型>则为小顶堆.
priority_queue <int> myque;	//生成了一个int类型大顶堆,即值越大优先级越高.
priority_queue <int,vector<int>,greater<int>> myque;	//生成了一个小顶堆.
操作方法:
myque.size()	//返回长度
myque.push(x)	//插入元素x
myque.pop()	//删除优先级最高的元素(称为堆顶.)
myque.top()	//访问堆顶(返回堆顶元素)
myque.empty()	//判断是否为空.



### 容器的for加强循环：
C++提供类似python的容器迭代方式,即for(循环变量:容器)
```cpp
//字符串的遍历:
string str="DieScatten";
for(char i : str)	//类似python中的for char in str:
{
	cout<<i<<" ";
	i="a"	//不能修改,没有用的语句
}
for(char& i : str)	//在声明时使用的&为[引用运算符],任何其他&的使用是地址操作符.在这里实际上是每次遍历都声明了str内一个字符的引用i,并操作i.
{
	cout<<i<<" ";
	i="a"	//将字符串str变成aaaaaaaaaa
}
```
同理,vector的遍历也类似.

迭代器:		//提供一个对任何容器统一的遍历方式.












###　常用头文件：
```cpp
#include<bits/stlc++.h>	//万能头文件
#include<iostream>
#include<string>
#include<stdio.h>
#include<string.h>
#include<queue>
#include<algorithm>
using namespace std
```

### sort函数介绍
(void) sort (起始地址,结束地址[,比较器])

例1(数组):
int arr[10]={5,3,6,0,2,8,2,6,9,11};
sort(arr,arr+10);

例2(可变数组):
vector <int> vec={5,3,6,0,2,8,2,6,9,11};
sort(vec.begin(),vec.end());	// 可变数组名.begin()/.end()表达式给出数组第一个元素和最后一个元素的地址.

例3⚠️(结构体):实现对结构体多个成员变量的多关键字排序,需要用到sort的第三个参数,一个布尔值.
```cpp
//声明结构体
struct List
{
int a;
int b;
string c;
};
//欲对StuList[50]进行a为第一关键字,b为第二关键字,c为第三关键字升序排序.
为了获得第三个参数,需要比较函数:
bool cmp(List A,List B)
{
if (A.a!=B.a)
	{return A.a<B.a;}
else
	{
	if(A.b!=B.b)
		return A.b<B.b;
	else
		{return A.c<B.c;}
	}
}
```
此时使用命令sort(StuList,Stulist+50,cmp);即可.第三个参数是比较函数的名字,不用加括号.


## 算法
### 贪心算法

为什么贪心是正确的？

#### Exchange Argument（交换论证法）
- (问题类型:消耗体力去交易行动(每个行动为proji(input(消耗体力),output(获得报酬)==c)),取得最大output.)||每个行动收益等价,取最简单的行动开始.
假设proji被选入了当前方案，而projj没有被选入当前方案，且outputi>outputj，此时总的体力花费为Δx
交换proji、projj，使得projj被选入而proji没有，此时方案中output保持不变，但是体力花费变为了Δx＋(inputi-inputj)<Δx
发现方案变得更好了!

- (问题类型:消耗体力去交易行动(每个行动为proji(input==c(消耗体力),output(获得报酬))),取得最大output.)||每个行动付出等价,取收获最高的开始.

- (问题类型:消耗体力去交易行动(每个行动为proji(input==c(消耗体力),output(获得报酬))),取得最大output.)||每个行动付出等价,取收获最高的开始.

- (问题类型:求最小代价,每个元素有(处理该元素用时,等待的代价),需要排出所有元素.)
假设挨在一起最前面的两个A1(t1,cost1),A2(t2,cost2)(类似冒泡排序,这样做的重点是这两个元素交换不影响后面的元素产生代价.),交换前这两个元素会产生代价t1cost2,交换后产生代价t2cost1,从而我们得知用每个元素的t/cost作为判据排序处理.

- (问题类型:集合问题)
在一个数轴上取一段距离，有一些线段，希望得到线段放进去最多的放法
从左往右放线段.每放入一个线段后删除掉不合要求的->剩下的右边最短选择第二个线段.

- (问题类型:求最大的最小值)
一条数轴有起点0点和终点n+1点,中间有n个点.欲去掉m个点,使剩下的n+1-m个间隔的最小值(关于去掉m个点的方法)最大.
解:用贪心策略。让每次选取的位置后后面的距离尽可能的长，而判断最大最小距离就用枚举法枚举出所有距离找到符合条件的最大最小值。但是普通枚举所有距离时间复杂度太长，因此我们用二分法枚举，当距离太长时则缩小距离，距离太短则放大距离。













### 二分查找
代码实现：
```cpp
#include <iostream>
#include <algorithm>
using namespace std;
//二分查找：在数组a中查找等于k的ai的i索引值。(如果数组a没有排序,则sort(a,a+n);即可.)
//要查的数组为a[],升序,长度为n,要寻找的值为k.
int binary_search(int a[],int n,int k) {
	int left = 0, right = n - 1,mid;
	bool find=false;			//棋子

	while (left<=right)//当左边界小于右边界时继续查找
	{
		mid = (left + right) / 2;
		if (a[mid] > k)//当中间值大于k时,则往左边找
		{
			right = mid-1;
			mid = (left + right) / 2;

		}
		if (a[mid] < k)//当中间值小于k时往右边找
		{

			left = mid+1;
			mid = (left + right) / 2;

		}
		if (a[mid] == k)//找到了
		{
			find = true;
			return mid;
		}
	}
	//当left>right还没返回时,查找结束,没找到
	return -1;
}
void main()
{
	int a[50];
	int n,k;
	cin >> n>>k;
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}
	cout<<binary_search(a, n, k);
	//测试用例:
	//5 16
	//1 13 16 40 88
	//运行输出2即为16的正向递增序号.
}
```


### stl提供模板：
 binary_search(arr[],arr[]+size,index)	//第一个参数传要查找的数组(也就是开头指针)，第二个传数组+size(也就是末尾指针),第三个参数是要查找的值.
lower_bound()

upper_bound





### 二分答案
最小的最大值/最大的最小值问题,需要把复杂度从n变为log
经典例题:农民约翰有 C ≤ 105 只牛，然后他有 n ≤ 105 个隔间，每个隔间都有自己的 x
坐标位置，如何安排把牛安排进隔间才能使牛两两之间距离的最小值最大，求
最大的最小值.(n个点中选c个点,使得每两个点之间的最小距离最大,求这个最大的最小值.)
考虑最终答案为最大的最小值x,则:大于x时,选不出c个点,小于x时需要更多点补充.
造一个check函数:贪心地选,判断能否选够c个点
()

###　分巧克力:
有N块巧克力,长宽为Li,Wi,
切出k块边长为l的正方形巧克力,求l的最大值.
可能的边长为1~(Li和Wi的最小值),二分可能的边长.
判断的check函数:





### DP




