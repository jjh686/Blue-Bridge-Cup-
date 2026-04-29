#include "system.h"

u8 buzzer_ctrl;

void system_init()
{
	P2P0_ctrl(0xa0,0);
	P2P0_ctrl(0x80,0xff);
}

u8 seg_disp_mode;
u8 cnt_kbd;
u8 key_val;
u8 seg_find_mode;
void kbd_proc()
{
	if(cnt_kbd>=10)
	{
		cnt_kbd=0;
		key_val=kbd_read_state();
			if(key_val==4)
			{
				seg_disp_mode=(seg_disp_mode+1)%3;
			}
			if(key_val==5)
			{
				if(seg_disp_mode==2)
				{
					seg_find_mode=(seg_find_mode+1)%3;
				}
					
			}
			if(key_val==9)
			{
				buzzer_ctrl=0;
				P2P0_ctrl(0xa0,buzzer_ctrl<<6);			
			}
	}
}

u8 hour,second,minute;
void ds1302_process()
{
	second=bcdtodec(Read_Ds1302_Byte(0x81));
	minute=bcdtodec(Read_Ds1302_Byte(0x83));
	hour=bcdtodec(Read_Ds1302_Byte(0x85));
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

u8 hour1,second1,minute1;
void vReti()
{
	static u8 k;	
	if((dist_sonic<30)&&k==0)
	{
		hour1=hour;
		minute1=minute;
		second1=second;
		k=1;
	}
	if(dist_sonic>=30)
	{
		k=0;
	}	
}

void vBuzzer_Process()
{
	if(dist_sonic<30)
	{
		buzzer_ctrl=1;
		P2P0_ctrl(0xa0,buzzer_ctrl<<6);			
	}
	if(dist_sonic>=30)
	{
		buzzer_ctrl=0;
		P2P0_ctrl(0xa0,buzzer_ctrl<<6);			
	}
}

u8 led_ctrl=0xff;
void led_proc()
{
	if(seg_disp_mode==0)
		led_ctrl&=~0x01;
	else
		led_ctrl |=0x01;
	if(seg_disp_mode==1)
		led_ctrl &=~0x02;
	else
		led_ctrl|=0x02;
	if(seg_disp_mode==2)
		led_ctrl &=~0x04;
	else
		led_ctrl|=0x04;
	P2P0_ctrl(0x80,led_ctrl);
}

void smg_process()
{
	if(seg_disp_mode==0)
	{
		smg_buf[0]=0x38;
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=smg_code[dist_sonic/100%10];
		smg_buf[6]=smg_code[dist_sonic/10%10];
		smg_buf[7]=smg_code[dist_sonic%10];		
	}
	if(seg_disp_mode==1)
	{
		smg_buf[0]=smg_code[hour/10];
		smg_buf[1]=smg_code[hour%10];
		smg_buf[2]=0x40;
		smg_buf[3]=smg_code[minute/10];
		smg_buf[4]=smg_code[minute%10];
		smg_buf[5]=0x40;
		smg_buf[6]=smg_code[second/10];
		smg_buf[7]=smg_code[second%10];			
	}
	if(seg_disp_mode==2)
	{
		if(key_val==8)
		{
			smg_buf[0]=0x77;
			smg_buf[1]=smg_code[seg_find_mode+1];
			smg_buf[2]=0x40;
			smg_buf[3]=0x40;
			smg_buf[4]=0x40;
			smg_buf[5]=0x40;	
			smg_buf[6]=0x40;
			smg_buf[7]=0x40;				
		}
		else
		{
			smg_buf[0]=0x77;
			smg_buf[1]=smg_code[seg_find_mode+1];
			smg_buf[2]=smg_code[hour1/10];
			smg_buf[3]=smg_code[hour1%10];
			smg_buf[4]=smg_code[minute1/10];
			smg_buf[5]=smg_code[minute1%10];	
			smg_buf[6]=smg_code[second/10];
			smg_buf[7]=smg_code[second%10];					
		}
	}
}

void main()
{
	system_init();
	Timer2Init();
	clock_set(23,59,50);
	
	while(1)
	{
		smg_process();
		kbd_proc();
		vSonic_Process();
		ds1302_process();
		vBuzzer_Process();
	}
}

void t2int() interrupt 12
{
	cnt_sonic++;
	cnt_kbd++;
	smg_disp();
	vReti();
	led_proc();
}
