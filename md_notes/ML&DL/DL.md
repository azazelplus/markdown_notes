
## 一个简单的神经网络例子.

### 简单神经网络例子：前向传播与反向传播

假设我们有一个非常简单的两层神经网络：

- **输入层** $x = (x_1, x_2)$
- **第一层**：有两个神经元，权重为 $W_1 = \begin{pmatrix} w_{11} & w_{12} \\ w_{21} & w_{22} \end{pmatrix}$，偏置为 $b_1 = (b_{11}, b_{12})$
- **输出层**：有一个神经元，权重为 $W_2 = (w_{31}, w_{32})$，偏置为 $b_2$

损失函数 $L$ 是均方误差（MSE），假设真实标签是 $y$，我们要最小化损失 $L = \frac{1}{2} (\hat{y} - y)^2$，其中 $\hat{y}$ 是网络的预测值。

### 1. 前向传播：

#### 输入层到第一层的计算：

假设输入是 $x = (x_1, x_2) = (1, 2)$，权重为：
$$
W_1 = \begin{pmatrix} 0.5 & -0.5 \\ 0.3 & 0.8 \end{pmatrix}, \quad b_1 = (0.1, -0.2)
$$

第一层的计算是：
$$
z_1 = W_1 \cdot x + b_1 = \begin{pmatrix} 0.5 & -0.5 \\ 0.3 & 0.8 \end{pmatrix} \cdot \begin{pmatrix} 1 \\ 2 \end{pmatrix} + \begin{pmatrix} 0.1 \\ -0.2 \end{pmatrix}
$$
$$
z_1 = \begin{pmatrix} 0.5 \cdot 1 + (-0.5) \cdot 2 + 0.1 \\ 0.3 \cdot 1 + 0.8 \cdot 2 - 0.2 \end{pmatrix} = \begin{pmatrix} -0.9 \\ 1.7 \end{pmatrix}
$$

然后应用激活函数（例如 ReLU）：
$$
a_1 = \text{ReLU}(z_1) = \begin{pmatrix} \max(0, -0.9) \\ \max(0, 1.7) \end{pmatrix} = \begin{pmatrix} 0 \\ 1.7 \end{pmatrix}
$$

#### 第一层到输出层的计算：

输出层的计算是：
$$
z_2 = W_2 \cdot a_1 + b_2 = (0.2, -0.4) \cdot \begin{pmatrix} 0 \\ 1.7 \end{pmatrix} + 0.5
$$
$$
z_2 = (0.2 \cdot 0 + (-0.4) \cdot 1.7) + 0.5 = -0.68 + 0.5 = -0.18
$$

然后应用激活函数（假设没有激活函数，即线性输出）：
$$
\hat{y} = -0.18
$$

#### 损失函数计算：

真实标签 $y = 1$，所以损失是：
$$
L = \frac{1}{2} (\hat{y} - y)^2 = \frac{1}{2} (-0.18 - 1)^2 = \frac{1}{2} (1.18)^2 = 0.697
$$

### 2. 反向传播：

现在我们需要计算损失函数 $L$ 对每个参数（如 $w_{11}, w_{12}, w_{31}$ 等）的偏导数。

#### 步骤1：计算输出层的梯度

首先，计算损失函数 $L$ 对输出 $\hat{y}$ 的导数：
$$
\frac{\partial L}{\partial \hat{y}} = \hat{y} - y = -0.18 - 1 = -1.18
$$

然后，计算损失对 $z_2$ 的导数（因为 $z_2$ 是计算 $\hat{y}$ 的输入）：
$$
\frac{\partial L}{\partial z_2} = \frac{\partial L}{\partial \hat{y}} \cdot \frac{\partial \hat{y}}{\partial z_2} = -1.18 \cdot 1 = -1.18
$$

