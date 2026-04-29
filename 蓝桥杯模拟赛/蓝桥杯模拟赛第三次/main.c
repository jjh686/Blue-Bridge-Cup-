#include "system.h"

u8 set_disp_mode=0;
u8 set_time=5;
u8 match_time=0;
u16 cnt_match_time=0;
u8 k=0,j=0;

void vSystem_init()
{
	vP2P0_ctrl(0xa0, 0);
	vP2P0_ctrl(0x80,0xff);
}

u8 cnt_btn;
u8 key_val;
void vBTN_Process()
{
	if(cnt_btn>=10)
	{
		cnt_btn=0;
		key_val=ucBTN_Read();
		if(set_disp_mode==1)
		{
			if(key_val==6)
				set_time--;	
			if(key_val==7)
				set_time++;	
		}
	}	
}

u8 cnt_adc;
u8 ch1;
void vADC_Process()
{
	if(cnt_adc>=10)
	{
		cnt_adc=0;
		EA=0;
		ch1=ucADC_Read(0x41);
		EA=1;
	}
}


void vLED_Process()
{
	u8 led_ctrl=0xff;
	static u8 led_old = 0xFF;
	if(set_disp_mode==0)
		led_ctrl&=~0x01;
	else
		led_ctrl |=0x01;
	if(set_disp_mode==1)
		led_ctrl &=~0x02;
	else
		led_ctrl|=0x02;
	if(set_disp_mode==2)
		led_ctrl &=~0x04;
	else
		led_ctrl|=0x04;
	if(set_disp_mode==3)
		led_ctrl &=~0x08;
	else
		led_ctrl|=0x08;
    if(led_ctrl != led_old)
    {
        vP2P0_ctrl(0x80,led_ctrl);
        led_old = led_ctrl; // ¸üÐÂ¾É×´Ì¬
    }
}

void vSmg_Process()
{
	if(set_disp_mode==0)
	{
		smg_buf[0]=0x76;
		smg_buf[1]=0x79;
		smg_buf[2]=0x38;
		smg_buf[3]=0x38;
		smg_buf[4]=smg_code[0];
		smg_buf[5]=0x00;
		smg_buf[6]=0x00;
		smg_buf[7]=0x00;
		if(ch1<110)
		{
			set_disp_mode=1;
		}
			P3&=0x0f;
			if(P3==0x00)
			{
				set_disp_mode=2;
			}
		match_time=set_time;
		if(set_time>99)
			set_time=0;			
	}

	if(set_disp_mode==1)
	{
		smg_buf[0]=0x3e;
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=0x00;
		if(set_time>=10)
			smg_buf[6]=smg_code[set_time/10];
		else 
			smg_buf[6]=smg_code[0];
		smg_buf[7]=smg_code[set_time%10];
		if(ch1>=110)
		{
			set_disp_mode=0;
		}
	}
	if(set_disp_mode==2)
	{
		smg_buf[0]=0x77;
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=0x00;
		if(match_time>=10)
			smg_buf[6]=smg_code[match_time/10];
		else 
			smg_buf[6]=smg_code[0];
		smg_buf[7]=smg_code[match_time%10];
		if(cnt_match_time>=1000)
		{
			cnt_match_time=0;
			match_time--;
		}
		if(match_time>99)
			match_time=0;
		if(match_time==0)
			set_disp_mode=3;
		P3&=0x0f;
		if(P3!=0x00)
			set_disp_mode=3;
	}
	if(set_disp_mode==3)
	{
		smg_buf[0]=0x79;
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=0x00;
		if(k==0)
		{
			P3=0x0f;
			if(P3==0x01)
			{
				smg_buf[6]=0x00;
				smg_buf[7]=smg_code[7];
			}
			else if(P3==0x02)
			{
				smg_buf[6]=0x00;
				smg_buf[7]=smg_code[6];
			}
			else if(P3==0x04)
			{
				smg_buf[6]=0x00;
				smg_buf[7]=smg_code[5];
			}
			else if(P3==0x08)
			{
				smg_buf[6]=0x00;
				smg_buf[7]=smg_code[4];
			}
			else
			{
				smg_buf[6]=0x79;
				smg_buf[7]=0x79;
			}
			k=1;
		}
		P3&=0x0f;
		if(P3==0x0f)
		{
			j=1;
		}
		if(j==1)
		{
			P3&=0x0f;
			if(P3!=0x0f)
			{
				j=0;
				k=0;
				set_disp_mode=0;
			}
		}
	}
}

void main()
{
	vSystem_init();
	Timer2Init();
	
	while(1)
	{
		vBTN_Process();
		vSmg_Process();
		vADC_Process();
		vLED_Process();
	}
}

void t2_init() interrupt 12
{
	cnt_btn++;
	vSmg_Display();
	cnt_adc++;
	cnt_match_time++;
}
