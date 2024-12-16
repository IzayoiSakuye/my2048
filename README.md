# my2048
 For the final homework
 [思路来源](https://github.com/mevdschee/2048.c)
 [视频参考](https://www.bilibili.com/video/BV1GY4y1p78Z)

 ## 基本要素

1. 窗口：
   - 尺寸：$4\times 4$正方形格子，每格$100\times100$像素，**使用宏定义预定义尺寸大小**
   - 创建：使用EasyX图形库创建窗口，引入头文件`#include <graphics.h>`并进行初始化窗口
   - 颜色：使用米色(RGB:255,224,172)作为背景色
2. 格子：
   - 颜色：使用枚举类型枚举出所有数字背景颜色，使用switch case判断当前格子要填充哪个颜色
   - 存储数字：使用整型二维数组存储每格的数字
   - 坐标：在初始化窗口的同时，使用二重循环将每个格子的坐标计算出来

