






# esp32&arduino


开发方式:
* 1.官方IDE
* 2.vscode platformIO








## 官方IDE

打开官方IDE, 在tools->board寻找你的开发板型号(`esp32`). 如果没有, 选择boards manager添加.

![alt text](image.png)

或者直接在侧边栏的第二个图标进入boards manager.

![alt text](image-1.png)

搜索`esp32`会发现两个结果:
![alt text](image-2.png)


### 🟦 **1\. `Arduino ESP32 Boards by Arduino`**

-   📦 版本：2.0.18（由 Arduino 官方团队维护）
    
-   ✅ 支持板子：Arduino Nano ESP32 系列
    
-   🔧 优化对象：主要为 Arduino 自家推出的 **Nano ESP32** 板子（不是普通 ESP32 DevKit）
    

⚠️ 如果你用的是市面上的 **普通 ESP32 DevKit V1、ESP32-WROOM-32、ESP32-C3** 等板子，不推荐选这个包，兼容性差。

* * *

### 🟩 **2\. `esp32 by Espressif Systems`**

-   📦 版本：3.2.0（由 Espressif 官方提供）
    
-   ✅ 支持板子：几乎所有 ESP32 系列（DevKit V1、ESP32-C3/C6、S2/S3、PICO 等）
    
-   💡 最常用、最强大、社区最活跃的支持包
    
-   💯 推荐用于 **绝大多数 ESP32 开发板**


我操死活下载不了, 说网络错误. 手动粘贴了url下载然后放在本地缓存目录(C:\Users\<你>\AppData\Local\Arduino15\staging\packages\)的压缩包然后IDE就她妈的安装了半个小时还是这样,.![alt text](image-3.png)
死妈IDE, 卸载.




## vscode+platformIO插件

![alt text](image-4.png)









#








#













