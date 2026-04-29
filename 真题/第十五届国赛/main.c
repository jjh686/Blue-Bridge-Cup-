#include "system.h"

void System_init()
{
	P2P0_ctrl(0x80,0xff);
	P2P0_ctrl(0xa0,0x00);
}

#define Disp_zb 0
#define Disp_sped 1
#define Disp_para 2
u8 Disp_mode=Disp_zb;
#define State_RUN 0
#define State_IDLE 1
#define State_Wait 2
u8 sped_state=State_IDLE;

float r_para=1.0;
float r_para_true=1.0;
u8 r_para_disp;
char b_para=0;
char b_para_true=0;
bit para_set=0;

u16 x_tar=0,y_tar=0;
u16 x_dev=0,y_dev=0;

float sped=0;
u32 sped_dis=0;

bit rec_flag=0;
bit bar_flag=0;

u16 cnt_ne555,ne555_val;

void vSped_Proc()
{
	sped=3.14f*r_para_true*ne555_val/100.0f+b_para_true;
	if(sped<0)
		sped=0;
}

u8 cnt_kbd=0,key_val=0;
void vKBD_Proc()
{
	if(cnt_kbd>=20)
	{
		cnt_kbd=0;
		key_val=ucKBD_Keyio_Read();
		if(key_val==4)
		{
			if(rec_flag==1&&sped_state==State_IDLE)
			{
				sped_state=State_RUN;
				rec_flag=0;
			}
			else if(sped_state==State_RUN)
				sped_state=State_Wait;
			else if(sped_state==State_Wait&&bar_flag==0)
				sped_state=State_RUN;
		}
		if(key_val==5)
		{
			if(sped_state==State_IDLE)
			{
				x_dev=0,y_dev=0;
			}
		}
		if(key_val==8)
		{
			 Disp_mode=(Disp_mode+1)%3;
		     if(Disp_mode==Disp_para)
				 para_set=0;
			 if(Disp_mode==Disp_zb)
			 {
				 r_para_true=r_para;
				 b_para_true=b_para;
			 }
		}
		if(key_val==9)
		{
			if(Disp_mode==Disp_para)
			{
				para_set=!para_set;
			}
		}
		if(key_val==12)
		{
			if(Disp_mode==Disp_para)
			{
				if(para_set==0&&r_para<2.0f)
					r_para+=0.1f;
				if(para_set==1&&b_para<90)
					b_para+=5;
			}
		}
		if(key_val==13)
		{
			if(Disp_mode==Disp_para)
			{
				if(para_set==0&&r_para>1.0f)
					r_para-=0.1f;
				if(para_set==1&&b_para>-90)
					b_para-=5;
			}
		}
	}
}

u8 cnt_adc=0,ch1=0;
void vADC_Proc()
{
	if(cnt_adc>=10)
	{
		cnt_adc=0;
		ch1=ucADC_Read(0x41);
	}
}

u16 cnt_sonic,dist_val;
void vSonic_Proc()
{
	if(cnt_sonic>=100)
	{
		cnt_sonic=0;
		dist_val=ucDist_Get();
	}
}

void vNE555_Proc()
{
	if(cnt_ne555>=1000)
	{
		cnt_ne555=0;
		ne555_val=(TH0<<8)|TL0;
		TH0=0;
		TL0=0;
	}
}

void SendNb(u16 num)
{
	if(num>=100)
	{
		SendData(num/100+'0');
		SendData(num/10%10+'0');
		SendData(num%10+'0');
	}
	else if(num>=10)
	{
		SendData(num/10%10+'0');
		SendData(num%10+'0');
	}
	else
	{
		SendData(num%10+'0');
	}
}

u8 rx_len;
void vUART_Proc()
{
	u8 i=0,j=0;
	char x_tar_str[4]={0},y_tar_str[4]={0};
	if(rx_flag==1)
	{
		rx_flag=0;

		if(rx_buf[0]=='?')
		{
			if(sped_state==State_RUN)
				SendString("Busy");
			else if(sped_state==State_IDLE)
				SendString("Idle");
			else if(sped_state==State_Wait)
				SendString("Wait");
		}
		else if(rx_buf[0]=='#')
		{
			SendData('(');
			SendNb(x_dev);
			SendData(',');
			SendNb(y_dev);
			SendData(')');
		}
		else if(rx_buf[0]=='(')
		{
			rx_len=strlen(rx_buf);
			if(rx_buf[rx_len-1]==')')
			{
				i++;
				while(i<rx_len-1&&rx_buf[i]!=',')
				{
					x_tar_str[j++]=rx_buf[i++];
					if(x_tar_str[j-1]<'0'||x_tar_str[j-1]>'9')
					{
						SendString("Error");
						return;						
					}
				}
				if(rx_buf[i]!=',')
				{
					SendString("Error");
					return;
				}
				i++;
				j=0;
				while(i<rx_len-1&&rx_buf[i]!=')')
				{
					y_tar_str[j++]=rx_buf[i++];
					if(y_tar_str[j-1]<'0'||y_tar_str[j-1]>'9')
					{
						SendString("Error");
						return;						
					}
				}
				if(sped_state==State_IDLE)
				{
					x_tar=atoi(x_tar_str);
					y_tar=atoi(y_tar_str);
					SendString("Got it");
					rec_flag=1;
//					SendData(x_tar/100);
//					SendData(x_tar/10%10);
//					SendData(x_tar%10);
//					SendData(y_tar/100);
//					SendData(y_tar/10%10);
//					SendData(y_tar%10);
				}
				else
					SendString("Busy");
			}
		}
		else
			SendString("Error");
	}
}

