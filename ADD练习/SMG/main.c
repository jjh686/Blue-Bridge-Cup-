#include "system.h"

u8 smg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x76,0x38};
//				O																	  14E	    16H  17L			
void system_init()
{
	P2P0_ctrl(0xa0,0x00);
	P2P0_ctrl(0x80,0xff);
	P2P0_ctrl(0xe0,0xff);
}

u8 smg_buf[8];
void smg_process()
{
	smg_buf[0]=smg_code[16];
	smg_buf[1]=smg_code[14];
	smg_buf[2]=smg_code[17];
	smg_buf[3]=smg_code[17];
	smg_buf[4]=smg_code[0];
	smg_buf[5]=0x00;
	smg_buf[6]=0x00;
	smg_buf[7]=0x00;
}

void smg_display()
{
	static u8 k;
	P2P0_ctrl(0xc0,0);
	P2P0_ctrl(0xe0,~smg_buf[k]);
	P2P0_ctrl(0xc0,0x01<<k);
	k=(k+1)%8;
}

void main()
{
	system_init();
	Timer2Init();
	
	while(1)
	{
		smg_process();
	}
}

void t2int() interrupt 12           //ÖÐ¶ÏÈë¿Ú
{
	smg_display();
}
