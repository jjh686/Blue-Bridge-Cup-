#include "system.h"

void vSystem_Init()
{
	vP2P0_Ctrl(0xa0,0);
	vP2P0_Ctrl(0x80,0xff);
	vP2P0_Ctrl(0xe0,0xff);
}

#define Disp_Env 0
#define Disp_Mov 1
#define Disp_Par 2
#define Disp_Sta 3
u8 disp_mode=Disp_Env;
u8 light_state=1;
u8 mov_state=1;
u8 temp_para=30;
u8 dist_para=30;
bit sel_index=0;
u16 relay_nc=0;
bit lock_state=0;

u8 cnt_tmp=0;
u16 tmp_now=0;
bit tmp_flag=0;
void vDs18b20_Proc()
{
	if(cnt_tmp>=100)
	{
		cnt_tmp=0;
		tmp_now=(u16)(ucRd_Temp());
		if(tmp_now>temp_para)
			tmp_flag=1;
		else
			tmp_flag=0;
	}
}

u16 cnt_sonic=0,dist_sonic=0,dist_sonic_prev=0;
u16 dist_diff=0;
bit closed_flag=0;
u8 move_state_prev=1,lock_cnt=0;
void vSonic_Proc()
{
	if(cnt_sonic>=1000)
	{
		cnt_sonic=0;
		dist_sonic=ucDistance_Get_PCA ();
		if(dist_sonic<dist_para)
			closed_flag=1;
		else
			closed_flag=0;
		
		if(dist_sonic>dist_sonic_prev)
			dist_diff=dist_sonic-dist_sonic_prev;
		else
			dist_diff=dist_sonic_prev-dist_sonic;
		dist_sonic_prev=dist_sonic;
		
		if(lock_cnt>0)
		{
			lock_cnt--;
			return;
		}
		
		if(dist_diff<5)
			mov_state=1;
		else if(dist_diff<10)
			mov_state=2;
		else
			mov_state=3;
		
		if(move_state_prev!=mov_state)
		{
			move_state_prev=mov_state;
			lock_cnt=3;
		}
	}
}

u8 cnt_kbd=0,kbd_val,key_h=0,key_s=0,kbd_val_zh=0;
u32 s8s9_time=0;
void vKbd_Proc()
{
	if(cnt_kbd>=20)
	{
		cnt_kbd=0;
		kbd_val=KBD_Keyio_Read();
		kbd_val_zh=KBD_Keyio_state_zh();
		if(kbd_val==4)
		{
			disp_mode=(disp_mode+1)%4;
			if(disp_mode==Disp_Par)
			{
				sel_index=0;
				lock_state=1;
			}
			else if(disp_mode==Disp_Sta)
			lock_state=0;
		}
		if(kbd_val==5)
		{
			if(disp_mode==Disp_Par)
				sel_index=!sel_index;
		}
		if(kbd_val==8)
		{
			if(disp_mode==Disp_Par)
			{
				if(sel_index==0&&temp_para<80)
					temp_para++;
				else if(sel_index==1&&dist_para<80)
					dist_para+=5;
			}
		}
		if(kbd_val==9)
		{
			if(disp_mode==Disp_Par)
			{
				if(sel_index==0&&temp_para>20)
					temp_para--;
				else if(sel_index==1&&dist_para>20)
					dist_para-=5;  
			}
		}
		
		key_h=P3&0x0c;key_s=P4&0x04;
		if(disp_mode==Disp_Sta)
		{
			if(key_h==0x00&&key_s==0x00)
				s8s9_time++;
			else
				s8s9_time=0;
			if(s8s9_time>100)
				relay_nc=0;
		}
	}
}

u8 cnt_adc=0,adc_val=0;
float adc_volt;
void vADC_Proc()
{
	if(cnt_adc>=100)
	{
		cnt_adc=0;
		adc_val=vread_adc(0x41);
		adc_volt=adc_val/51.0f;
		if(adc_volt>=3.0f)  
			light_state=1;
		else if(adc_volt>=2.0f)
			light_state=2;
		else if(adc_volt>=0.5f)
			light_state=3;
		else
			light_state=4;
	}
}

