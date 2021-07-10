#include <time.h>
#include <iostream>
#include <string>
#include "uart.hpp"
#include "Can.hpp"
#include "Motor.hpp"
#include "Motor_rmd.hpp"
#include "Motor_m2006.hpp"
#include "Motor_ak80.hpp"
#include "Pid.hpp"
#include "Robot.hpp"
#include "RestState.hpp"
#include "StandState.hpp"

using namespace std;

void test_rmd();
void test_m2006();
void test_imu();
void test_2motor();
void test_ak80();

int main(int argc, char const *argv[])
{
    test_ak80();
    // int uart1 = uart_init(115200, (char*)"/dev/ttyACM0");
    // int uart2 = uart_init(115200, (char*)"/dev/ttyACM1");
    // int uart3 = uart_init(115200, (char*)"/dev/ttyACM2");

    // Motor_rmd hip_L = Motor_rmd(Can(uart1, 0, 1));
    // Motor_rmd hip_R = Motor_rmd(Can(uart1, 0, 2));
    // Motor_rmd knee_L = Motor_rmd(Can(uart1, 0, 3));
    // Motor_rmd knee_R = Motor_rmd(Can(uart1, 0, 4));

    // Motor_m2006 wheel_L = Motor_m2006(uart2, 0, 1);
    // Motor_m2006 wheel_R = Motor_m2006(uart2, 1, 2);
    // m2006_start_update(&wheel_L, &wheel_R);

    // Imu imu = Imu(uart3);
    // imu_start_update(&imu);

    // Robot robot = Robot(imu, hip_L, hip_R, knee_L, knee_R, wheel_L, wheel_R);

    // RestState restState = RestState();
    // StandState standState = StandState();

    // robot.SetCurState(&restState);

    // string cmd;
    // cin>>cmd;
    // if(0==cmd.compare("s"))
    // {
    //     robot.ChangeState(&standState);
    //     while(1)
    //     {
    //         robot.Update();
    //     }
    // }
}

void test_ak80()
{
    int uart1 = uart_init(115200, (char*)"/dev/ttyACM0");
    Motor_ak80 m1 = Motor_ak80(uart1, 0, 1);

    string cmd;
    while(1)
    {
        cin>>cmd;
        if(0==cmd.compare("ga"))
        {
            printf("%f\n", m1.GetAngle());
            printf("\n");
        }
        if(0==cmd.compare("sa"))
        {
            double angle;
            cin>>angle;
            m1.SetAngle(angle);
            printf("\n");
        }
        if(0==cmd.compare("ss"))
        {
            double speed;
            cin>>speed;
            m1.SetSpeed(speed);
            printf("\n");
        }
        if(0==cmd.compare("st"))
        {
            double torque;
            cin>>torque;
            m1.SetAngle(torque);
            printf("\n");
        }
        if(0==cmd.compare("sz"))
        {
            m1.SendZeroCmd();
            printf("\n");
        }
    }
}

void test_2motor()
{
    // int uart2 = uart_init(115200, (char*)"/dev/ttyACM0");
    // Motor_m2006 motor_m2006 = Motor_m2006(uart2, 0, 1);
    // m2006_start_update(&motor_m2006, &motor_m2006);

    int uart1 = uart_init(115200, (char*)"/dev/ttyACM0");
    Motor motor_rmd1 = Motor_rmd(uart1, 0, 1);
    Motor motor_rmd2 = Motor_rmd(uart1, 0, 2);

    string cmd;
    while(1)
    {
        cin>>cmd;
        if(0==cmd.compare("ga"))
        {
            printf("%f\n", motor_rmd1.GetAngle());
            printf("%f\n", motor_rmd2.GetAngle());
            printf("\n");
        }
        if(0==cmd.compare("sa1"))
        {
            double angle;
            cin>>angle;
            motor_rmd1.SetAngle(angle,100);
            printf("\n");
        }
        if(0==cmd.compare("sa2"))
        {
            double angle;
            cin>>angle;
            motor_rmd2.SetAngle(angle,100);
            printf("\n");
        }
        if(0==cmd.compare("sa3"))
        {
            double angle1, angle2;
            cin>>angle1;
            cin>>angle2;
            motor_rmd1.SetAngle(angle1,100);
            motor_rmd2.SetAngle(angle2,100);
            printf("\n");
        }
        if(0==cmd.compare("r"))
        {
            motor_rmd1.SetTorque(0);
            motor_rmd2.SetTorque(0);
            printf("\n");
        }
        if(0==cmd.compare("st2"))
        {
            double torque;
            cin>>torque;
            motor_rmd2.SetTorque(torque);
            printf("\n");
        }
    }
}

void test_imu()
{
    int uart = uart_init(115200, (char*)"/dev/ttyACM0");
    Imu imu = Imu(uart);
    imu_start_update(&imu);
}

