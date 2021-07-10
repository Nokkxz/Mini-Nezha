#include "Motor_ak80.hpp"

/// Value Limits ///
#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -45.0f
#define V_MAX 45.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -44.0f
#define T_MAX 44.0f

static std::mutex uart_mut;
static uint8_t s_buf[19];
static uint8_t r_buf[19000];

static int FIRSTRECV = 1;

struct motor_ak80_cmd{
    float p_des, v_des, kp, kd, t_ff;
};

void packCmd(motor_ak80_cmd cmd, uint8_t* can_data);
void motor_ak80_can_send(int uart, uint32_t can_id, uint8_t can_port, uint8_t can_data[8]);

Motor_ak80::Motor_ak80(){}

Motor_ak80::Motor_ak80(int uart, uint8_t can_port, uint32_t can_id)
{
    this->uart = uart;
    this->can_port = can_port;
    this->can_id = can_id;
}

Motor_ak80::~Motor_ak80(){}

double Motor_ak80::GetAngle()
{
    return p;
}

double Motor_ak80::GetSpeed()
{
    return v;
}

double Motor_ak80::GetTorque()
{
    return t;
}

int Motor_ak80::SetAngle(double angle)  //TODO
{
    float p_des = angle;
    float v_des = 0;
    float kp = 1;
    float kd = 0;
    float t_ff = 1;
    motor_ak80_cmd cmd = {p_des, v_des, kp, kd, t_ff};
    uint8_t can_data[8];
    packCmd(cmd, can_data);
    motor_ak80_can_send(uart, can_id, can_port, can_data);
    return 1;
}

int Motor_ak80::SetSpeed(double speed)  //TODO
{
    float p_des = 0;
    float v_des = speed;
    float kp = 1;
    float kd = 0;
    float t_ff = 1;
    motor_ak80_cmd cmd = {p_des, v_des, kp, kd, t_ff};
    uint8_t can_data[8];
    packCmd(cmd, can_data);
    motor_ak80_can_send(uart, can_id, can_port, can_data);
    return 1;
}

int Motor_ak80::SetTorque(double torque)  //TODO
{
    float p_des = 0;
    float v_des = 0;
    float kp = 1;
    float kd = 0;
    float t_ff = torque;
    motor_ak80_cmd cmd = {p_des, v_des, kp, kd, t_ff};
    uint8_t can_data[8];
    packCmd(cmd, can_data);
    motor_ak80_can_send(uart, can_id, can_port, can_data);
    return 1;
}

void Motor_ak80::SendZeroCmd()
{
    uint8_t can_data[8];
    can_data[0] = 0xFF;
	can_data[1] = 0xFF;
	can_data[2] = 0xFF;
	can_data[3] = 0xFF;
	can_data[4] = 0xFF;
	can_data[5] = 0xFF;
	can_data[6] = 0xFF;
	can_data[7] = 0xFE;
    motor_ak80_can_send(uart, can_id, can_port, can_data);
}

void Motor_ak80::EnterMotorMode()
{
    uint8_t can_data[8];
    can_data[0] = 0xFF;
	can_data[1] = 0xFF;
	can_data[2] = 0xFF;
	can_data[3] = 0xFF;
	can_data[4] = 0xFF;
	can_data[5] = 0xFF;
	can_data[6] = 0xFF;
	can_data[7] = 0xFC;
    motor_ak80_can_send(uart, can_id, can_port, can_data);
}

void Motor_ak80::ExitMotorMode()
{
    uint8_t can_data[8];
    can_data[0] = 0xFF;
	can_data[1] = 0xFF;
	can_data[2] = 0xFF;
	can_data[3] = 0xFF;
	can_data[4] = 0xFF;
	can_data[5] = 0xFF;
	can_data[6] = 0xFF;
	can_data[7] = 0xFD;
    motor_ak80_can_send(uart, can_id, can_port, can_data);
}

void motor_ak80_uart_send_thread(int uart)
{
    int s_len;
    uart_mut.lock();
    s_len = write(uart, s_buf, sizeof(s_buf));
    uart_mut.unlock();
}

