Use of this resource is subject to the Academic Free License v3.0.

注释：
MPU6050(连接在PB10(SCL),PB11(SDA),PA15(AD0)上面,INT未用到). 
修改文档
2024Apr10:

增加文件：
\USER\USER_MPU6050.c
\USER\USER_MPU6050.h
include USER_MPU6050.h
\HARDWARE\MPU6050下的所有文件

main函数中增加MPU的初始值，报告内容以及显示内容

Apr23:
更新线性代数的运算过程，在USER_MPU6050.c下
Apr26:
Add the "Array.h" and "Array.c"

Apr30:
将两部分整合

在Array.c下增加了delay_n1000ms的函数和计算PWM占空比的函数
PWM输入为9.7时候，实测为10.66%，此时电机会停止

May3:

增加了HC08对于print的部分（但是目前有点问题）

PWM改为PB0

May7:
增加了Control_System.c与Control_System.h

May8:
按5吸水，按6排水