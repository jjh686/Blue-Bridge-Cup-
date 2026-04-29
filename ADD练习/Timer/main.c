#include "system.h"

hextobin uln_ctrl,led_ctrl;

void rybr_ctrl()
{
	P2P0_ctrl(0xa0,0);
	led_ctrl.hex=0xff;
	P2P0_ctrl(0x80,led_ctrl.hex);
}

u16 cnt_led,cnt_led_running;
u8 cnt_led_bilnk,led_shift;
void led_Process()
{
	if(cnt_led==500)
	{
		cnt_led=0;
		led_ctrl.b.b0=~led_ctrl.b.b0;
		P2P0_ctrl(0x80,led_ctrl.hex);
		cnt_led_bilnk++;
	}
	
	if(cnt_led_running==100)
	{
		cnt_led_running=0;
		led_ctrl.b.b3=~(0x01>>led_shift);
		led_ctrl.b.b4=~(0x02>>led_shift);
		led_ctrl.b.b5=~(0x04>>led_shift);
		led_ctrl.b.b6=~(0x08>>led_shift);
		led_ctrl.b.b7=~(0x10>>led_shift);
		P2P0_ctrl(0x80,led_ctrl.hex);
		led_shift=(led_shift+1)%5;
	}
}

void uln_Process()
{
	if(cnt_led_bilnk==10)
	{
		uln_ctrl.b.b4=1;
		P2P0_ctrl(0xa0,uln_ctrl.hex);
	}
	if(cnt_led_bilnk==20)
	{
		cnt_led_bilnk=0;
		uln_ctrl.b.b4=0;
		P2P0_ctrl(0xa0,uln_ctrl.hex);
	}
}

void Timer2Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0x18;		//设置定时初值
	T2H = 0xFC;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2|=0x04;
	EA=1;
}

void main()
{
	rybr_ctrl();
	Timer2Init();
	
	while(1)
	{
		
	}
}

void t2int() interrupt 12           //中断入口
{
	cnt_led++;
	cnt_led_running++;
	led_Process();
	uln_Process();
}
