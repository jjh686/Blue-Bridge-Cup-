#include "system.h"
	
void vSystem_init()
{
	vP2P0_Ctrl(0x80,0xff);
	vP2P0_Ctrl(0xa0,0);
	vP2P0_Ctrl(0xe0,0xff);
}

#define Disp_Tmp 0
#define Disp_Dac 1
#define Disp_Cal 2
u8 disp_mode=Disp_Tmp;
u8 dac_val=100;
char cal_val=0;
char cal_val_true=0; 
bit lock_state=0;
u8 relay_ctrl=0x00;
bit dac_mode=0;

u8 cnt_tmp;
s16 tmp_val=0,tmp_val_true=0,tmp_val_true_prev=0;
u8 tmp_alarm=0;

u32 cnt_l2=0,cnt_l3=0,cnt_l4=0;
void vDs18b20_Proc()
{
	if(cnt_tmp>=100)
	{
		cnt_tmp=0;
		tmp_val=(int)(ucTemp_Read());
		tmp_val_true_prev=tmp_val_true;
		tmp_val_true=tmp_val+cal_val_true;
		if(tmp_val_true-tmp_val_true_prev>=1)
		{
			cnt_l2=2000;
			cnt_l4=3000;
			tmp_alarm=1;
		}
		else if(tmp_val_true-tmp_val_true_prev<=-1)
		{
			cnt_l3=2000;
			cnt_l4=3000; 
			tmp_alarm=2; 
		}
		else
			tmp_alarm=0;
	}
} 

u8 cnt_kbd,kbd_val,kbd_long;
u32 time_s13=0;
void vKBD_Proc()
{
	if(cnt_kbd>=20)
	{
		cnt_kbd=0;
		kbd_val=ucKbd_keyio_Read();
		kbd_long=ucKbd_keyio_state();
		if(kbd_val==12)
		{
			disp_mode=(disp_mode+1)%3;
			if(disp_mode==Disp_Tmp)
				cal_val_true=cal_val;
		}
		if(kbd_val==13)
		{
//			dac_mode=!dac_mode;
		}
		if(kbd_long==13)
		{
			time_s13++;
		}
		if(kbd_val==0&&kbd_long==0)
		{
			if(time_s13>=75)
				lock_state=!lock_state;
			else if(time_s13!=0)
				dac_mode=!dac_mode;
			time_s13=0;
		}
		if(kbd_val==16)
		{
			if(disp_mode==Disp_Dac&&dac_val<255)
				dac_val+=5;
			if(disp_mode==Disp_Cal&&cal_val<9)
				cal_val+=1;
			if(disp_mode==Disp_Tmp&&lock_state==0)
				relay_ctrl|=0x10;
		}
		if(kbd_val==17)
		{
			if(disp_mode==Disp_Dac&&dac_val>0)
				dac_val-=5;
			if(disp_mode==Disp_Cal&&cal_val>-9)
				cal_val-=1;
			if(disp_mode==Disp_Tmp&&lock_state==0)
				relay_ctrl&=~0x10;			
		}
	}
}

void vRelay_Proc()
{
	vP2P0_Ctrl(0xa0,relay_ctrl);
}

u8 cnt_dac=0;
void vDAC_Proc()
{
	if(cnt_dac>=10)
	{
		cnt_dac=0;
		if(dac_mode==1)
			vDAC_Write(dac_val);
		else
		{
			if(tmp_val_true<=10)
				vDAC_Write(102);
			else if(tmp_val_true>=40)
				vDAC_Write(255);
			else
				vDAC_Write(5.1f*tmp_val_true+51.0f);
		}
	}
}

u32 cnt_led=0;
u8 led_ctrl=0xff;
void vLED_Proc()
{
	cnt_led++;
	if(dac_mode==0)
		led_ctrl&=~0x01; 
	else
		led_ctrl|=0x01;
	if(cnt_l2>0)
	{
		cnt_l2--;
		led_ctrl&=~0x02; 
	}
	else
		led_ctrl|=0x02;
	if(cnt_l3>0)
	{
		cnt_l3--;
		led_ctrl&=~0x04; 
	}
	else
		led_ctrl|=0x04;
	if(cnt_l4>0)
	{
		cnt_l4--; 
		if(cnt_led%400<200)
			led_ctrl&=~0x08; 
		else
			led_ctrl|=0x08;
	}
	else
		led_ctrl|=0x08;
	if(lock_state==0)
		led_ctrl&=~0x80; 
	else
		led_ctrl|=0x80;
	vP2P0_Ctrl(0x80,led_ctrl); 
}

void vSmg_Process()
{
	if(disp_mode==Disp_Tmp)
	{
		smg_buf[0]=0x39;
		smg_buf[1]=smg_buf[2]=smg_buf[3]=smg_buf[4]=smg_buf[5]=0x00;
		smg_buf[6]=smg_code[tmp_val_true/10];
		smg_buf[7]=smg_code[tmp_val_true%10];		
	}
	else if(disp_mode==Disp_Dac)
	{
		smg_buf[0]=0x77;
		smg_buf[1]=smg_buf[2]=smg_buf[3]=smg_buf[4]=0x00;
		smg_buf[5]=smg_code[(dac_val>99)?(dac_val/100%10):10];
		smg_buf[6]=smg_code[(dac_val>9)?(dac_val/10%10):10];
		smg_buf[7]=smg_code[dac_val%10];
	}
	else if(disp_mode==Disp_Cal)
	{
		smg_buf[0]=0x73;
		smg_buf[1]=smg_buf[2]=smg_buf[3]=smg_buf[4]=smg_buf[5]=0x00;
		if(cal_val>=0)
		{
			smg_buf[6]=0x00;
			smg_buf[7]=smg_code[cal_val];
		}
		else
		{
			smg_buf[6]=0x40;
			smg_buf[7]=smg_code[-cal_val];
		}
	}	
}

void main()
{
	vSystem_init();
	Timer2Init();
	while((u8)ucTemp_Read()==85);
	
	while(1)
	{
		vSmg_Process();
		vDs18b20_Proc();
		vKBD_Proc();
		vDAC_Proc();
	}
}

void vTimer2_init() interrupt 12
{
	vSmg_disp();
	cnt_tmp++;
	cnt_kbd++;
	vRelay_Proc();
	cnt_dac++;
	vLED_Proc();
}
