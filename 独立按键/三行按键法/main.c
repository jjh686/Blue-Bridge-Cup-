#include "system.h"
#include <intrins.h>
#include "Delay.h"
#include "Device.h"
u8 code smg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x76,0x79,0x38};

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

#define KEY_NO	 0
#define KEY_DOWN 1
#define KEY_UP   2

u8 vBTN_Read(void)
{
	static u8 key_state = 0;
	u8 key_io = 0, key_val = 0;
	
	key_io = P3 & 0x0f;
	switch(key_state)
	{
		case KEY_NO:
			if(key_io!=0x0f) key_state = KEY_DOWN;
		break;
		
		case KEY_DOWN:
			if(key_io!=0x0f)
			{
				if(key_io==0x0e) key_val = 7;
				if(key_io==0x0d) key_val = 6;
				if(key_io==0x0b) key_val = 5;
				if(key_io==0x07) key_val = 4;
				key_state = KEY_UP;
			}
			else
				key_state = KEY_NO;
		break;
			
		case KEY_UP:
			if(key_io==0x0f) key_state = KEY_NO;
		break;
	}
	return key_val;
}
u8 cnt_key;
u8 s4_number;
void vBTN_Process(void)
{
	u8 key_val;
	if(cnt_key>=10)
	{
		cnt_key=0;
		key_val = vBTN_Read();
		if(key_val==4)
		{
			s4_number++;
		}
	}
}

u8 smg_buf[8];
void vSMG_Process()
{
	smg_buf[0]=smg_code[s4_number/100];
	smg_buf[1]=smg_code[s4_number/10%10];
	smg_buf[2]=smg_code[s4_number%10];
	smg_buf[3]=0x00;
	smg_buf[4]=0x00;
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
		vBTN_Process();
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
	cnt_key++;
	vSMG_Display();
}
