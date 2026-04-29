#include "system.h"
#include <intrins.h>
#include "Delay.h"
#include "Device.h"
u8 smg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00,0x3E,0x73};

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


void main(void)
{
	vSystem_Init();
	vTimer2_Init();
	while(1)
	{
		
	}
}

void vSMG_Display()
{
	static u8 i=0;
	vDevice_Ctrl(0xe0,~smg_code[i]);
	vDevice_Ctrl(0xc0,0x01<<i);
	i = (i + 1) % 8;
}

//中断服务程序
void vTimer2_ISR() interrupt 12     //中断入口
{
	vSMG_Display();
}
