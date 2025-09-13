






# 机电耦合系数是啥?

**机电耦合系数×电压=机械力**.

![alt text](image-10.png)

![alt text](image-11.png)









# 计算PDE


![alt text](image.png)


## 加权余量法 (Weighted Residual Method, WRM)

![alt text](image-1.png)

(这是N个积分式. 其中积分域Ω指的是原始PDE方程定义的空间区域, 比如一维问题中是梁长, $Ω\=\[0,L\]$)




* 这些模态函数基底是怎麽得到的?
  忘记了...解某个本征函数得到一个个本征解.
![alt text](image-5.png)




### Galerkin 法 / Rayleigh–Ritz

能和残差R(x,t)相乘为0的权函数w(x)有非常多种选择方式, 比如0. 我们要选出一组N个w_m(x).

Galerkin 法 指的是具体如何去选择这组权函数w_m(x): 直接令$w_m(x)=ϕ_m(x)$.


![alt text](image-2.png)

![alt text](image-6.png)

以Euler–Bernoulli 梁方程为例.

![alt text](image-3.png)

![alt text](image-4.png)

![alt text](image-7.png)

![alt text](image-8.png)

![alt text](image-9.png)





