#include "system.h"

void vUartRx_Process()
{
	if(rx_flag)
	{
		rx_flag=0;
		vP2p0_Ctrl(0x80,rx_buf[0]);
		vP2p0_Ctrl(0xa0,rx_buf[1]);
	}
}

void main()
{
	u8 urat_data=123;
	Uart_Init();
	SendString("ÄãºÃ\r\n");
	SendString("Hello World!\r\n");
	SendData(urat_data/100+'0');
	SendData(urat_data/10%10+'0');
	SendData(urat_data%10+'0');
	
	while(1)
	{
		vUartRx_Process(); 
	}
	
}

void t2int() interrupt 12
{
	vRxIdle_Process();
}
