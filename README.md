# mini-nezha

## 介绍
Mini-Nezha控制代码

## 硬件架构
### 电机：
rmd_x8pro和m2006两种电机各使用一个USB口，通过USB转can模块与电调通信。
### IMU：
1个IMU使用一个USB口与上位机通信。

## 软件架构
### Motor：
#### Motor_rmd：
光毓x8pro电机为单线程阻塞式通信（电调接收到一帧can消息后返回一帧）：  
Motor_rmd类调用组合的Can实例进行包的转义与收发。  
Motor_rmd类继承模板类Motor，实现函数GetTorque/Speed/Angle, SetTorque/Speed/Angle。
#### Motor_m2006：
大疆m2006电机为多线程通信（电调以1kHz的频率发送信息）：  
Motor_m2006通过收、发两个线程完成通信。  
Motor_m2006类继承模板类Motor，实现函数GetTorque/Speed/Angle, SetTorque。
### Robot：
Robot类中包含hip/knee/wheel_L/R共6个电机，1个IMU。  
Robot类同时作为StateMachine，包含当前状态CurState，实现了Update、ChangeState函数。
### State：
State为模板类，每个状态都继承于State，需要实现Enter、Exit函数（进出状态时执行的函数，Robot::ChangeState中调用）和Execute函数（状态中执行的函数，Robot::Update中执行）。

## 安装教程
1.  git clone “https://gitee.com/nokk/mini-nezha.git”  
2.  mkdir build & cd build  
3.  cmake ..  
4.  sudo code --user-data-dir ="~/.vscode-root"（root权限打开vscode，并打开mini-nezha文件夹）
5.  F5编译并运行

## 使用说明
1.  Motor_rmd还实现了其他不常用的函数（如更改pid，设置编码器零点，获得单圈角度等），未进行进一步封装，详见光毓电机文档。  
