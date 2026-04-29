#include "system.h"

void system_init()
{
	P2P0_ctrl(0xa0,0);
	P2P0_ctrl(0x80,0xff);
}

u16 temperature;
u8 cnt_temp;
void ds18b20_read()
{
	if(cnt_temp>=100)
	{
		cnt_temp=0;
		temperature=(u16)(rd_temperature()*100);
	}
}

u8 temp_setting=25;
u8 temp_comparing=25;
u8 cnt_dac;
u8 dac_val;
u16 dac_disp;
u8 dac_mode;
void dac_process()
{
	if(cnt_dac>=10)
	{
		cnt_dac=0;
		if(dac_mode==0)
		{
			if(temperature<(temp_comparing*100))
				dac_val=0;
			else
				dac_val=0xff;			
		}
		else
		{
			if(temperature<=2000)
				dac_val=51;
			else if(temperature>=4000)
				dac_val=204;
			else
				dac_val=(u8)(7.65*temperature/100-102);
		}
		write_dac(dac_val);
		dac_disp=dac_val*100/51;
	}
}

u8 seg_disp_mode;
u8 cnt_kbd;
u8 key_val;
void kbd_proc()
{
	if(cnt_kbd>=10)
	{
		cnt_kbd=0;
		key_val=kbd_read_state();
			if(key_val==4)
			{
				seg_disp_mode=(seg_disp_mode+1)%3;
				if(seg_disp_mode==2)
					temp_comparing=temp_setting;
			}
			if(key_val==8)
			{
				if(seg_disp_mode==1)
					if(temp_setting>0)
					temp_setting--;
			}
			if(key_val==9)
			{
				if(seg_disp_mode==1)
					if(temp_setting<100)
					temp_setting++;
			}
			if(key_val==5)
			{
				dac_mode=(dac_mode+1)%2;
			}
	}
}

u8 led_ctrl=0xff;
void led_proc()
{
	if(dac_mode==0)
		led_ctrl&=~0x01;
	else
		led_ctrl |=0x01;
	if(seg_disp_mode==0)
		led_ctrl &=~0x02;
	else
		led_ctrl|=0x02;
	if(seg_disp_mode==1)
		led_ctrl &=~0x04;
	else
		led_ctrl|=0x04;
	if(seg_disp_mode==2)
		led_ctrl &=~0x08;
	else
		led_ctrl |=0x08;
	P2P0_ctrl(0x80,led_ctrl);
}

void smg_process()
{
	if(seg_disp_mode==0)
	{
		smg_buf[0]=smg_code[10];
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=smg_code[temperature/1000];
		smg_buf[5]=smg_code[temperature/100%10]|0x80;
		smg_buf[6]=smg_code[temperature/10%10];
		smg_buf[7]=smg_code[temperature%10];		
	}
	if(seg_disp_mode==1)
	{
		smg_buf[0]=smg_code[11];
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=0x00;
		smg_buf[6]=smg_code[temp_setting/10];
		smg_buf[7]=smg_code[temp_setting%10];			
	}
	if(seg_disp_mode==2)
	{
		smg_buf[0]=smg_code[12];
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=smg_code[dac_disp/100]|0x80;	
		smg_buf[6]=smg_code[dac_disp/10%10];
		smg_buf[7]=smg_code[dac_disp%10];				
	}
}

void main()
{
	system_init();
	Timer2Init();
	while((u8)(rd_temperature())==85);
	
	while(1)
	{
		smg_process();
		ds18b20_read();
		kbd_proc();
		dac_process();
	}
}

void t2int() interrupt 12
{
	cnt_kbd++;
	smg_disp();
	cnt_temp++;
	cnt_dac++;
	led_proc();
}
