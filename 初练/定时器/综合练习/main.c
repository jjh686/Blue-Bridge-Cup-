#include "system.h"
#include <intrins.h>
#include "Delay.h"
#include "Device.h"


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

u16 cnt_led_blink,cnt_led_running;
u8 led_blink_times;
u8 led_shift;
void vLED_Process()
{
	if(cnt_led_blink>=500)
	{
		cnt_led_blink=0;
		led_ctrl.b.b0 = ~led_ctrl.b.b0;
		vDevice_Ctrl(0x80,led_ctrl.hex);
		
		led_blink_times++;
	}
	
	if(cnt_led_running>=100)
	{
		cnt_led_running=0;
		led_ctrl.b.b3 = ~(0x01>>led_shift);
		led_ctrl.b.b4 = ~(0x02>>led_shift);
		led_ctrl.b.b5 = ~(0x04>>led_shift);
		led_ctrl.b.b6 = ~(0x08>>led_shift);
		led_ctrl.b.b7 = ~(0x10>>led_shift);
		vDevice_Ctrl(0x80,led_ctrl.hex);	
		
		led_shift++;
		if(led_shift==5) 
			led_shift=0;
	}
}

void vULN_Process()
{
	if(led_blink_times==10)
	{
		uln_ctrl.b.b4 = 1;
		vDevice_Ctrl(0xa0,uln_ctrl.hex);		
	}
	if(led_blink_times==20)
	{
		led_blink_times=0;
		uln_ctrl.b.b4 = 0;
		vDevice_Ctrl(0xa0,uln_ctrl.hex);			
	}
}	
void main(void)
{
	vSystem_Init();
	vTimer2_Init();
	while(1)
	{
		
	}
}

//中断服务程序
void vTimer2_ISR() interrupt 12     //中断入口
{
	cnt_led_blink++;
	cnt_led_running++;
	vLED_Process();
	vULN_Process();
}
