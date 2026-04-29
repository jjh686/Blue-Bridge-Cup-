#include "system.h"

void system_init()
{
	P2P0_ctrl(0xa0,0x00);
	P2P0_ctrl(0x80,0xff);
	P2P0_ctrl(0xc0,0x00);
}

u8 cnt_temp;
u32 temp_float;
u8 temperature;
void ds18b20_read()
{
	if(cnt_temp==100)
	{
		cnt_temp=0;
//		temperature=(u8)rd_temperature();
		temp_float=(u32)(rd_temperature()*10000);
	}
}

void smg_process()
{
	smg_buf[0]=smg_code[temp_float/100000];
	smg_buf[1]=smg_code[temp_float/10000%10]|0x80;
	smg_buf[2]=smg_code[temp_float/1000%10];
	smg_buf[3]=smg_code[temp_float/100%10];
	smg_buf[4]=smg_code[temp_float/10%10];
	smg_buf[5]=smg_code[temp_float%10];
	smg_buf[6]=0x00;
	smg_buf[7]=0x00;		
}

void main()
{
	system_init();
	Timer2Init();
	
	while(1)
	{
		smg_process();
		ds18b20_read();
	}
}

void t2int() interrupt 12
{
	smg_display();
	cnt_temp++;
}
