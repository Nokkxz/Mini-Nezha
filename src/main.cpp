#include "uart.hpp"
#include "Can.hpp"
#include "Motor_rmd.hpp"
#include "Motor_m2006.hpp"
#include <time.h>
#include <iostream>
#include <string>

using namespace std;

void test_rmd();
void test_m2006();

int main(int argc, char const *argv[])
{
    // int uart1 = uart_init(115200, (char*)"/dev/ttyACM0");
    // int uart2 = uart_init(115200, (char*)"/dev/ttyACM1");

    // Motor_rmd motor_rmd_1 = Motor_rmd(Can(uart1, 0, 1));
    // Motor_rmd motor_rmd_2 = Motor_rmd(Can(uart1, 0, 2));
    // Motor_m2006 motor_m2006_1 = Motor_m2006(uart2, 0, 1);
    // Motor_m2006 motor_m2006_2 = Motor_m2006(uart2, 1, 2);
    // m2006_start_update(&motor_m2006_1, &motor_m2006_2);

}

void test_rmd()
{
    int uart = uart_init(115200, (char*)"/dev/ttyACM0");

    Motor_rmd motor_rmd = Motor_rmd(Can(uart, 0, 1));

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
            double angle = motor_rmd.get_angle();
            double speed = motor_rmd.get_speed();
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
            motor_rmd.set_torque(torque_set);
            printf("\n");
        }
        else if(0==cmd.compare("sc"))
        {
            int speed_set;
            printf("speed_control\n");
            printf("speed_set(dps): ");
            std::cin>>speed_set;
            motor_rmd.set_speed(speed_set);
            printf("\n");
        }
        else if(0==cmd.compare("ac"))
        {
            int angle_set;
            printf("angle_control\n");
            printf("angle_multiloop(d): ");
            std::cin>>angle_set;
            motor_rmd.set_angle(angle_set);
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
            motor_rmd.set_angle(set[0], set[1]);
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
    m2006_start_update(&motor_m2006, &motor_m2006_);

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
            double angle = motor_m2006.get_angle();
            printf("angle: %f\n", angle);
            printf("\n");
        }
        else if(0==cmd.compare("gs"))
        {
            printf("get_speed\n");
            double speed = motor_m2006.get_speed();
            printf("speed: %f\n", speed);
            printf("\n");
        }
        else if(0==cmd.compare("gt"))
        {
            printf("get_torque\n");
            double torque = motor_m2006.get_torque();
            printf("torque: %f\n", torque);
            printf("\n");
        }
        else if(0==cmd.compare("st"))
        {
            int torque_set;
            printf("set_torque\n");
            printf("torque_set(): ");
            std::cin>>torque_set;
            motor_m2006.set_torque(torque_set);
            printf("\n");
        }
    }
}
