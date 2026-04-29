#include "system.h"

HexToBin led_ctrl,uln_ctrl;

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
	led_ctrl.hex=0xff;
	vDevice_Ctrl(0x80,led_ctrl.hex);	
}

u8 cnt_adc;
u8 ch0,ch1,ch3;
void vADC_Process()
{
	if(cnt_adc>=10)
	{
		ch1 = ucRead_ADC(0x41);
	}
}

void vSMG_Process()
{
	smg_buf[0]=smg_code[ch1/100];
	smg_buf[1]=smg_code[ch1/10%10];
	smg_buf[2]=smg_code[ch1%10];
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
		vADC_Process();
	}
}

//中断服务程序
void vTimer2_ISR() interrupt 12     //中断入口
{
	cnt_adc++;
	vSMG_Display();
}
