#include "system.h"
#include "Delay.h" 
#include "intrins.h"
#include "Device.h"

//Delay软件延时
//void main(void)
//{
//	
//	
//	while(1)
//	{
//		P34 = 1;
//		vDelay_Ms(1);
//		vDelay_100us(5);
//		P34 = 0;
//		vDelay_100us(5);
//	}
//}

//继电器与蜂鸣器程序控制（位控制）
//sbit bRelay = P0^4;				//继电器
//sbit bBuzzer = P0^6;			//蜂鸣器

//void vSystem_Init(void)
//{
//	bRelay=1;					//开
//	bBuzzer=0;					//关
//	P27=1;P26=0;P25=1;			//选通译码器，开锁存器
//	P27=0;P26=0;P25=0;			//选通译码器，关锁存器
//}

//void main(void)
//{
//	vSystem_Init();
//	while(1)
//	{
//		
//	}
//}

//继电器与蜂鸣器程序控制（字节控制）
//void vSystem_Init(void)
//{
////	P0=0x01<<6;		//P06输出高电平，P04输出低电平
//	P0=0x40;		//P06输出高电平，P04输出低电平
//	P2=0xa0;		//打开锁存器
//	P2=0;			//关闭锁存器
//}

//void main(void)
//{
//	vSystem_Init();
//	while(1)
//	{
//		
//	}
//}

//继电器与蜂鸣器程序控制（标志位控制）
//unsigned char buzzer_ctrl;
//unsigned char relay_ctrl;

//void vSystem_Init(void)
//{
//	buzzer_ctrl=0;		
//	relay_ctrl=1;
//	P0=(buzzer_ctrl<<6)|(relay_ctrl<<4);
//	P2=0xa0;		//打开锁存器
//	P2=0;			//关闭锁存器
//}

//void main(void)
//{
//	vSystem_Init();
//	while(1)
//	{
//		
//	}
//}

//继电器与蜂鸣器程序控制（间接开关）	
//unsigned char buzzer_ctrl;
//unsigned char relay_ctrl;

//void vSystem_Init(void)
//{
//	buzzer_ctrl=0;		
//	relay_ctrl=1;
//	vDevice_Ctrl(0xa0,(buzzer_ctrl<<6)|(relay_ctrl<<4));
//}

//void main(void)
//{
//	vSystem_Init();
//	while(1)
//	{
//		buzzer_ctrl=1;
//		vDevice_Ctrl(0xa0,(buzzer_ctrl<<6)|(relay_ctrl<<4));
//		vDelay_Ms(500);
//		buzzer_ctrl=0;
//		vDevice_Ctrl(0xa0,(buzzer_ctrl<<6)|(relay_ctrl<<4));
//		vDelay_Ms(500);		
//	}
//}

//LED（点亮指定LED）	
//void vSystem_Init(void)
//{
//	vDevice_Ctrl(0xa0,0);		//关闭继电器与蜂鸣器	
//}

//void main(void)
//{
//	vSystem_Init();
//	vDevice_Ctrl(0x80,0x3a);		//点亮L1,L3,L7,L8
//	vDevice_Ctrl(0x80,0xaa);		//点亮L1,L3,L5,L7
//	while(1)
//	{
//			
//	}
//}

//LED（变量位操作）	
//unsigned char led_ctrl[9];

//void vSystem_Init(void)
//{
//	vDevice_Ctrl(0xa0,0);		//关闭继电器与蜂鸣器	
//}

//void main(void)
//{
//	vSystem_Init();
//	led_ctrl[1]=1;led_ctrl[3]=1;led_ctrl[7]=1;led_ctrl[8]=1;
//	vDevice_Ctrl(0x80,~(led_ctrl[8]<<7 | led_ctrl[7]<<6 | led_ctrl[6]<<5 | led_ctrl[5]<<4 | led_ctrl[4]<<3 | led_ctrl[3]<<2 | led_ctrl[2]<<1 | led_ctrl[1]));
//	while(1)
//	{
//			
//	}
//}

//LED（流水灯）	
//unsigned char led_ctrl[9];

//void vSystem_Init(void)
//{
//	vDevice_Ctrl(0xa0,0);		//关闭继电器与蜂鸣器	
//}

//void vLED_Running_L1_L8(void)
//{
//	static unsigned char led_shift=0;
//	vDevice_Ctrl(0x80,~(0x01<<led_shift));
//	led_shift++;
//	if(led_shift==8)
//		led_shift=0;
//}

//void vLED_Running_L8_L1(void)
//{
//	static unsigned char led_shift;
//	vDevice_Ctrl(0x80,~(0x80>>led_shift));
//	led_shift = (led_shift + 1) % 8;
//}

//void vLED_Running_L2_L4_L6_L8(void)
//{
//	static unsigned char led_shift;
//	vDevice_Ctrl(0x80,~(0x02<<led_shift));
//	led_shift = (led_shift + 2) % 8;
//}


//void main(void)
//{
//	vSystem_Init();
//	led_ctrl[1]=1;led_ctrl[3]=1;led_ctrl[7]=1;led_ctrl[8]=1;
//	vDevice_Ctrl(0x80,~(led_ctrl[8]<<7 | led_ctrl[7]<<6 | led_ctrl[6]<<5 | led_ctrl[5]<<4 | led_ctrl[4]<<3 | led_ctrl[3]<<2 | led_ctrl[2]<<1 | led_ctrl[1]));
//	while(1)
//	{
//		vLED_Running_L2_L4_L6_L8();
//		vDelay_Ms(200);
//	}
//}

//LED(利用共用体实现字节赋值和位赋值)

HexToBin led_ctrl,buzzer_ctrl;

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);		//关闭继电器与蜂鸣器	
}

void main(void)
{
	vSystem_Init();
	led_ctrl.hex = 0x55;
	vDevice_Ctrl(0x80,led_ctrl.hex);
	
	buzzer_ctrl.b.b4 = 1;
	vDevice_Ctrl(0xa0,buzzer_ctrl.hex);
	while(1)
	{
		led_ctrl.b.b1 = 1;
		vDevice_Ctrl(0x80,led_ctrl.hex);
		vDelay_Ms(200);
		
		led_ctrl.b.b1 = 0;
		vDevice_Ctrl(0x80,led_ctrl.hex);
		vDelay_Ms(200);
	}
}