void motor_ak80_can_send(int uart, uint32_t can_id, uint8_t can_port, uint8_t can_data[8])
{
    s_buf[0] = 0x55;                // frame header
    s_buf[1] = 0x00;                // cmd
    s_buf[2] = 0x00;                // subcmd
    s_buf[3] = 0x13;                // frame length
    for(int i=0; i<4; i++)          // CAN id
    {
        s_buf[i+4] = (uint8_t)(can_id>>(i*8));
    }
    s_buf[8] = can_port;            // CAN port
    s_buf[9] = 0x08;                // data length
    for(int i=0; i<8; i++)          // data
    {
        s_buf[i+10] = can_data[i];
    }
    s_buf[18] = 0xaa;               // frame end

    std::thread uart_send_t(motor_ak80_uart_send_thread, uart);
    uart_send_t.detach();
}

void packCmd(motor_ak80_cmd cmd, uint8_t* can_data)
{
    /// limit data to be within bounds ///
	float p_des = fminf2(fmaxf2(P_MIN, cmd.p_des), P_MAX);
	float v_des = fminf2(fmaxf2(V_MIN, cmd.v_des), V_MAX);
	float kp = fminf2(fmaxf2(KP_MIN, cmd.kp), KP_MAX);
	float kd = fminf2(fmaxf2(KD_MIN, cmd.kd), KD_MAX);
	float t_ff = fminf2(fmaxf2(T_MIN, cmd.t_ff), T_MAX);
	/// convert floats to unsigned ints ///
	uint16_t p_int = float_to_uint(p_des, P_MIN, P_MAX, 16);
	uint16_t v_int = float_to_uint(v_des, V_MIN, V_MAX, 12);
	uint16_t kp_int = float_to_uint(kp, KP_MIN, KP_MAX, 12);
	uint16_t kd_int = float_to_uint(kd, KD_MIN, KD_MAX, 12);
	uint16_t t_int = float_to_uint(t_ff, T_MIN, T_MAX, 12);
	/// pack ints into the joint_msg ///
	can_data[0] = p_int >> 8;
	can_data[1] = p_int & 0xFF;
	can_data[2] = v_int >> 4;
	can_data[3] = ((v_int & 0xF) << 4) | (kp_int >> 8);
	can_data[4] = kp_int & 0xFF;
	can_data[5] = kd_int >> 4;
	can_data[6] = ((kd_int & 0xF) << 4) | (t_int >> 8);
	can_data[7] = t_int & 0xff;
}



void motor_ak80_can_recv(Motor_ak80* m1, Motor_ak80* m2)
{
    uint32_t can_id;
    uint8_t can_port;
    if (0x55==r_buf[0])
    {
        switch (r_buf[1])
        {
        case 0x10:  // USB2CAN send back
            break;
        case 0x02:  // CAN receive
            if(FIRSTRECV)
            {
                printf("motor_ak80 ready\n\n");
                FIRSTRECV = 0;
            }
            can_port = r_buf[8];
            can_id = (uint32_t)r_buf[4]+((uint32_t)r_buf[5]<<8)
                        +((uint32_t)r_buf[6]<<16)+((uint32_t)r_buf[7]<<24);
            break;
        default:
            break;
        }
    }

    uint16_t id = int(r_buf[10]);
    uint16_t p_int = (r_buf[11] << 8) | r_buf[12];
    uint16_t v_int = (r_buf[13] << 4) | (r_buf[14] >> 4);
    uint16_t i_int = ((r_buf[14] & 0xF) << 8) | r_buf[15];

    float p = uint_to_float(p_int, P_MIN, P_MAX, 16);
    float v = uint_to_float(v_int, V_MIN, V_MAX, 12);
    float t = uint_to_float(i_int, -T_MAX, T_MAX, 12);

    if(can_port == m1->can_port && id == m1->can_id)
    {
        m1->p = p;
        m1->v = v;
        m1->t = t;
    }
    else if(can_port == m2->can_port && id == m2->can_id)
    {
        m2->p = p;
        m2->v = v;
        m2->t = t;
    }
}

void motor_ak80_uart_recv_thread(Motor_ak80* m1, Motor_ak80* m2)
{
    int uart = m1->uart;
    int r_len;

    while(1)
    {
        usleep(1);
        uart_mut.lock();
        r_len = read(uart, r_buf, sizeof(r_buf));
        uart_mut.unlock();
        if(r_len > 0)
        {
            motor_ak80_can_recv(m1, m2);
        }
    }
}

void Motor_ak80_start_update(Motor_ak80* m1, Motor_ak80* m2)
{
    std::thread motor_recv_t(motor_ak80_uart_recv_thread, m1, m2);
    motor_recv_t.detach();
}