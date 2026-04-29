#include "system.h"
#include <intrins.h>
#include "Delay.h"
#include "Device.h"
u8 smg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x76,0x79,0x38};

HexToBin led_ctrl,uln_ctrl;

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
	led_ctrl.hex=0xff;
	vDevice_Ctrl(0x80,led_ctrl.hex);	
}

void vTimer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0x18;			//设置定时初值
	T2H = 0xFC;			//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;                    //开定时器2中断
	EA = 1;
}

u8 smg_buf[8];
void vSMG_Process()
{
	smg_buf[0]=smg_code[10];
	smg_buf[1]=smg_code[11];
	smg_buf[2]=smg_code[12];
	smg_buf[3]=smg_code[12];
	smg_buf[4]=smg_code[0];
	smg_buf[5]=0x00;
	smg_buf[6]=0x00;
	smg_buf[7]=0x00;
}

void main(void)
{
	vSystem_Init();
	vTimer2_Init();
	while(1)
	{
		vSMG_Process();
	}
}

void vSMG_Display()
{
	static u8 i=0;
	vDevice_Ctrl(0xc0,0);
	vDevice_Ctrl(0xe0,~smg_buf[i]);
	vDevice_Ctrl(0xc0,0x01<<i);
	i = (i + 1) % 8;
}

//中断服务程序
void vTimer2_ISR() interrupt 12     //中断入口
{
	vSMG_Display();
}
