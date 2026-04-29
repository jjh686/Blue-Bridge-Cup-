#include "system.h"

HexToBin led_ctrl,uln_ctrl;

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
	led_ctrl.hex=0xff;
	vDevice_Ctrl(0x80,led_ctrl.hex);	
}

u8 vKBD_KeyIO_State()
{
	u8 key_io = 0;
	P3=0xf0;P4=0xff;
	if(P44==0) key_io=4;
	if(P42==0) key_io=8;
	if(P35==0) key_io=12;
	if(P34==0) key_io=16;
	
	P3=0x0f;P4=0x00;
	if(P33==0) key_io+=0;
	if(P32==0) key_io+=1;
	if(P31==0) key_io+=2;
	if(P30==0) key_io+=3;
	
	return key_io;
}

u8 vKBD_KeyIO_ThreeLine()
{
	u8 key_io = 0xff;
	P3=0xf0;P4=0xff;
	if(P44==0) key_io=0x70;
	if(P42==0) key_io=0xb0;
	if(P35==0) key_io=0xd0;
	if(P34==0) key_io=0xe0;
	
	P3=0x0f;P4=0x00;
	if(P33==0) key_io=key_io | 0x07;
	if(P32==0) key_io=key_io | 0x0b;
	if(P31==0) key_io=key_io | 0x0d;
	if(P30==0) key_io=key_io | 0x0e;
	
	return key_io;
}

u8 cnt_kbd;
u8 kbd_number;
u8 key_val;
void vKBD_Process(void)
{
	if(cnt_kbd>=10)
	{
		cnt_kbd=0;
		vKBD_Read_ThreeLine();
		key_val = Cont_KBD;
		if(Trg_KBD==0x88)
		{
			kbd_number++;
		}
		if(Trg_KBD==0x11)
		{
			kbd_number--;
		}
	}
}

void vSMG_Process()
{
	smg_buf[0]=smg_code[key_val/16];
	smg_buf[1]=smg_code[key_val%16];
	smg_buf[2]=0x00;
	smg_buf[3]=0x00;
	smg_buf[4]=0x00;
	smg_buf[5]=smg_code[kbd_number/100];
	smg_buf[6]=smg_code[kbd_number/10%10];
	smg_buf[7]=smg_code[kbd_number%10];
}

void main(void)
{
	vSystem_Init();
	vTimer2_Init();
	while(1)
	{
		vSMG_Process();
		vKBD_Process();
	}
}

//中断服务程序
void vTimer2_ISR() interrupt 12     //中断入口
{
	cnt_kbd++;
	vSMG_Display();
}
