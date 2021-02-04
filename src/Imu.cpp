#include "Imu.hpp"

static std::mutex uart_mut;
static char r_buf[1024];

Imu::Imu()
{
}

Imu::~Imu()
{
}

Imu::Imu(int uart):uart(uart)
{
}

void imu_parse_data(char chr, Imu* imu)
{
    static float a[3],w[3],Angle[3],h[3];
    static char chrBuf[100];
	static unsigned char chrCnt=0;
	signed short sData[4];
	unsigned char i;
		
	time_t now;
	chrBuf[chrCnt++]=chr;
	if (chrCnt<11) return;
		
	if ((chrBuf[0]!=0x55)||((chrBuf[1]&0x50)!=0x50)) {printf("Error:%x %x\r\n",chrBuf[0],chrBuf[1]);memcpy(&chrBuf[0],&chrBuf[1],10);chrCnt--;return;}
		
	memcpy(&sData[0],&chrBuf[2],8);
	switch(chrBuf[1])
	{
		case 0x51:
			for (i=0;i<3;i++) a[i] = (float)sData[i]/32768.0*16.0;
			time(&now);
			printf("\r\nT:%s a:%6.3f %6.3f %6.3f ",asctime(localtime(&now)),a[0],a[1],a[2]);
			break;
		case 0x52:
			for (i=0;i<3;i++) w[i] = (float)sData[i]/32768.0*2000.0;
			printf("w:%7.3f %7.3f %7.3f ",w[0],w[1],w[2]);
			break;
		case 0x53:
			for (i=0;i<3;i++) Angle[i] = (float)sData[i]/32768.0*180.0;
			printf("A:%7.3f %7.3f %7.3f ",Angle[0],Angle[1],Angle[2]);
			break;
		case 0x54:
			for (i=0;i<3;i++) h[i] = (float)sData[i];
			printf("h:%4.0f %4.0f %4.0f ",h[0],h[1],h[2]);
			break;
	}
	chrCnt=0;

	imu->angle_X = Angle[0];
	imu->angle_Y = Angle[1];
	imu->angle_Z = Angle[2];
	imu->angularSpeed_X = w[0];
	imu->angularSpeed_Y = w[1];
	imu->angularSpeed_Z = w[2];
	imu->acceleration_X = a[0];
	imu->acceleration_Y = a[1];
	imu->acceleration_Z = a[2];
}

void uart_recv_thread(Imu* imu)
{
    int uart = imu->uart;
    int r_len;

    while(1)
    {
        usleep(1);
        uart_mut.lock();
        r_len = read(uart, r_buf, 44);
        uart_mut.unlock();
        if(r_len > 0)
        {
            for(int i=0; i<r_len; i++)
            {
                imu_parse_data(r_buf[i], imu);
            }
        }
    }
}

void imu_start_update(Imu* imu)
{
    std::thread motor_recv_t(uart_recv_thread, imu);
    motor_recv_t.detach();
}