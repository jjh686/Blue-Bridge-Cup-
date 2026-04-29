#include "system.h"

void system_init()
{
	P2P0_ctrl(0xa0,0x00);
	P2P0_ctrl(0x80,0xff);
	P2P0_ctrl(0xc0,0x00);
}

u8 cnt_kbd;
u8 key_val;
u8 kbd_nmuber;
void kbd_process()
{
	if(cnt_kbd==10)
	{
		cnt_kbd=0;
		kbd_read_threeline();
		key_val=cont_kbd;
		if(trg_kbd==0x88)
			kbd_nmuber++;
		if(trg_kbd==0x11)
			kbd_nmuber--;		
	}

}

void smg_process()
{
	smg_buf[0]=smg_code[key_val/16];
	smg_buf[1]=smg_code[key_val%16];
	smg_buf[2]=0x00;
	smg_buf[3]=0x00;
	smg_buf[4]=0x00;
	smg_buf[5]=smg_code[kbd_nmuber/100];
	smg_buf[6]=smg_code[kbd_nmuber/10%10];
	smg_buf[7]=smg_code[kbd_nmuber%10];		
}

void main()
{
	system_init();
	Timer2Init();
	
	while(1)
	{
		smg_process();
		kbd_process();
	}
}

void t2int() interrupt 12
{
	cnt_kbd++;
	smg_display();
}