计算损失对权重 $W_2$ 的梯度：
$$
\frac{\partial L}{\partial W_2} = \frac{\partial L}{\partial z_2} \cdot a_1^T = -1.18 \cdot \begin{pmatrix} 0 \\ 1.7 \end{pmatrix} = \begin{pmatrix} 0 \\ -2.006 \end{pmatrix}
$$

计算损失对偏置 $b_2$ 的梯度：
$$
\frac{\partial L}{\partial b_2} = \frac{\partial L}{\partial z_2} = -1.18
$$

#### 步骤2：计算第一层的梯度

我们需要计算第一层的梯度。首先，计算损失对 $a_1$ 的导数：
$$
\frac{\partial L}{\partial a_1} = W_2^T \cdot \frac{\partial L}{\partial z_2} = \begin{pmatrix} 0.2 \\ -0.4 \end{pmatrix} \cdot (-1.18) = \begin{pmatrix} -0.236 \\ 0.472 \end{pmatrix}
$$

然后，计算损失对 $z_1$ 的导数（因为 $z_1$ 是 $a_1$ 的输入）：
$$
\frac{\partial L}{\partial z_1} = \frac{\partial L}{\partial a_1} \cdot \frac{\partial a_1}{\partial z_1}
$$

因为我们使用的是 ReLU 激活函数，$\frac{\partial a_1}{\partial z_1}$ 会是 1 或 0。对于 $a_1 = \begin{pmatrix} 0 \\ 1.7 \end{pmatrix}$，我们有：
$$
\frac{\partial L}{\partial z_1} = \begin{pmatrix} 0 \\ 0.472 \end{pmatrix}
$$

计算损失对权重 $W_1$ 的梯度：
$$
\frac{\partial L}{\partial W_1} = \frac{\partial L}{\partial z_1} \cdot x^T
$$

假设输入 $x = \begin{pmatrix} 1 \\ 2 \end{pmatrix}$，那么：
$$
\frac{\partial L}{\partial W_1} = \begin{pmatrix} 0 \\ 0.472 \end{pmatrix} \cdot \begin{pmatrix} 1 & 2 \end{pmatrix} = \begin{pmatrix} 0 & 0 \\ 0.472 & 0.944 \end{pmatrix}
$$

计算损失对偏置 $b_1$ 的梯度：
$$
\frac{\partial L}{\partial b_1} = \frac{\partial L}{\partial z_1} = \begin{pmatrix} 0 \\ 0.472 \end{pmatrix}
$$

### 3. 更新参数：

根据计算的梯度，可以使用梯度下降法来更新参数：
$$
W_1 \leftarrow W_1 - \alpha \cdot \frac{\partial L}{\partial W_1}, \quad W_2 \leftarrow W_2 - \alpha \cdot \frac{\partial L}{\partial W_2}, \quad b_1 \leftarrow b_1 - \alpha \cdot \frac{\partial L}{\partial b_1}, \quad b_2 \leftarrow b_2 - \alpha \cdot \frac{\partial L}{\partial b_2}
$$





## 归一化层



### **1\. 什么是 batch？**

在神经网络训练中，我们通常不会一次性输入整个数据集，而是**分批（batch）输入**，这样可以提高计算效率，并利用梯度下降优化参数。

#### **举个例子**

假设你有一个 10000 张图片的数据集：

-   **全数据（全部样本）训练**：把 10000 张图片一次性输入神经网络进行训练，这会导致内存消耗过大。
-   **小批量（mini-batch）训练**：把数据集拆分成多个小批次，比如每次输入 32 张图片（batch size = 32）。
-   **单样本（stochastic）训练**：每次只输入 1 张图片（batch size = 1）。

batch 就是**一次输入神经网络的一组样本**，例如：

-   如果 batch size = 32，表示**每次训练输入 32 张图片**。
-   如果 batch size = 64，表示**每次训练输入 64 张图片**。

batch 主要用于：

