#include <STC15F2K60S2.H>
#include "intrins.h"

void vDelay_Ms(unsigned int ms)		//@12.000MHz
{
	unsigned char i, j;
	unsigned int k;

	for(k=0;k<ms;k++)
	{
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}

void vDelay_Us(unsigned int us)		//@12.000MHz
{
	unsigned int k;	
	
	for(k=0;k<us;k++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();		
	}
}

void vDelay_10us(unsigned int us)		//@12.000MHz
{
	unsigned char i;
	unsigned int k;

	for(k=0;k<us;k++)
	{
		_nop_();
		_nop_();
		i = 27;
		while (--i);		
	}
}

void vDelay_100us(unsigned int us)		//@12.000MHz
{
	unsigned char i, j;
	unsigned int k;

	for(k=0;k<us;k++)
	{
		i = 2;
		j = 39;
		do
		{
			while (--j);
		} while (--i);		
	}
}

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

void vDevice_Ctrl(unsigned char p2data,unsigned char p0data)
{
	P0 = p0data;
	P2 = p2data;
	P2 = 0;
}

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


