#include "system.h"
#include <intrins.h>
#include "Delay.h"
#include "Device.h"


HexToBin led_ctrl,uln_ctrl;
u16 cnt_buzzer;

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
		vDelay_Ms(100);
	}
}


//中断服务程序
void vTimer2_ISR() interrupt 12     //中断入口
{
	cnt_buzzer++;
	if(cnt_buzzer>=200)
	{
		cnt_buzzer=0;
		uln_ctrl.b.b6 = ~uln_ctrl.b.b6;
		vDevice_Ctrl(0xa0,uln_ctrl.hex);	
	}
}
