#include "system.h"

void vSystem_init()
{
	vP2P0_ctrl(0xa0,0);
	vP2P0_ctrl(0x80,0xff);
	vP2P0_ctrl(0xc0,0);
}

u8 cnt_kbd;
u8 kbd_val;
void vKBD_Process()
{
	if(cnt_kbd>=10)
	{
		cnt_kbd=0;
		kbd_val=ucKBD_Read_State();
	}
}

u16 cnt_ne555;
u16 freq_ne555;
void vNE555_Process()
{
	cnt_ne555++;
	if(cnt_ne555>=100)
	{
		cnt_ne555=0;
		freq_ne555=((TH0<<8)|TL0)*10;
		TL0=0;
		TH0=0; 
	}	
}

void Smg_Process()
{
	smg_buf[0]=smg_code[freq_ne555/10000];
	smg_buf[1]=smg_code[freq_ne555/1000%10];
	smg_buf[2]=smg_code[freq_ne555/100%10];
	smg_buf[3]=smg_code[freq_ne555/10%10];
	smg_buf[4]=smg_code[freq_ne555%10];
	smg_buf[5]=0x00;
	if(kbd_val!=0)
	{
		smg_buf[6]=smg_code[kbd_val/10];
		smg_buf[7]=smg_code[kbd_val%10];	
	}
}

void main()
{
	vSystem_init();
	Timer2Init();
	vCounter0_Init();
	
	while(1)
	{
		Smg_Process();
		vKBD_Process();
	}
	
}

void t2int() interrupt 12
{
	cnt_kbd++;
	vSmg_Display();
	vNE555_Process();
}
