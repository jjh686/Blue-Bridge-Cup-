#include "system.h"

void system_init()
{
	P2P0_ctrl(0xa0,0x00);
	P2P0_ctrl(0x80,0xff);
	P2P0_ctrl(0xc0,0x00);
}

//状态机按键处理函数
u8 cnt_key;
u8 s_number;
void btn_process()
{
	u8 key_val;
	if(cnt_key==10)
	{
		cnt_key=0;
		key_val = btn_read();
		if(key_val==4)
		{
			s_number++;
		}
	}
}

//三行按键法处理函数
void btn_process_threeline()
{
	if(cnt_key==10)
	{
		cnt_key=0;
		btn_read_threeline();
		if(trg & 0x08)			//S4
		{
			s_number++;
		}
		if(trg & 0x04)			//S5
		{
			s_number--;
		}
		if(trg & 0x02)			//S6
		{
			s_number+=10;
		}
		if(trg & 0x01)			//S7
		{
			s_number-=10;
		}
	}
}

//数码管操作函数
void smg_process()
{
	smg_buf[0]=smg_code[s_number/100];
	smg_buf[1]=smg_code[s_number/10%10];
	smg_buf[2]=smg_code[s_number%10];
	smg_buf[3]=0x00;
	smg_buf[4]=0x00;
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
		smg_process();
		btn_process_threeline();
	}
}

void t2int() interrupt 12
{
	cnt_key++;
	smg_display();
}
