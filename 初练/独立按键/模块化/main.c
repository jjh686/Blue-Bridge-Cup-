#include "system.h"

HexToBin led_ctrl,uln_ctrl;

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
	led_ctrl.hex=0xff;
	vDevice_Ctrl(0x80,led_ctrl.hex);	
}

u8 cnt_key;
u8 s4_number;
void vBTN_State_Process(void)
{
	u8 key_val;
	if(cnt_key>=10)
	{
		cnt_key=0;
		key_val = vBTN_Read_State();
		if(key_val==4)
		{
			s4_number++;
		}
	}
}

void vBTN_ThreeLine_Process(void)
{
	if(cnt_key>=10)
	{
		cnt_key=0;
		vBTN_Read_ThreeLine();
		if(Trg & 0x08)
		{
			s4_number++;
		}
		if(Trg & 0x04)
		{
			s4_number--;
		}
		if(Trg & 0x02)
		{
			s4_number+=10;
		}
		if(Trg & 0x01)
		{
			s4_number-=10;
		}
	}
}

void vSMG_Process()
{
	smg_buf[0]=smg_code[s4_number/100];
	smg_buf[1]=smg_code[s4_number/10%10];
	smg_buf[2]=smg_code[s4_number%10];
	smg_buf[3]=0x00;
	smg_buf[4]=0x00;
	smg_buf[5]=0x00;
	smg_buf[6]=0x00;
	smg_buf[7]=0x00;
}

void main(void)
{
	vSystem_Init();
	vTimer2_Init();
	while(1)
	{
		vSMG_Process();
		vBTN_ThreeLine_Process();
	}
}

//中断服务程序
void vTimer2_ISR() interrupt 12     //中断入口
{
	cnt_key++;
	vSMG_Display();
}