void test_rmd()
{
    int uart = uart_init(115200, (char*)"/dev/ttyACM0");

    Motor_rmd motor_rmd = Motor_rmd(uart, 0, 1);

    while(1)
    {
        string cmd;
        cin>>cmd;
        if(0==cmd.compare("l"))
        {
            printf("get_motor_state: gms\n");
            printf("get_angle_multiloop: gam\n");
            printf("get_pid: gpid\n");
            printf("set_pid: spid\n");
            printf("current_control: cc\n");
            printf("torque_control: tc\n");
            printf("speed_control: sc\n");
            printf("angle_control: ac\n");
            printf("angle_control(max_speed): acs\n");
            printf("angle_multiloop_control: amc\n");
            printf("angle_multiloop_control(max_speed): amcs\n");
            printf("angle_singleloop_control: asc\n");
            printf("angle_singleloop_control(max_speed): ascs\n");
            printf("\n");
        }
        else if(0==cmd.compare("gms"))
        {
            printf("get_motor_state\n");
            motor_rmd.get_motor_state();
            double angle = motor_rmd.GetAngle();
            double speed = motor_rmd.GetSpeed();
            printf("temperature: %d\n", motor_rmd.temperature);
            printf("encoder: %d\n", motor_rmd.encoder);
            printf("current: %d\n", motor_rmd.current);
            printf("speed: %f\n", speed);
            printf("angle: %f\n", angle);
            printf("\n");
        }
        else if(0==cmd.compare("gpid"))
        {
            motor_rmd.get_pid();
            printf("get_pid\n");
            printf("angle_kp: %d\n", motor_rmd.angle_kp);
            printf("angle_ki: %d\n", motor_rmd.angle_ki);
            printf("speed_kp: %d\n", motor_rmd.speed_kp);
            printf("speed_ki: %d\n", motor_rmd.speed_ki);
            printf("current_kp: %d\n", motor_rmd.current_kp);
            printf("current_ki: %d\n", motor_rmd.current_ki);
            printf("\n");
        }
        else if(0==cmd.compare("spid"))
        {
            int pid_set[6];
            std::string pid_str[6] = {"\nangle_kp_set: ","\nangle_ki_set: ",
                                    "\nspeed_kp_set: ","\nspeed_ki_set: ",
                                    "\ncurrent_kp_set: ","\ncurrent_ki_set: "};
            printf("set_pid\n");
            for(int i=0; i<6; i++)
            {
                printf("%s", pid_str[i].c_str());
                std::cin>>pid_set[i];
            }
            motor_rmd.set_pid(pid_set[0], pid_set[1], pid_set[2],
                            pid_set[3], pid_set[4], pid_set[5]);
            printf("\n");
        }
        else if(0==cmd.compare("cc"))
        {
            int current_set;
            printf("current_control\n");
            printf("current_set(-2000~2000)(15): ");
            std::cin>>current_set;
            motor_rmd.current_control(current_set);
            printf("\n");
        }
        else if(0==cmd.compare("tc"))
        {
            int torque_set;
            printf("torque_control\n");
            printf("torque_set(): ");
            std::cin>>torque_set;
            motor_rmd.SetTorque(torque_set);
            printf("\n");
        }
        else if(0==cmd.compare("sc"))
        {
            int speed_set;
            printf("speed_control\n");
            printf("speed_set(dps): ");
            std::cin>>speed_set;
            motor_rmd.SetSpeed(speed_set);
            printf("\n");
        }
        else if(0==cmd.compare("ac"))
        {
            int angle_set;
            printf("angle_control\n");
            printf("angle_multiloop(d): ");
            std::cin>>angle_set;
            motor_rmd.SetAngle(angle_set);
            printf("\n");
        }
        else if(0==cmd.compare("acs"))
        {
            int set[2];
            std::string str[2] = {"angle_multiloop(d): ",
                                    "max_speed(dps): "};
            printf("angle_multiloop_control(max_speed)\n");
            for(int i=0; i<2; i++)
            {
                printf("%s", str[i].c_str());
                std::cin>>set[i];
            }
            motor_rmd.SetAngle(set[0], set[1]);
            printf("\n");
        }
        else
        {
            printf("%s\n\n", cmd.c_str());
        }
    }
}

void test_m2006()
{
    int uart = uart_init(115200, (char*)"/dev/ttyACM0");
    Motor_m2006 motor_m2006 = Motor_m2006(uart, 0, 1);
    Motor_m2006 motor_m2006_ = Motor_m2006(uart, 1, 1);
    motor_m2006_start_update(&motor_m2006, &motor_m2006_);

    while(1)
    {
        string cmd;
        cin>>cmd;
        if(0==cmd.compare("l"))
        {
            printf("get_angle: ga\n");
            printf("\n");
        }
        else if(0==cmd.compare("ga"))
        {
            printf("get_angle\n");
            double angle = motor_m2006.GetAngle();
            printf("angle: %f\n", angle);
            printf("\n");
        }
        else if(0==cmd.compare("gs"))
        {
            printf("get_speed\n");
            double speed = motor_m2006.GetSpeed();
            printf("speed: %f\n", speed);
            printf("\n");
        }
        else if(0==cmd.compare("gt"))
        {
            printf("get_torque\n");
            double torque = motor_m2006.GetTorque();
            printf("torque: %f\n", torque);
            printf("\n");
        }
        else if(0==cmd.compare("st"))
        {
            int torque_set;
            printf("set_torque\n");
            printf("torque_set(): ");
            std::cin>>torque_set;
            motor_m2006.SetTorque(torque_set);
            printf("\n");
        }
    }
}
