/*************************************************
File name: 	 【PCF8591】DAC输出电压
Description: 
		学会DAC输出电压的函数

Copyright (c) 电子设计工坊 dianshe.taobao.com
All rights reserved
*************************************************/
#include "system.h"
HexToBin led_ctrl,uln_ctrl;

/*
  * @brief  系统初始化函数：关闭继电器、蜂鸣器
  * @param  None
  * @retval None
  * @author dianshe.taobao.com
  */
void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0); 				/* 关闭蜂鸣器、继电器*/
	led_ctrl.hex=0xff;
	vDevice_Ctrl(0x80,led_ctrl.hex); 	/* 关闭LED*/
}

u8 hour,minute,second;
void vDS1302_Process()
{
	second = BCDToDec(Read_Ds1302_Byte(0x81));
	minute = BCDToDec(Read_Ds1302_Byte(0x83));
	hour = BCDToDec(Read_Ds1302_Byte(0x85));
}

//数码管操作函数
void vSMG_Process()
{
	smg_buf[0]=smg_code[hour/10];		//旋转电位器的电压值
	smg_buf[1]=smg_code[hour%10]|0x80;
	smg_buf[2]=smg_code[minute/10];
	smg_buf[3]=smg_code[minute%10]|0x80;
	smg_buf[4]=smg_code[second/10];
	smg_buf[5]=smg_code[second%10];
	smg_buf[6]=0x00;
	smg_buf[7]=0x00;
}

void main(void)
{
	vSystem_Init();
	vClock_Set(23,59,55);
	vTimer2_Init();
	while(1)
	{
		vSMG_Process();
		vDS1302_Process();
	}
}


//中断服务程序
void vTimer2_ISR() interrupt 12     	//中断入口
{
	vSMG_Display();
}
