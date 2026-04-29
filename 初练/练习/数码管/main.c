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
typedef struct
{
	unsigned char b0 : 1;
	unsigned char b1 : 1;
	unsigned char b2 : 1;
	unsigned char b3 : 1;
	unsigned char b4 : 1;
	unsigned char b5 : 1;
	unsigned char b6 : 1;
	unsigned char b7 : 1;
}bits;

typedef union
{
	unsigned char hex;
	bits b;
}HexToBin;

typedef char s8;
typedef unsigned char u8;
typedef int s16;
typedef unsigned int  u16;
typedef unsigned long u32;

//HexToBin led_ctrl,buzzer_ctrl;

//void vSystem_Init(void)
//{
//	vDevice_Ctrl(0xa0,0);		//关闭继电器与蜂鸣器	
//}

//void main(void)
//{
//	vSystem_Init();
//	led_ctrl.hex = 0x55;
//	vDevice_Ctrl(0x80,led_ctrl.hex);
//	
//	buzzer_ctrl.b.b4 = 1;
//	vDevice_Ctrl(0xa0,buzzer_ctrl.hex);
//	while(1)
//	{
//		led_ctrl.b.b1 = 1;
//		vDevice_Ctrl(0x80,led_ctrl.hex);
//		vDelay_Ms(200);
//		
//		led_ctrl.b.b1 = 0;
//		vDevice_Ctrl(0x80,led_ctrl.hex);
//		vDelay_Ms(200);
//	}
//}

////定时器(定时器2编程)
//HexToBin led_ctrl,buzzer_ctrl;
//u16 cnt_buzzer;

//void vSystem_Init(void)
//{
//	vDevice_Ctrl(0xa0,0);		//关闭继电器与蜂鸣器	
//}

//void vTimer2_Init(void)		//1毫秒@12.000MHz
//{
//	AUXR &= 0xFB;		//定时器时钟12T模式
//	T2L = 0x18;		//设置定时初值
//	T2H = 0xFC;		//设置定时初值
//	AUXR |= 0x10;		//定时器2开始计时
//	IE2 |= 0x04;        //开定时器2中断
//	EA = 1;    			//开启总中断
//}

//void main(void)
//{
//	vSystem_Init();
//	vTimer2_Init();

//	while(1)
//	{

//	}
//}

////中断服务程序
//void vTimer_ISR() interrupt 12           //中断入口
//{
//	//写法一：根据时间段
////	cnt_buzzer++;
////	if(cnt_buzzer<=200)
////	{
////		buzzer_ctrl.b.b6 = 1;
////		vDevice_Ctrl(0xa0,buzzer_ctrl.hex);
////	}
////	else if(cnt_buzzer<=400)
////	{
////		buzzer_ctrl.b.b6 = 0;
////		vDevice_Ctrl(0xa0,buzzer_ctrl.hex);		
////	}
////	else
////		cnt_buzzer=0;
//	//写法二：根据时间点
//	cnt_buzzer++;
//	if(cnt_buzzer>=200)
//	{
//		cnt_buzzer=0;
//		buzzer_ctrl.b.b6 = ~buzzer_ctrl.b.b6;
//		vDevice_Ctrl(0xa0,buzzer_ctrl.hex);		
//	}	
//}

////定时器(综合练习)
//HexToBin led_ctrl,uln_ctrl;

//void vSystem_Init(void)
//{
//	vDevice_Ctrl(0xa0,0);		//关闭继电器与蜂鸣器	
//	led_ctrl.hex=0xff;
//	vDevice_Ctrl(0x80,led_ctrl.hex);		//关闭LED
//}

//void vTimer2_Init(void)		//1毫秒@12.000MHz
//{
//	AUXR &= 0xFB;		//定时器时钟12T模式
//	T2L = 0x18;		//设置定时初值
//	T2H = 0xFC;		//设置定时初值
//	AUXR |= 0x10;		//定时器2开始计时
//	IE2 |= 0x04;        //开定时器2中断
//	EA = 1;    			//开启总中断
//}

