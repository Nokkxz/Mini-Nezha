#include "uart.hpp"
#include "Can.hpp"
#include "Motor_rmd.hpp"
#include <time.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
    int uart = uart_init(115200, (char*)"/dev/ttyACM0");

    Motor_rmd motor_rmd = Motor_rmd(Can(uart, 0, 0x141));

    while(0==motor_rmd.get_error_state());
    while(0==motor_rmd.get_motor_state());
    while(0==motor_rmd.get_pid());
    while(0==motor_rmd.get_angle_singleloop());
    while(0==motor_rmd.get_angle_multiloop());
    printf("motor ready\n\n");

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
            printf("temperature: %d\n", motor_rmd.temperature);
            printf("current: %d\n", motor_rmd.current);
            printf("speed: %d\n", motor_rmd.speed);
            printf("encoder: %d\n", motor_rmd.encoder);
            printf("\n");
        }
        else if(0==cmd.compare("gam"))
        {
            printf("get_angle_multiloop\n");
            motor_rmd.get_angle_multiloop();
            printf("angle_multiloop: %ld\n", motor_rmd.angle_multiloop);
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
        else if(0==cmd.compare("sc"))
        {
            int speed_set;
            printf("speed_control\n");
            printf("speed_set(0.01dps): ");
            std::cin>>speed_set;
            motor_rmd.speed_control(speed_set);
            printf("\n");
        }
        else if(0==cmd.compare("ac"))
        {
            int angle_set;
            printf("angle_control\n");
            printf("angle_multiloop(d): ");
            std::cin>>angle_set;
            angle_set *= 600;   //600 for x8
            motor_rmd.angle_multiloop_control(angle_set);
            printf("\n");
        }
        else if(0==cmd.compare("acs"))
        {
            int set[2];
            std::string str[2] = {"max_speed(dps): ",
                                    "angle_multiloop(d): "};
            printf("angle_multiloop_control(max_speed)\n");
            for(int i=0; i<2; i++)
            {
                printf("%s", str[i].c_str());
                std::cin>>set[i];
            }
            set[1] *= 600;   //600 for x8
            motor_rmd.angle_multiloop_control(set[0], set[1]);
            printf("\n");
        }
        else if(0==cmd.compare("amc"))
        {
            int angle_set;
            printf("angle_multiloop_control\n");
            printf("angle_multiloop(0.01d): ");
            std::cin>>angle_set;
            motor_rmd.angle_multiloop_control(angle_set);
            printf("\n");
        }
        else if(0==cmd.compare("amcs"))
        {
            int set[2];
            std::string str[2] = {"max_speed(dps): ",
                                    "angle_multiloop(0.01d): "};
            printf("angle_multiloop_control(max_speed)\n");
            for(int i=0; i<2; i++)
            {
                printf("%s", str[i].c_str());
                std::cin>>set[i];
            }
            motor_rmd.angle_multiloop_control(set[0], set[1]);
            printf("\n");
        }
        else if(0==cmd.compare("asc"))
        {
            int set[2];
            std::string str[2] = {"direction(cw:0/1): ",
                                    "angle_singleloop(0.01d): "};
            printf("angle_singleloop_control\n");
            for(int i=0; i<2; i++)
            {
                printf("%s", str[i].c_str());
                std::cin>>set[i];
            }
            motor_rmd.angle_singleloop_control(set[0], set[1]);
            printf("\n");
        }
        else if(0==cmd.compare("ascs"))
        {
            int set[3];
            std::string str[3] = {"direction(cw:0/1): ",
                                    "max_speed(1dps): ",
                                    "angle_singleloop(0.01d): "};
            printf("angle_singleloop_control(max_speed)\n");
            for(int i=0; i<3; i++)
            {
                printf("%s", str[i].c_str());
                std::cin>>set[i];
            }
            motor_rmd.angle_singleloop_control(set[0], set[1], set[2]);
            printf("\n");
        }
        else
        {
            printf("%s\n\n", cmd.c_str());
        }
    }

    // clock_t start_time, end_time;
    // start_time = clock();
    // int cnt = 0;

    // while(1)
    // {
    //     end_time = clock();
    //     if((end_time-start_time)>=CLOCKS_PER_SEC)
    //     {
    //         printf("%d\n",cnt);
    //         cnt = 0;
    //         start_time = end_time;
    //     }

    //     cnt += motor_rmd.get_motor_state();
        
    // }
}
