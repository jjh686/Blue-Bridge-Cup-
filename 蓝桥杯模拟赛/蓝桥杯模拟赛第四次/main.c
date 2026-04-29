#include "system.h"

u8 second,minute,hour,second1,minute1,hour1;
u8 disp_mode,b,z;
u8 mima4[1]=9,mima3[1]=5,mima2[1]=2,mima1[1]=7;
u8 key_mima=10;
u8 mima_disp1[1]=10,mima_disp2[1]=10,mima_disp3[1]=10,mima_disp4[1]=10;

void vSystem_Init()
{
	vP2P0_Ctrl(0xe0,0xff);
	vP2P0_Ctrl(0xa0,0x10);
	vP2P0_Ctrl(0x80,0xff);
}

u8 cnt_kbd;
u16 cnt_relay,cnt_buzzer;
u8 key_val;
void vKBD_Proc()
{
	if(cnt_kbd>=10&&b==0&&z==0)
	{
		cnt_kbd=0;
		key_mima=10;
		key_val=ucKBD_Read_State();
		if(key_val==4)
		{
			disp_mode=(disp_mode+1)%3;
			if(disp_mode==1)
				mima_disp1[0]=10,mima_disp2[0]=10,mima_disp3[0]=10,mima_disp4[0]=10;
		}
		if(key_val==12) key_mima=0;if(key_val==9) key_mima=1;if(key_val==13) key_mima=2;if(key_val==17) key_mima=3;
		if(key_val==10) key_mima=4;if(key_val==14) key_mima=5;if(key_val==18) key_mima=6;if(key_val==11) key_mima=7;
		if(key_val==15) key_mima=8;if(key_val==19) key_mima=9;
			if(disp_mode==1)
			{
				if(key_mima<10&&(mima_disp4[0]==10))
				{
					mima_disp4[0]=mima_disp3[0];mima_disp3[0]=mima_disp2[0];mima_disp2[0]=mima_disp1[0];mima_disp1[0]=key_mima;
				}
				if(key_val==5)
				{
					if(mima_disp4[0]==mima4[0]&&mima_disp3[0]==mima3[0]&&mima_disp2[0]==mima2[0]&&mima_disp1[0]==mima1[0])
					{
						second1=second;minute1=minute;hour1=hour;
						vP2P0_Ctrl(0xa0,0x00);
						b=1;
					}
					else
					{
						vP2P0_Ctrl(0xa0,0x50);
						z=1;
					}
				}
				if(key_val==6)
				{
					mima_disp1[0]=10;mima_disp2[0]=10;mima_disp3[0]=10;mima_disp4[0]=10;
				}
			}
	}
}

void vDs1302_Proc()
{
	second=bcdtodec(Read_Ds1302_Byte(0x81));
	minute=bcdtodec(Read_Ds1302_Byte(0x83));
	hour=bcdtodec(Read_Ds1302_Byte(0x85));
}

void vcnt_Proc()
{
	if(cnt_relay>=3000)
	{
		cnt_relay=0;
		vP2P0_Ctrl(0xa0,0x10);
		b=0;
		mima_disp1[0]=10;mima_disp2[0]=10;mima_disp3[0]=10;mima_disp4[0]=10;
		disp_mode=0;
	}
	if(cnt_buzzer>=3000)
	{
		cnt_buzzer=0;
		vP2P0_Ctrl(0xa0,0x10);
		z=0;
		mima_disp1[0]=10;mima_disp2[0]=10;mima_disp3[0]=10;mima_disp4[0]=10;
		disp_mode=0;
	}
}

u8 led_ctrl=0xff;
void vLED_Proc()
{
	if(disp_mode==0)
		led_ctrl&=~0x01;
	else
		led_ctrl|=0x01;
	if(disp_mode==1)
		led_ctrl&=~0x02;
	else
		led_ctrl|=0x02;
	if(disp_mode==2)
		led_ctrl&=~0x04;
	else
		led_ctrl|=0x04;	
	if(b==1)
		led_ctrl&=~0x40;
	else
		led_ctrl|=0x40;
	if(z==1)
		led_ctrl&=~0x80;
	else
		led_ctrl|=0x80;
	vP2P0_Ctrl(0x80,led_ctrl);
}

void vSMG_Proc()
{
	if(disp_mode==0)
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
	if(disp_mode==1)
	{
		smg_buf[0]=0x73;
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;	
		if(mima_disp1[0]==10&&mima_disp2[0]==10&&mima_disp3[0]==10&&mima_disp4[0]==10)
		{
			smg_buf[4]=0x40;
			smg_buf[5]=0x40;
			smg_buf[6]=0x40;
			smg_buf[7]=0x40;		
		}
		else
		{
			if(mima_disp4[0]!=10)
				smg_buf[4]=smg_code[mima_disp4[0]];
			else
				smg_buf[4]=0x40;
			if(mima_disp3[0]!=10)
				smg_buf[5]=smg_code[mima_disp3[0]];
			else
				smg_buf[5]=0x40;
			if(mima_disp2[0]!=10)
				smg_buf[6]=smg_code[mima_disp2[0]];
			else
				smg_buf[6]=0x40;
			smg_buf[7]=smg_code[mima_disp1[0]];		
		}
	}
	if(disp_mode==2)
	{
		smg_buf[0]=0x77;
		smg_buf[1]=0x00;
		smg_buf[2]=smg_code[hour1/10];
		smg_buf[3]=smg_code[hour1%10];
		smg_buf[4]=smg_code[minute1/10];
		smg_buf[5]=smg_code[minute1%10];
		smg_buf[6]=smg_code[second1/10];
		smg_buf[7]=smg_code[second1%10];	
	}

}

void main()
{
	vSystem_Init();
	Timer2Init();
	vClock_Set(0,0,0);
	
	while(1)
	{
		vSMG_Proc();
		vKBD_Proc();
		vDs1302_Proc();
		vcnt_Proc();
	}
	
}

void vTimer2_ISR() interrupt 12
{
	vSmg_Disp();
	cnt_kbd++;
	if(b==1) cnt_relay++;
	if(z==1) cnt_buzzer++;
	vLED_Proc();
}
