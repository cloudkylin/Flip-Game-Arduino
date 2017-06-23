# Flip-Game-Arduino
在Arduino上通过LCD1602和8*8点阵制作开灯游戏。

游戏描述来源于POJ（http://poj.org/problem?id=1753）

开发板／开发环境：Arduino MEGA 2560/Arduino IDE 1.8.1 on macOS 10.12

需要的库：IRremote（https://github.com/z3t0/Arduino-IRremote）

配件：LCD1602，1088AS，红外及遥控器，按键。

如需要在Arduino UNO上运行，请考虑去掉lcdprint,start,delaya三个函数以削减内存消耗。
