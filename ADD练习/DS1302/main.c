#include "system.h"

void system_init()
{
	P2P0_ctrl(0xa0,0x00);
	P2P0_ctrl(0x80,0xff);
	P2P0_ctrl(0xc0,0x00);
}

u8 hour,second,minute;
void ds1302_process()
{
	second=bcdtodec(Read_Ds1302_Byte(0x81));
	minute=bcdtodec(Read_Ds1302_Byte(0x83));
	hour=bcdtodec(Read_Ds1302_Byte(0x85));
}

void smg_process()
{
	smg_buf[0]=smg_code[hour/10];
	smg_buf[1]=smg_code[hour%10]|0x80;
	smg_buf[2]=smg_code[minute/10];
	smg_buf[3]=smg_code[minute%10]|0x80;
	smg_buf[4]=smg_code[second/10];
	smg_buf[5]=smg_code[second%10];
	smg_buf[6]=0x00;
	smg_buf[7]=0x00;
}

void main()
{
	system_init();
	Timer2Init();
	clock_set(23,59,55);
	
	while(1)
	{
		smg_process();
		ds1302_process();
	}
}

void t2int() interrupt 12
{
	smg_display();
}
