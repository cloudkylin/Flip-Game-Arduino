# Flip-Game-Arduino
## 项目简介
在Arduino上通过LCD1602和8*8点阵制作开灯游戏。<br>
游戏描述来源于[POJ-1753](http://poj.org/problem?id=1753)
## 制作要求
开发板：Arduino MEGA 2560<br>
需要的库：[IRremote](https://github.com/z3t0/Arduino-IRremote)<br>
配件：LCD1602，1088AS，红外及遥控器，按键。<br>
## 平台需求
该代码无法在Arduino UNO等低内存的开发板上运行，因为本程序使用大约需要4kb内存，在UNO上使用会发生内存溢出。</br>
`如需要在Arduino UNO上运行，请考虑去掉lcdprint,start,delaya三个函数及部分变量以削减内存消耗。`
