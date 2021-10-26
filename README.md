# Mini-Nezha

## Introduction
Source code for Mini-Nezha

## Hardware
### Motor：
AK-80 x2 for wheels driving  
RMD-X8Pro x4 for 2 knees and 2 hips driving  
Communicate with PC by high-speed CAN bus communication through a USB2CAN module  

### IMU：
mpu6050 module x1, connect to PC with USB  

## Software
### Motor
Template class, with virtue function GetTorque/Speed/Angle, SetTorque/Speed/Angle
#### Motor_rmd:Motor
Single thread blocking communication  
Call the composited instance of the Can class to process the Can package
#### Motor_ak80:Motor
Receive and send Can package with multithread
### Robot
Composite 6 joint motors and an IMU  
As the finite state machine, contain CurState, realize Update and ChangeState function
### State
Temple class, with virtue function Enter, Exit and Execute
#### RestState:State
In RestState, set all torque to 0
#### StandState:State
In StandState, hip and knee joints are fixed by position control, robot keeps standing by PID control of the wheels

## Installation
1.  git clone “https://github.com/Nokkxz/Mini-Nezha.git”
2.  mkdir build & cd build
3.  cmake ..
4.  sudo code --user-data-dir ="~/.vscode-root" (open VSCode with root access)
5.  F5 compile and run

## Others
Motor_rmd Class implements other not commonly used functions (Change PID params, set the zero point...), check the manual of RMD motor for details
