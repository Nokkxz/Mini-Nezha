#include"uart.hpp"

static std::mutex uart_mut;
extern int test_send_len, test_recv_len, test_recv_cnt;

void uart_recv_parse(uint8_t* r_buf, CAN_info info)
{
    if (0x55==r_buf[0])
    {
        test_recv_cnt++;
        switch (r_buf[1])
        {
        case 0x10:  // send back
            info.CAN_port = r_buf[8];
            info.CAN_id = (uint32_t)r_buf[4]+((uint32_t)r_buf[5]<<8)
                        +((uint32_t)r_buf[6]<<16)+((uint32_t)r_buf[7]<<24);
            for(int i=0; i<8; i++)
            {
                info.CAN_data[i] = (float)r_buf[i+10];
            }
            break;
        case 0x02:  // CAN receive
            info.CAN_port = r_buf[8];
            info.CAN_id = (uint32_t)r_buf[4]+((uint32_t)r_buf[5]<<8)
                        +((uint32_t)r_buf[6]<<16)+((uint32_t)r_buf[7]<<24);
            for(int i=0; i<8; i++)
            {
                info.CAN_data[i] = (float)r_buf[i+10];
            }
            break;
        default:
            break;
        }
    }
    
}

void uart_send_parse(uint8_t* s_buf, CAN_info info)
{
    s_buf[0] = 0x55;                // frame header
    s_buf[1] = 0x00;                // cmd
    s_buf[2] = 0x00;                // subcmd
    s_buf[3] = 0x13;                // frame length
    for(int i=0; i<4; i++)          // CAN id
    {
        s_buf[i+4] = (uint8_t)(info.CAN_id>>(i*8));
    }
    s_buf[8] = info.CAN_port - 1;   // CAN port
    s_buf[9] = 0x08;                // data length
    for(int i=0; i<8; i++)          // data
    {
        s_buf[i+10] = info.CAN_data[i];
    }
    s_buf[18] = 0xaa;               // frame end
}

void uart_recv_thread(int uart, CAN_info info)
{
    uint8_t r_buf[100];
    int r_len;
    while(1)
    {
        usleep(1);
        uart_mut.lock();
        r_len = read(uart, r_buf, sizeof(r_buf));
        uart_mut.unlock();
        if(r_len > 0)
        {
            test_recv_len += r_len;
            // for(int i = 0; i < r_len; i++)
            //     printf("%02x ", r_buf[i]);
            // printf("\n");
            uart_recv_parse(r_buf, info);
        }
    }
}

void uart_send_thread(int uart, uint8_t* s_buf, int length)
{
    int s_len;
    uart_mut.lock();
    s_len = write(uart, s_buf, length);
    uart_mut.unlock();
    test_send_len += s_len;
}

void uart_send(int uart, CAN_info info)
{
    static uint8_t s_buf[19];
    uart_send_parse(s_buf, info);
    std::thread uart_send_t(uart_send_thread, uart, s_buf, 19);
    uart_send_t.detach();
}

int uart_open(int fd,const char *pathname)
{
    fd = open(pathname, O_RDWR|O_NOCTTY); 
    if (-1 == fd)
    { 
        perror("Can't Open Serial Port"); 
		return(-1); 
	} 
    if(isatty(STDIN_FILENO)==0) 
		printf("standard input is not a terminal device\n"); 
    return fd; 
}

int uart_set(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio; 
    if  ( tcgetattr( fd,&oldtio)  !=  0)
    {  
        perror("SetupSerial 1");
	    printf("tcgetattr( fd,&oldtio) -> %d\n",tcgetattr( fd,&oldtio)); 
        return -1; 
    } 
    bzero( &newtio, sizeof( newtio ) ); 
    newtio.c_cflag  |=  CLOCAL | CREAD;  
    newtio.c_cflag &= ~CSIZE;  
    switch( nBits ) 
    { 
        case 7: 
            newtio.c_cflag |= CS7; 
            break; 
        case 8: 
            newtio.c_cflag |= CS8; 
            break; 
    } 
    switch( nEvent ) 
    { 
        case 'o':
        case 'O': 
            newtio.c_cflag |= PARENB; 
            newtio.c_cflag |= PARODD; 
            newtio.c_iflag |= (INPCK | ISTRIP); 
            break; 
        case 'e':
        case 'E': 
            newtio.c_iflag |= (INPCK | ISTRIP); 
            newtio.c_cflag |= PARENB; 
            newtio.c_cflag &= ~PARODD; 
            break;
        case 'n':
        case 'N': 
            newtio.c_cflag &= ~PARENB; 
            break;
        default:
            break;
    }
    switch( nSpeed ) 
    { 
        case 2400: 
            cfsetispeed(&newtio, B2400); 
            cfsetospeed(&newtio, B2400); 
            break; 
        case 4800: 
            cfsetispeed(&newtio, B4800); 
            cfsetospeed(&newtio, B4800); 
            break; 
        case 9600: 
            cfsetispeed(&newtio, B9600); 
            cfsetospeed(&newtio, B9600); 
            break; 
        case 115200: 
            cfsetispeed(&newtio, B115200); 
            cfsetospeed(&newtio, B115200); 
            break; 
        case 460800: 
            cfsetispeed(&newtio, B460800); 
            cfsetospeed(&newtio, B460800); 
            break; 
        default: 
            cfsetispeed(&newtio, B115200); 
            cfsetospeed(&newtio, B115200); 
        break; 
    } 
    if( nStop == 1 ) 
        newtio.c_cflag &=  ~CSTOPB; 
    else if ( nStop == 2 ) 
        newtio.c_cflag |=  CSTOPB; 
    newtio.c_cc[VTIME]  = 0; 
    newtio.c_cc[VMIN] = 0; 
    tcflush(fd,TCIFLUSH); 
    if((tcsetattr(fd,TCSANOW,&newtio))!=0) 
    { 
        perror("com set error"); 
        return -1; 
    } 
    return 0; 
}

int uart_close(int fd)
{
    assert(fd);
    close(fd);
    return 0;
}

int uart_init(int baud, char* uart_path)
{
    int fd = uart_open(fd,uart_path);
    if(fd == -1)
    {
        fprintf(stderr,"uart_open error\n");
        exit(EXIT_FAILURE);
    }
    if(uart_set(fd,BAUD,8,'N',1) == -1)
    {
        fprintf(stderr,"uart set failed!\n");
        exit(EXIT_FAILURE);
    }
    return fd;
}