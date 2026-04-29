#include "system.h"

u8 disp_mode,dacsc_mode,kbd_lval,k,h,s13_long_flag = 0;
s8 temp_jz,temp_jz_active;
u16 cnt_s13;

float temp_fl,temp_fljs[2]={0},temp_fljs_cz;
	
void vSystem_init()
{
	vP2P0_Ctrl(0x80,0xff);
	vP2P0_Ctrl(0xa0,0);
	vP2P0_Ctrl(0xe0,0xff);
}

u8 cnt_DAC;
u8 dac_val_temp,dac_val_hand=100;
void vDAC_Proc()
{
	if(cnt_DAC>=2)
	{
		cnt_DAC=0;
		if(dacsc_mode==0)
		{
			if(temp_fl<10.f)
				dac_val_temp=102;
			else if(temp_fl>40.f)
				dac_val_temp=255;
			else
			dac_val_temp=(u8)(5.1f*temp_fl+51.f);
			vDAC_Write(dac_val_temp);			
		}
		if(dacsc_mode==1)
		{
			vDAC_Write(dac_val_hand);
		}
	}
}

u8 cnt_temp;
void vTemp_Proc()
{
	if(cnt_temp>=100)
	{
		cnt_temp=0;
		temp_fl=ucTemp_Read();
		temp_fl=temp_fl+(float)temp_jz_active;			
		temp_fljs[1]=temp_fljs[0];
		temp_fljs[0]=temp_fl;
		temp_fljs_cz=temp_fljs[0]-temp_fljs[1];
		if(temp_fljs_cz<0.f)
			temp_fljs_cz=-temp_fljs_cz;
	}
}

u8 cnt_kbd;
u8 kbd_val;
void vKBD_Proc()
{
	if(cnt_kbd>=10)
	{
		cnt_kbd=0;
		kbd_val=ucKbd_keyio_Read();
		if(kbd_val==12)
		{
			if(disp_mode==2)
				temp_jz_active=temp_jz;
			disp_mode=(disp_mode+1)%3;
		}
		else if(kbd_val==16)
		{
			if(disp_mode==2&&temp_jz<9)
				temp_jz+=1;
			if(disp_mode==1&&dac_val_hand<255)
				dac_val_hand+=5;
			if(disp_mode==0&&k==0)
				vP2P0_Ctrl(0xa0,0x10);
		}
		else if(kbd_val==17)
		{
			if(disp_mode==2&&temp_jz>-9)
				temp_jz-=1;			
			if(disp_mode==1&&dac_val_hand>0)
				dac_val_hand-=5;
			if(disp_mode==0&&k==0)
				vP2P0_Ctrl(0xa0,0x00);
		}
		else if(kbd_val==13)
			if(s13_long_flag == 1) {
				// 如果刚刚发生过长按，这里只是松手，清除标志位，什么也不做
				s13_long_flag = 0; 
			} else {
				// 只有没发生长按的正常松手，才算作短按
				dacsc_mode=(dacsc_mode+1)%2;
			}
	}
}


void vRelay_Ctrl()
{
	kbd_lval=ucKbd_keyio_state();
	if(kbd_lval==13)
		cnt_s13++;
	else
		cnt_s13=0;	
	if(cnt_s13>=1500)
	{
		h=1;
		s13_long_flag = 1;
	}
		
	if(h==1&&kbd_lval!=13)
	{
		k=~k;
		h=0;
	}
}

u16 cnt_l2,cnt_l3,cnt_l4,cnt_l4_400;
u8 l2_locate,led_ctrl=0xff,l3_locate,temptb_loc;
void vLED_Proc()
{
	if(dacsc_mode==0)
		led_ctrl&=~0x01;
	else
		led_ctrl|=0x01;
	if(temp_fljs[0]-temp_fljs[1]>0.f&&l2_locate==0)
		l2_locate=1;
	if(l2_locate==1)
	{
		cnt_l2++;
		led_ctrl&=~0x02;
	}
	if(cnt_l2>=2000)
	{
		cnt_l2=0;
		l2_locate=0;
		led_ctrl|=0x02;
	}
	if(temp_fljs[0]-temp_fljs[1]<0.f&&l3_locate==0)
		l3_locate=1;
	if(l3_locate==1)
	{
		cnt_l3++;
		led_ctrl&=~0x04;
	}
	if(cnt_l3>=2000)
	{
		cnt_l3=0;
		l3_locate=0;
		led_ctrl|=0x04;
	}
	if(temp_fljs_cz>1.f&&temptb_loc==0)
		temptb_loc=1;
	if(temptb_loc==1)
	{
		cnt_l4++;
		cnt_l4_400=cnt_l4%400;
		if((cnt_l4_400)<=200)
			led_ctrl&=~0x08;
		else if((cnt_l4_400)<=400)
			led_ctrl|=0x08;
		if(cnt_l4>=3000)
		{
			cnt_l4=0;
			temptb_loc=0;
			led_ctrl|=0x08;
		}
	}
	if(k==0)
		led_ctrl&=~0x80;
	else
		led_ctrl|=0x80;
	vP2P0_Ctrl(0x80,led_ctrl);
}

void vSmg_Process()
{
	if(disp_mode==0)
	{
		smg_buf[0]=0x39;
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=0x00;
		smg_buf[6]=smg_code[(u8)temp_fl/10];
		smg_buf[7]=smg_code[(u8)temp_fl%10];		
	}
	else if(disp_mode==1)
	{
		smg_buf[0]=0x77;
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=smg_code[(dac_val_hand>=100) ? (dac_val_hand/100) : 10];
		smg_buf[6]=smg_code[(dac_val_hand>=10) ? (dac_val_hand/10%10) : 10];
		smg_buf[7]=smg_code[dac_val_hand%10];			
	}
	else if(disp_mode==2)
	{
		smg_buf[0]=0x73;
		smg_buf[1]=0x00;
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=0x00;
		smg_buf[6]=smg_code[(temp_jz<0) ? 11 : 10];
		smg_buf[7]=smg_code[(temp_jz<0) ? -temp_jz : temp_jz];			
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
		vDAC_Proc();
		vTemp_Proc();
		vKBD_Proc();
	}
}

void vTimer2_init() interrupt 12
{
	vSmg_disp();
	cnt_temp++;
	cnt_DAC++;
	cnt_kbd++;
	vRelay_Ctrl();
	vLED_Proc();
}
