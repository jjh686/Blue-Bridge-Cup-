/*************************************************
File name: 	 【PCF8591】ADC值的转换
Description: 
		学会：将ADC值转换成实际电压、缩放范围、划分区间。

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
//ADC采样
u8 cnt_adc;
u8 ch0,ch1,ch3;
u16 ch3_volt;
u8 ch3_0_99,ch3_1_5;
void vADC_Process()
{
	if(cnt_adc>=10)
	{
		cnt_adc=0;
		ucRead_ADC(0x41);
		ch1 = ucRead_ADC(0x41);			//光敏电阻的AD值 - 通道1
		ucRead_ADC(0x43);
		ch3 = ucRead_ADC(0x43);			//旋转电位器的AD值 - 通道3
		
		ch3_volt = ch3*100/51;			// 0~255 --> 0~500
		ch3_0_99 = ch3/2.57f;			// 0~255 --> 0~99
		ch3_1_5 = ch3/51.1f + 1;
	}
}


//数码管操作函数
void vSMG_Process()
{
	smg_buf[0]=smg_code[ch3_volt/100]|0x80;			//旋转电位器的电压值
	smg_buf[1]=smg_code[ch3_volt/10%10];	
	smg_buf[2]=smg_code[ch3_volt%10];
	smg_buf[3]=0x00;
	smg_buf[4]=smg_code[ch3_0_99/10];
	smg_buf[5]=smg_code[ch3_0_99%10];		
	smg_buf[6]=0x00;
	smg_buf[7]=smg_code[ch3_1_5];
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
void vTimer2_ISR() interrupt 12     	//中断入口
{
	cnt_adc++;
	vSMG_Display();
}
