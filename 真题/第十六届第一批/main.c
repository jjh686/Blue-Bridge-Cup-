#include "system.h"

u8 disp_mode;
u8 k,c,d,e,f,temp_ck=30,h=0,dist_ck=30,j,l,m;
s16 g;
u16 a[2]={0},i;
void vSystem_Init()
{
	vP2P0_Ctrl(0xa0,0);
	vP2P0_Ctrl(0x80,0xff);
	vP2P0_Ctrl(0xe0,0xff);
}

u8 cnt_KBD;
u8 key_val;
void vKBD_Proc()
{
	if(cnt_KBD>=10)
	{
		cnt_KBD=0;
		key_val=KBD_Keyio_Read();
		if(key_val==4)
		{	disp_mode=(disp_mode+1)%4;
			if(disp_mode==2)
				h=0;
		}
		if(key_val==5)
			if(disp_mode==2)
				h=(h+1)%2;
		if(key_val==8)
		{
			if(disp_mode==2)
			{
				if(h==0&&temp_ck<80)
					temp_ck+=1;
				if(h==1&&dist_ck<80)
					dist_ck+=5;				
			}
		}
		if(key_val==9)
		{
			if(disp_mode==2)
			{
				if(h==0&&temp_ck>20)
					temp_ck-=1;
				if(h==1&&dist_ck>20)
					dist_ck-=5;					
			}
		}
	}
}

u8 cnt_ds18b20;
u16 read_temp;
void vDs18b20_Proc()
{
	u16 temp;
	if(cnt_ds18b20>=100)
	{
		cnt_ds18b20=0;
		temp=(u16)ucRd_Temp();
		if(temp != 85 && temp < 120) // 过滤掉默认值和错误断线值
			read_temp = temp;
	}
}

u8 cnt_adc;
u8 ch1;
void vAdc_Proc()
{
	float adc_val;
	if(cnt_adc>=50)
	{
		cnt_adc=0;
		EA=0;
		ch1=vread_adc(0x41);
		EA=1;
		adc_val=(float)ch1*4.0/255;
		if(adc_val>=3.0)
			k=1;
		else if(adc_val>=2.0)
			k=2;
		else if(adc_val>=0.5)
			k=3;
		else
			k=4;
	}
}

u16 cnt_sonic;
u16 dist_sonic=999;
void vSonic_Proc()
{
	u16 temp_dist;
	if(cnt_sonic>=1000)
	{
		cnt_sonic=0;
		temp_dist=ucDistance_Get();
		if(temp_dist!=0)
			dist_sonic=temp_dist;
		a[1]=a[0];
		a[0]=dist_sonic;
		if(d==0)
		{
			g=a[0]-a[1];
			if(g<0)
				g=-g;
				f=c;
				if(g<5)
					c=1;
				else if(g<10)
					c=2;
				else
					c=3;
				if(f==c)
					d=0;
				else
				{
					d=1;
				}
		}
		if(d==1)
			e++;
		if(e==3)
		{
			e=0;
			d=0;
		}
	}
}

u16 cnt_realy;
void vRelay_Ctrl()
{
	u8 key_state_cor=P3&0x0c,key_state_ver=P4&0x04;
	if(read_temp>temp_ck&&dist_sonic<dist_ck)
	{
		vP2P0_Ctrl(0xa0,0x10);
		if(j==0)
			i++;
		j=1;
	}
	else
	{
		vP2P0_Ctrl(0xa0,0x00);
		j=0;
	}
	if(key_state_cor==0x00&&key_state_ver==0x00&&disp_mode==3)
	{
		l=1;
		if(cnt_realy>=2000)
			i=0;
	}
	else
		l=0;
}