1.  **减少内存占用**，避免一次性处理整个数据集。
2.  **加速梯度计算**，因为 mini-batch 计算均值和梯度更加稳定。
3.  **提高优化器的效果**，像 SGD（随机梯度下降）在 mini-batch 里计算梯度时更加高效。

* * *

### **2\. 什么是归一化层？（Normalization Layer）**

归一化（Normalization）就是对输入数据进行标准化，使其数值分布更加均匀，从而**加速收敛，提高泛化能力**。

#### **2.1 为什么要归一化？**

神经网络训练时，输入的数据如果数值范围相差太大，网络容易出现以下问题：

-   **梯度消失/梯度爆炸**：如果输入数据过大或过小，经过多个层传递后，梯度可能会变得很小（学习变慢）或很大（不稳定）。
-   **训练不稳定**：不同层之间的数值分布变化过大，导致训练时权重更新不均衡。

归一化层的作用是：

1.  **调整数据分布**，让输入数据均值接近 0，方差接近 1，避免梯度问题。
2.  **加速收敛**，让训练更快、更稳定。
3.  **减少对初始化的敏感性**，让神经网络更容易训练。

* * *

### **3\. 归一化层的几种类型**

归一化层的核心思想是**对输入数据进行标准化**，常见的归一化方法有：

1.  **Batch Normalization（BN）**（按 batch 归一化）
2.  **Instance Normalization（IN）**（按单个样本归一化）
3.  **Layer Normalization（LN）**（按特征层归一化）
4.  **Group Normalization（GN）**（按多个通道归一化）

#### **3.1 Batch Normalization（BN，批量归一化）**

**归一化范围**：在一个 batch 里的**相同通道上**计算均值和标准差。

假设 batch size = 32，每个样本是 3 通道（RGB）的 32×32 图片：

-   计算**整个 batch 里的每个通道**的均值和标准差。
-   **所有样本在同一个通道上的数值会一起归一化**。

##### **数学公式**

给定某个 batch 的输入 xn,c,h,wx\_{n,c,h,w}xn,c,h,w​，它表示：

-   第 nnn 个样本（batch size 内）
-   第 ccc 个通道（如 RGB 的 R 通道）
-   高度 hhh，宽度 www

BN 计算公式：

1.  **计算均值和标准差（在 batch 维度上计算）**
    $$
    μc\=1N⋅H⋅W∑n\=1N∑h\=1H∑w\=1Wxn,c,h,w\\mu\_c = \\frac{1}{N \\cdot H \\cdot W} \\sum\_{n=1}^{N} \\sum\_{h=1}^{H} \\sum\_{w=1}^{W} x\_{n,c,h,w}μc​\=N⋅H⋅W1​n\=1∑N​h\=1∑H​w\=1∑W​xn,c,h,w​ σc2\=1N⋅H⋅W∑n\=1N∑h\=1H∑w\=1W(xn,c,h,w−μc)2\\sigma\_c^2 = \\frac{1}{N \\cdot H \\cdot W} \\sum\_{n=1}^{N} \\sum\_{h=1}^{H} \\sum\_{w=1}^{W} (x\_{n,c,h,w} - \\mu\_c)^2σc2​\=N⋅H⋅W1​n\=1∑N​h\=1∑H​w\=1∑W​(xn,c,h,w​−μc​)2
    $$
    -   **计算整个 batch 的某个通道的均值和方差**，让同一通道的所有样本归一化。
2.  **归一化**
    
    x^n,c,h,w\=xn,c,h,w−μcσc2+ϵ\\hat{x}\_{n,c,h,w} = \\frac{x\_{n,c,h,w} - \\mu\_c}{\\sqrt{\\sigma\_c^2 + \\epsilon}}x^n,c,h,w​\=σc2​+ϵ​xn,c,h,w​−μc​​