////LED操做函数
//u16 cnt_led_blink,cnt_led_running;
//u8 led_blink_times,led_shift;
//void vLED_Process()
//{
//	if(cnt_led_blink>=500)
//	{
//		cnt_led_blink=0;
//		led_ctrl.b.b0 = ~led_ctrl.b.b0;
//		vDevice_Ctrl(0x80,led_ctrl.hex);
//		
//		led_blink_times++;
//	}
//	
//	if(cnt_led_running>=100)
//	{
//		cnt_led_running=0;
//		led_ctrl.b.b3 = ~(0x01>>led_shift);
//		led_ctrl.b.b4 = ~(0x02>>led_shift);
//		led_ctrl.b.b5 = ~(0x04>>led_shift);
//		led_ctrl.b.b6 = ~(0x08>>led_shift);
//		led_ctrl.b.b7 = ~(0x10>>led_shift);
//		vDevice_Ctrl(0x80,led_ctrl.hex);	

//		led_shift++;
//		if(led_shift==5)
//			led_shift=0;
//	}
//}

////继电器操做函数
//void vULN_Process()
//{
//	if(led_blink_times==10)
//	{
//		uln_ctrl.b.b4 = 1;
//		vDevice_Ctrl(0xa0,uln_ctrl.hex);
//	}
//	if(led_blink_times==20)
//	{
//		led_blink_times=0;
//		uln_ctrl.b.b4 = 0;
//		vDevice_Ctrl(0xa0,uln_ctrl.hex);
//	}
//}

//void main(void)
//{
//	vSystem_Init();
//	vTimer2_Init();

//	while(1)
//	{

//	}
//}

////中断服务程序
//void vTimer_ISR() interrupt 12           //中断入口
//{
//	cnt_led_blink++;
//	cnt_led_running++;
//	vLED_Process();
//	vULN_Process();

//}

////数码管(动态显示)
//u8 smg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00,0x3E,0x73};

//HexToBin led_ctrl,uln_ctrl;

//void vSystem_Init(void)
//{
//	vDevice_Ctrl(0xa0,0);		//关闭继电器与蜂鸣器	
//	led_ctrl.hex=0xff;
//	vDevice_Ctrl(0x80,led_ctrl.hex);		//关闭LED
//}

//void vTimer2_Init(void)		//1毫秒@12.000MHz
//{
//	AUXR &= 0xFB;		//定时器时钟12T模式
//	T2L = 0x18;		//设置定时初值
//	T2H = 0xFC;		//设置定时初值
//	AUXR |= 0x10;		//定时器2开始计时
//	IE2 |= 0x04;        //开定时器2中断
//	EA = 1;    			//开启总中断
//}

//void main(void)
//{
//	vSystem_Init();
//	vTimer2_Init();

//	while(1)
//	{

//	}
//}

//void vSMG_Display()
//{
//	static u8 i=0;
//	vDevice_Ctrl(0xe0,~smg_code[i]);
//	vDevice_Ctrl(0xc0,0x01<<i);
//	i = (i + 1) % 8;
//}

////中断服务程序
//void vTimer_ISR() interrupt 12           //中断入口
//{
//	vSMG_Display();
//}

//数码管(消除鬼影)
u8 smg_code[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x00,0x3E,0x73,0x76,0x79,0x38};

HexToBin led_ctrl,uln_ctrl;

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);		//关闭继电器与蜂鸣器	
	led_ctrl.hex=0xff;
	vDevice_Ctrl(0x80,led_ctrl.hex);		//关闭LED
}

void vTimer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;		//定时器时钟12T模式
	T2L = 0x18;		//设置定时初值
	T2H = 0xFC;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;        //开定时器2中断
	EA = 1;    			//开启总中断
}

u8 smg_buf[8];

void vSMG_Process()
{
	smg_buf[0]=smg_code[13];
	smg_buf[1]=smg_code[14];
	smg_buf[2]=smg_code[15];
	smg_buf[3]=smg_code[15];
	smg_buf[4]=smg_code[0];
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
	}
}

void vSMG_Display()
{
	static u8 i=0;
	vDevice_Ctrl(0xc0,0);
	vDevice_Ctrl(0xe0,~smg_buf[i]);
	vDevice_Ctrl(0xc0,0x01<<i);
	i = (i + 1) % 8;
}

//中断服务程序
void vTimer_ISR() interrupt 12           //中断入口
{
	vSMG_Display();
}
