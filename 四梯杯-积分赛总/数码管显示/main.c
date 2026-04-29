#include "system.h"

void Delay1000ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 46;
	j = 153;
	k = 245;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void vSystem_init()
{
	vP2p0_ctrl(0x80,0xff);
	vP2p0_ctrl(0xa0,0);
	vP2p0_ctrl(0xc0,0);
}

u8 cnt_btn;
u8 key_val;
u8 key_val_2;
u16 btn_number;
void vBTN_Process()
{
	static u8 k=0;
	if(cnt_btn>=10)
	{
		cnt_btn=0;
		key_val=ucBTN_Read_State();
		key_val_2=ucBTN_KeyIO_State();
		if(key_val_2==4)
		{
			if(k==0)
			{
				Delay1000ms();
			}
				btn_number++;
			k=1;
		}
		if(key_val_2!=4)
		{
			k=0;
		}		
		if(key_val==5)
		{
			btn_number+=10;
		}
		if(key_val==6)
		{
			btn_number=0;
		}		
	}
}

u8 btn_number_3;
u8 btn_number_4;
void vBTN_Number_Process()
{
	if(btn_number>=100)
	{
		btn_number_3=smg_code[btn_number/100%10];
	}
	else
		btn_number_3=0x00;
	if(btn_number>=1000)
	{
		btn_number_4=smg_code[btn_number/1000];
	}
	else
		btn_number_4=0x00;	
}

void vSmg_Process()
{
	smg_buf[0]=0x00;
	smg_buf[1]=0x00;
	smg_buf[2]=0x00;
	smg_buf[3]=0x00;
	smg_buf[4]=btn_number_4;
	smg_buf[5]=btn_number_3;
	smg_buf[6]=smg_code[btn_number/10%10]|0x80;
	smg_buf[7]=smg_code[btn_number%10];	
}

void main()
{
	vSystem_init();
	vTimer2_Init();
	
	while(1)
	{
		vSmg_Process();
		vBTN_Process();
		vBTN_Number_Process();
	}
	
}

void t2int() interrupt 12
{
	cnt_btn++;
	vSmg_Disp();
}
