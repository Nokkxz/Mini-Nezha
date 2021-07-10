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
void can_recv(Motor_ak80* m1, Motor_ak80* m2)
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

void uart_recv_thread(Motor_ak80* m1, Motor_ak80* m2)
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
            can_recv(m1, m2);
        }
    }
}

void ak80_start_update(Motor_ak80* m1, Motor_ak80* m2)
{
    std::thread motor_recv_t(uart_recv_thread, m1, m2);
    motor_recv_t.detach();
}