u8 led_ctrl=0xff;
u32 cnt_l8=0;
void vLED_Proc()
{
	if(lock_state==0)
	{
		if(closed_flag==1)
		{
			if(light_state==1)
			{
				led_ctrl&=~0x01;
				led_ctrl|=0x0e;
			}
			else if(light_state==2)
			{
				led_ctrl&=~0x03;
				led_ctrl|=0x0c;		
			}
			else if(light_state==3)
			{
				led_ctrl&=~0x07;
				led_ctrl|=0x08;		
			}
			else if(light_state==4)
				led_ctrl&=~0x0f;	
		}
		else
			led_ctrl|=0x0f;
		
		cnt_l8++;
		if(mov_state==1)
			led_ctrl|=0x80;
		else if(mov_state==2)
			led_ctrl&=~0x80;
		else
		{
			if(cnt_l8%200<100) 
				led_ctrl&=~0x80;
			else
				led_ctrl|=0x80;
		}
		
		vP2P0_Ctrl(0x80,led_ctrl);
	}
}

u8 relay_ctrl=0x00;
bit relay_flag=0,relay_flag_prev=0;
void vRelay_Proc()
{
	if(lock_state==0)
	{
		relay_flag_prev=relay_flag;
		if(closed_flag==1&&tmp_flag==1)
		{
			relay_ctrl|=0x10; 
			relay_flag=1;
			if(relay_flag_prev!=relay_flag)
				relay_nc++;
		}
		else
		{
			relay_ctrl&=~0x10;
			relay_flag=0;
		}
		vP2P0_Ctrl(0xa0,relay_ctrl);
	}
}

void vSmg_proc()
{
	if(disp_mode==Disp_Env)
	{
		smg_buf[0]=0x39;
		smg_buf[1]=smg_code[tmp_now/10];
		smg_buf[2]=smg_code[tmp_now%10];
		smg_buf[3]=smg_buf[4]=smg_buf[5]=0x00;
		smg_buf[6]=0x37;
		smg_buf[7]=smg_code[light_state];	
	}	
	else if(disp_mode==Disp_Mov)
	{
		smg_buf[0]=0x38;
		smg_buf[1]=smg_code[mov_state];
		smg_buf[2]=smg_buf[3]=smg_buf[4]=0x00;
		smg_buf[5]=smg_code[dist_sonic/100];
		smg_buf[6]=smg_code[dist_sonic/10%10];
		smg_buf[7]=smg_code[dist_sonic%10];	
	}	
	else if(disp_mode==Disp_Par)
	{
		if(sel_index==0)
		{
			smg_buf[0]=0x73;
			smg_buf[1]=0x39;
			smg_buf[2]=smg_buf[3]=smg_buf[4]=smg_buf[5]=0x00;
			smg_buf[6]=smg_code[temp_para/10];
			smg_buf[7]=smg_code[temp_para%10];
		}
		else
		{
			smg_buf[0]=0x73;
			smg_buf[1]=0x38;
			smg_buf[2]=smg_buf[3]=smg_buf[4]=smg_buf[5]=0x00;
			smg_buf[6]=smg_code[dist_para/10];
			smg_buf[7]=smg_code[dist_para%10];			
		}
	}
	else if(disp_mode==Disp_Sta)
	{
		smg_buf[0]=0x37;
		smg_buf[1]=0x39;
		smg_buf[2]=smg_buf[3]=0x00;
		smg_buf[4]=smg_code[(relay_nc>999)?(relay_nc/1000%10):10];
		smg_buf[5]=smg_code[(relay_nc>99)?(relay_nc/100%10):10];
		smg_buf[6]=smg_code[(relay_nc>9)?(relay_nc/10%10):10];
		smg_buf[7]=smg_code[relay_nc%10];	
	}		
}

void main()
{
	vSystem_Init();
	Timer2Init();
	while((u8)ucRd_Temp()==85);
	
	while(1)
	{
		vSmg_proc();
		vDs18b20_Proc();
		vSonic_Proc();
		vKbd_Proc();
		vADC_Proc();
	}
}

void vTimer2_init() interrupt 12
{
	vSmg_Disp();
	cnt_tmp++;
	cnt_sonic++;
	cnt_kbd++;
	cnt_adc++;
	vLED_Proc();
	vRelay_Proc();
}
