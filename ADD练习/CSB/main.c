#include "system.h"

void system_init()
{
	P2P0_ctrl(0xa0,0);
	P2P0_ctrl(0x80,0xff);
	P2P0_ctrl(0xc0,0x00);
}

u16 cnt_sonic;
u16 dist_sonic;
void vSonic_Process()
{
  	if(cnt_sonic>=100)
	{
		cnt_sonic=0;
		dist_sonic=uiDistance_Get();
	}
}

void vSmg_Process()
{
	smg_buf[0]=smg_code[dist_sonic/10000];
	smg_buf[1]=smg_code[dist_sonic/1000%10];
	smg_buf[2]=smg_code[dist_sonic/100%10];
	smg_buf[3]=smg_code[dist_sonic/10%10];
	smg_buf[4]=smg_code[dist_sonic%10];	
	smg_buf[5]=0x00;
	smg_buf[6]=0x00;
	smg_buf[7]=0x00;
}

void main()
{
	system_init();
	Timer2Init();
	
	while(1)
	{
		vSmg_Process();
		vSonic_Process();
	}
	
}

void t2int() interrupt 12
{
	vSmg_Display();
	cnt_sonic++;
}
