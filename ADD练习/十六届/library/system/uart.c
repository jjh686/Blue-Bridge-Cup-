#include "uart.h"

void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x8F;		//设定定时初值
	TH1 = 0xFD;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	ES = 1;
	EA = 1;
}


bit busy;
u8 rx_buf[RXBUF];
u8 cnt_rx=0;
bit rx_flag;
u8 rx_time=0;
/*----------------------------
UART 中断服务程序
-----------------------------*/
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //清除RI位
		rx_time=0;
		rx_buf[cnt_rx++]=SBUF;
		if(rx_buf[cnt_rx-1]=='\n')
		{
			rx_flag=1;
			cnt_rx=0;
		}
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy = 0;               //清忙标志
    }
}

void vRXkx_time()
{
	rx_time++;
	if(rx_time>=50)
	{
		rx_time=0;
		cnt_rx=0;
		memset(rx_buf,'\0',sizeof(rx_buf));
	}
}

/*---------------------------- 
发送串口数据
----------------------------*/
void SendData(u8 dat)
{
    while (busy);               //等待前面的数据发送完成
    busy = 1;
    SBUF = dat;                 //写数据到UART数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}

