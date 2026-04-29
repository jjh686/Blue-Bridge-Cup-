/*************************************************
File name: 	 【PWM】PWM输出

Copyright (c) 电子设计工坊 dianshe.taobao.com
All rights reserved
*************************************************/
#include "system.h"

/*
  * @brief  系统初始化函数：关闭继电器、蜂鸣器
  * @param  None
  * @retval None
  * @author dianshe.taobao.com
  */
void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0); 		/* 关闭蜂鸣器、继电器*/
	vDevice_Ctrl(0x80,0xff); 	/* 关闭LED*/
}

//PWM输出函数，放在0.1ms中断里执行
u8 cnt_pwm=0;
u8 pwm_duty = 80;
u8 led_ctrl;
void vPWM_Prcocess()
{
	cnt_pwm = cnt_pwm % 10 + 1;	//cnt_pwm的范围是1~10，共10个数，即定义了PWM的周期为1ms
	if(cnt_pwm <= (pwm_duty/10))
	{
		led_ctrl=0x55;
		
		smg_buf[0]=smg_code[12345/10000];
		smg_buf[1]=smg_code[12345/1000%10];	
		smg_buf[2]=smg_code[12345/100%10];	
		smg_buf[3]=smg_code[12345/10%10];	
		smg_buf[4]=smg_code[12345%10];	
		smg_buf[5]=0x00;	
		smg_buf[6]=0x00;
		smg_buf[7]=0x00;
	}		
	else
	{
		led_ctrl=0xff;
		
		smg_buf[0]=0x00;
		smg_buf[1]=0x00;	
		smg_buf[2]=0x00;	
		smg_buf[3]=0x00;	
		smg_buf[4]=0x00;	
		smg_buf[5]=0x00;	
		smg_buf[6]=0x00;
		smg_buf[7]=0x00;
	}
}

u8 cnt_btn;
void vBTN_Proc()
{
	if(cnt_btn>=100)
	{
		cnt_btn=0;
		vBTN_Read_ThreeLine();
		if(Trg_BTN&0x08)
		{
			pwm_duty=(pwm_duty+10)%110;
		}
		if(Trg_BTN&0x04)
		{
			pwm_duty=(pwm_duty-10)%110;
		}		
	}
	
}

//数码管操作函数
void vSMG_Process()
{

}

void main(void)
{
	vSystem_Init();
	vTimer2_Init_100us();
	while(1)
	{
		vSMG_Process();
		vBTN_Proc();
	}
}


//中断服务程序
void vTimer2_ISR() interrupt 12     	//中断入口，仅操作实时性要求高的函数
{
	cnt_btn++;
	vSMG_Display();
	vPWM_Prcocess();
	vDevice_Ctrl(0x80,led_ctrl);
}