u8 led_ctrl;
u8 led_cnt;
void vLED_Proc()
{
	static u8 n=0;
	
	led_ctrl = 0xff; // 第一步：进来先默认全部熄灭，后面只管“点亮”谁

	// 处理 L1 ~ L4
	if(dist_sonic < dist_ck)
	{
		if(k==1)      led_ctrl &= ~0x01; // 只亮L1
		else if(k==2) led_ctrl &= ~0x03; // 亮L1, L2
		else if(k==3) led_ctrl &= ~0x07; // 亮L1, L2, L3
		else if(k==4) led_ctrl &= ~0x0f; // 亮L1-L4
	}

	// 处理 L8
	if(c==1)
	{
		led_ctrl |= 0x80; // 熄灭L8 (因为上面已经是0xff了，这句写上只为逻辑清晰)
		m = 0; // 重置闪烁标志
	}
	else if(c==2)
	{
		led_ctrl &= ~0x80; // 强制点亮L8
		m = 0;
	}
	else if(c==3)
	{
		m = 1; // 开启闪烁计数
		if(led_cnt >= 100)
		{
			led_cnt = 0;
			n = ~n;
		}
		if(n == 0) led_ctrl |= 0x80;
		else       led_ctrl &= ~0x80;
	}
	else
	{
		m = 0; // 其他情况务必清零 m，否则定时器里的 led_cnt 会一直乱加
	}
	
	vP2P0_Ctrl(0x80, led_ctrl);
}

void vSmg_proc()
{
	if(disp_mode==0)
	{
		smg_buf[0]=0x39;
		smg_buf[1]=smg_code[read_temp/10];
		smg_buf[2]=smg_code[read_temp%10];
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=0x00;
		smg_buf[6]=0x37;
		smg_buf[7]=smg_code[k];		
	}
	if(disp_mode==1)
	{
		smg_buf[0]=0x38;
		smg_buf[1]=smg_code[c];
		smg_buf[2]=0x00;
		smg_buf[3]=0x00;
		smg_buf[4]=0x00;
		smg_buf[5]=smg_code[dist_sonic/100];
		smg_buf[6]=smg_code[dist_sonic/10%10];
		smg_buf[7]=smg_code[dist_sonic%10];			
	}
	if(disp_mode==2)
	{
		if(h==0)
		{
			smg_buf[0]=0x73;
			smg_buf[1]=0x39;
			smg_buf[2]=0x00;
			smg_buf[3]=0x00;
			smg_buf[4]=0x00;
			smg_buf[5]=0x00;
			smg_buf[6]=smg_code[temp_ck/10];	
			smg_buf[7]=smg_code[temp_ck%10];				
		}
		if(h==1)
		{
			smg_buf[0]=0x73;
			smg_buf[1]=0x38;
			smg_buf[2]=0x00;
			smg_buf[3]=0x00;
			smg_buf[4]=0x00;
			smg_buf[5]=0x00;
			smg_buf[6]=smg_code[dist_ck/10];	
			smg_buf[7]=smg_code[dist_ck%10];				
		}
	}
	if(disp_mode==3)
	{
			smg_buf[0]=0x37;
			smg_buf[1]=0x39;
			smg_buf[2]=0x00;
			smg_buf[3]=0x00;
			smg_buf[4]=smg_code[(i>=1000) ? (i/1000) : 10];
			smg_buf[5]=smg_code[(i>=100) ? (i/100%10) : 10];
			smg_buf[6]=smg_code[(i>=10) ? (i/10%10) : 10];	
			smg_buf[7]=smg_code[i%10];		
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
		vKBD_Proc();
		vDs18b20_Proc();
		vAdc_Proc();
		vSonic_Proc();
	}
}

void vTimer2_init() interrupt 12
{
	vSmg_Disp();
	cnt_KBD++;
	cnt_ds18b20++;
	cnt_adc++;
	cnt_sonic++;
	vRelay_Ctrl();
	if(l==1)
		cnt_realy++;
	else 
		cnt_realy=0;
	vLED_Proc();
	if(m==1)
		led_cnt++;
	else
		led_cnt=0;
}