3.  **加入可训练参数**
    
    yn,c,h,w\=γcx^n,c,h,w+βcy\_{n,c,h,w} = \\gamma\_c \\hat{x}\_{n,c,h,w} + \\beta\_cyn,c,h,w​\=γc​x^n,c,h,w​+βc​
    -   γc\\gamma\_cγc​ 和 βc\\beta\_cβc​ 是**可训练参数**，用于调整归一化后的数据。

**特点**：

-   适用于大 batch 训练（batch size 越小，均值和标准差计算越不稳定）。
-   用于 CNN 任务，如分类、检测等。

* * *

#### **3.2 Instance Normalization（IN，实例归一化）**

**归一化范围**：对**单个样本的每个通道**计算均值和标准差，而**不考虑 batch 之间的关系**。

##### **数学公式**

1.  **计算均值和标准差（在单个样本内的单个通道计算）**
    
    μn,c\=1H⋅W∑h\=1H∑w\=1Wxn,c,h,w\\mu\_{n,c} = \\frac{1}{H \\cdot W} \\sum\_{h=1}^{H} \\sum\_{w=1}^{W} x\_{n,c,h,w}μn,c​\=H⋅W1​h\=1∑H​w\=1∑W​xn,c,h,w​ σn,c2\=1H⋅W∑h\=1H∑w\=1W(xn,c,h,w−μn,c)2\\sigma\_{n,c}^2 = \\frac{1}{H \\cdot W} \\sum\_{h=1}^{H} \\sum\_{w=1}^{W} (x\_{n,c,h,w} - \\mu\_{n,c})^2σn,c2​\=H⋅W1​h\=1∑H​w\=1∑W​(xn,c,h,w​−μn,c​)2
2.  **归一化**
    
    x^n,c,h,w\=xn,c,h,w−μn,cσn,c2+ϵ\\hat{x}\_{n,c,h,w} = \\frac{x\_{n,c,h,w} - \\mu\_{n,c}}{\\sqrt{\\sigma\_{n,c}^2 + \\epsilon}}x^n,c,h,w​\=σn,c2​+ϵ​xn,c,h,w​−μn,c​​
3.  **加入可训练参数**
    
    yn,c,h,w\=γcx^n,c,h,w+βcy\_{n,c,h,w} = \\gamma\_c \\hat{x}\_{n,c,h,w} + \\beta\_cyn,c,h,w​\=γc​x^n,c,h,w​+βc​

**特点**：

-   适用于风格迁移、医学图像等任务。
-   不受 batch size 影响。



# 各种层

## 上采样 upsampling
指的是把低分辨率的数据转换为高分辨率的数据，在计算机视觉中主要用于恢复图像细节、提高分辨率。上采样通常在解码器（Decoder）或者生成模型（如 GAN）中使用，以便从小的特征图生成大尺寸的输出。
事实上**上采样**和**下采样**都可以归为图像的**重采样resample**.

常见的上采样方法:

- **插值interpolation**

    插值层一般没有可学习的参数. 是固定的生成.
  - **最近邻插值（Nearest Neighbor Interpolation）**：复制最近的像素值。
  - **双线性/双三次插值（Bilinear/Bicubic Interpolation）**：使用加权平均法插值。

- **转置卷积（Transpose Convolution，或反卷积）**：学习一个**可训练的卷积核**，用于恢复空间尺寸。

### 反卷积数学实现:
![标准卷积](image.png)
---

### **(2) 3D 逆卷积（Transpose Convolution）的数学计算**
在**转置卷积**中，我们希望执行卷积的**逆操作**，从 \( Y \) 反推 \( X \)。

![逆卷积](image-1.png)

![逆卷积图解](image-2.png)
其实就是有一个2\*2卷积核, 它对(比如说2\*2大小的)input每个像素进行逆卷积(也就是卷积核每个元素都和这个input元素相乘, 得到一个2\*2的逆卷积结果. 然后这些2\*2的逆卷积结果拼起来得到3\*3的输出.) 我们可以看到这个过程反过来就是卷积.


---

#
##
