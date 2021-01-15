#include"uart.hpp"

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
    if(uart_set(fd,baud,8,'N',1) == -1)
    {
        fprintf(stderr,"uart set failed!\n");
        exit(EXIT_FAILURE);
    }
    return fd;
}