void vSmg_Proc()
{
	if(Disp_mode==Disp_zb)
	{
		if(sped_state==State_Wait||sped_state==State_RUN)
		{
			smg_buf[0]=0x38;
			smg_buf[1]=smg_code[(x_tar>99)?(x_tar/100%10):10];
			smg_buf[2]=smg_code[(x_tar>9)?(x_tar/10%10):10];
			smg_buf[3]=smg_code[x_tar%10];
			smg_buf[4]=0x40;
			smg_buf[5]=smg_code[(y_tar>99)?(y_tar/100%10):10];
			smg_buf[6]=smg_code[(y_tar>9)?(y_tar/10%10):10];
			smg_buf[7]=smg_code[y_tar%10];
		}
		if(sped_state==State_IDLE)
		{
			smg_buf[0]=0x38;
			smg_buf[1]=smg_code[(x_dev>99)?(x_dev/100%10):10];
			smg_buf[2]=smg_code[(x_dev>9)?(x_dev/10%10):10];
			smg_buf[3]=smg_code[x_dev%10];
			smg_buf[4]=0x40;
			smg_buf[5]=smg_code[(y_dev>99)?(y_dev/100%10):10];
			smg_buf[6]=smg_code[(y_dev>9)?(y_dev/10%10):10];
			smg_buf[7]=smg_code[y_dev%10];			
		}
	}
	else if(Disp_mode==Disp_sped)
	{
		if(sped_state==State_RUN)
		{
			sped_dis=(u32)(sped*10); 
			smg_buf[0]=0x79;
			smg_buf[1]=smg_code[1];
			smg_buf[2]=0x00;
			smg_buf[3]=smg_code[(sped_dis>9999)?(sped_dis/10000%10):10];
			smg_buf[4]=smg_code[(sped_dis>999)?(sped_dis/1000%10):10];
			smg_buf[5]=smg_code[(sped_dis>99)?(sped_dis/100%10):10];
			smg_buf[6]=smg_code[sped_dis/10%10]|0x80;
			smg_buf[7]=smg_code[sped_dis%10];					
		}
		else if(sped_state==State_IDLE)
		{
			smg_buf[0]=0x79;
			smg_buf[1]=smg_code[2];
			smg_buf[2]=0x00;
			smg_buf[3]=smg_buf[4]=smg_buf[5]=smg_buf[6]=smg_buf[7]=0x40;
		}
		else if(sped_state==State_Wait)
		{
			smg_buf[0]=0x79;
			smg_buf[1]=smg_code[3];
			smg_buf[2]=0x00;			
			
		}
	}
	else if(Disp_mode==Disp_para)
	{
		r_para_disp=(u8)(r_para*10);
		smg_buf[0]=0x73;
		smg_buf[1]=smg_buf[4]=0x00;
		smg_buf[2]=smg_code[r_para_disp/10]|0x80;
		smg_buf[3]=smg_code[r_para_disp%10];
		if(b_para==0)
		{
			smg_buf[5]=smg_buf[6]=0x00;
			smg_buf[7]=smg_code[0];
		}
		else if(b_para>0)
		{
			smg_buf[5]=0x00;
			smg_buf[6]=smg_code[(b_para>9)?(b_para/10):10];
			smg_buf[7]=smg_code[b_para%10];
		}
		else
		{
			smg_buf[5]=smg_code[(-b_para>9)?11:10 ];
			smg_buf[6]=smg_code[(-b_para>9)?(-b_para/10):11];
			smg_buf[7]=smg_code[-b_para%10];			
		}
	}
}

void main()
{
	System_init();
	Timer2_Init();
	vCounter0_init();
	Uart1_Init();
	
	while(1)
	{
		vSmg_Proc();
		vKBD_Proc();
		vADC_Proc();
		vSonic_Proc();
		vNE555_Proc();
		vUART_Proc();
	}
}

void T2_init() interrupt 12
{
	Smg_Disp();
	cnt_kbd++;
	cnt_adc++;
	cnt_sonic++;
	cnt_ne555++;
	vRX_time_Proc();
	vSped_Proc();
}
