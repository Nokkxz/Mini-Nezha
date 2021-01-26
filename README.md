# mini-nezha

#### 介绍
Mini-Nezha控制代码

#### 软件架构
rmd_x8pro和m2006两种电机各使用一个串口，通过uart转can模块与电调通信。
其中rmd电机为单线程阻塞式通信（电调接收到一帧can消息后返回一帧），m2006电机为多线程通信（电调以1kHz的频率发送信息）。
Motor_rmd类调用组合的Can实例进行包的转义与收发，Motor_m2006通过收、发两个线程完成通信。


#### 安装教程

1.  git clone “https://gitee.com/nokk/mini-nezha.git”
2.  mkdir build & cd build
3.  cmake ..
4.  sudo code --user-data-dir ="~/.vscode-root"（root权限打开vscode，并打开mini-nezha文件夹）
5.  F5编译并运行（第一次编译会失败，按两次F5即可）

#### 使用说明

1.  电机初始化例子见main函数注释部分
2.  常用函数：
    Motor_m2006通过get_torque/speed/angle获得反馈（角度为单圈角度），set_torque输出力矩；
    Motor_rmd通过get_torque/speed/angle获得反馈（角度为多圈角度），set_torque/speed/angle输出力矩
3.  Motor_rmd还实现了其他不常用的函数（如更改pid，设置编码器零点，获得单圈角度等），未进行进一步封装，详见光毓电机文档