#include "system.h"

HexToBin led_ctrl,uln_ctrl;

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
	led_ctrl.hex=0xff;
	vDevice_Ctrl(0x80,led_ctrl.hex);	
}

u8 temperature;
u32 temp_float;
u8 cnt_temp;
void vDS18B20_Read()
{
	if(cnt_temp>=100)
	{
		cnt_temp=0;
//		temperature =(u8)fRead_temperature();
		temp_float =(u32) (fRead_temperature()*10000);
	}
}

void vSMG_Process()
{
	smg_buf[0]=smg_code[temperature/10];
	smg_buf[1]=smg_code[temperature%10];
	smg_buf[2]=smg_code[temp_float/100000];
	smg_buf[3]=smg_code[temp_float/10000%10]|0x80;
	smg_buf[4]=smg_code[temp_float/1000%10];
	smg_buf[5]=smg_code[temp_float/100%10];
	smg_buf[6]=smg_code[temp_float/10%10];
	smg_buf[7]=smg_code[temp_float%10];
}

void main(void)
{
	vSystem_Init();
	vTimer2_Init();
	while(1)
	{
		vSMG_Process();
		vDS18B20_Read();
	}
}

//中断服务程序
void vTimer2_ISR() interrupt 12     //中断入口
{
	cnt_temp++;
	vSMG_Display();
}
