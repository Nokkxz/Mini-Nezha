#include "Can.hpp"

Can::Can()
{
    
}

Can::Can(int uart, uint8_t can_port, uint32_t can_id)
{
    this->uart = uart;
    this->can_port = can_port;
    this->can_id = can_id;
}

int Can::send(uint8_t can_data[8])
{
    int send_len;

    send_buf[0] = 0x55;                         // frame header
    send_buf[1] = 0x00;                         // cmd
    send_buf[2] = (CAN_SENDBACK)? 0x01:0x00;    // subcmd
    send_buf[3] = 0x13;                         // frame length
    for(int i=0; i<4; i++)                      // CAN id
    {
        send_buf[i+4] = (uint8_t)(can_id>>(i*8));
    }
    send_buf[8] = can_port;                     // CAN port
    send_buf[9] = 0x08;                         // data length
    for(int i=0; i<8; i++)                      // data
    {
        send_buf[i+10] = can_data[i];
    }
    send_buf[18] = 0xaa;                        // frame end

    send_len = write(uart, send_buf, sizeof(send_buf));

    if(send_len!=sizeof(send_buf))
    {
        printf("Uart write failed!");
        return 0;
    }

    return send_len;
}

int Can::recv(uint8_t recv_data[8])
{
    int recv_len;
    while(1)
    {
        recv_len = read(uart, recv_buf, sizeof(recv_buf));
        if(recv_len > 0)
        {
            if(CAN_PRINT)
            {
                for(int i = 0; i < recv_len; i++)
                    printf("%02x ", recv_buf[i]);
                printf("\n");
            }
            if(recv_parse(recv_buf, recv_data))
            {
                return recv_len;
            }
        }
    }
}

int Can::recv_parse(uint8_t* recv_buf, uint8_t* recv_data)
{
    if (0x55==recv_buf[0])
    {
        uint8_t recv_port = recv_buf[8];
        uint32_t recv_id = (uint32_t)recv_buf[4]+((uint32_t)recv_buf[5]<<8)
                        +((uint32_t)recv_buf[6]<<16)+((uint32_t)recv_buf[7]<<24);
        if(recv_port != can_port || recv_id != can_id)
        {
            return 0;   // CAN port or CAN id dosen't match
        }
        switch (recv_buf[1])
        {
        case 0x10:  // uart send back
            for(int i=0; i<8; i++)
            {
                recv_data[i] = recv_buf[i+10];
            }
            break;
        case 0x02:  // CAN receive
            for(int i=0; i<8; i++)
            {
                recv_data[i] = recv_buf[i+10];
            }
            break;
        default:
            break;
        }
    }
    return 1;
}
