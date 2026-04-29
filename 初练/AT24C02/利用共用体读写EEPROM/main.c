#include "system.h"

HexToBin led_ctrl,uln_ctrl;

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
	led_ctrl.hex=0xff;
	vDevice_Ctrl(0x80,led_ctrl.hex);	
}

union eeprom_u16
{
	u16 a;
	u8 b[2];
}u16_write,u16_read;

union eeprom_s16
{
	s16 a;
	u8 b[2];
}s16_write,s16_read;

union eeprom_float
{
	float a;
	u8 b[4];
}float_write,float_read;

union eeprom_u32
{
	u32 a;
	u8 b[4];
}u32_write;

void vEEPROM_Process()
{
	u8 i = 0;
	
	u16_write.a = 1234;
	for(i=0;i<sizeof(s16);i++)
	{
		vWrite_EEPROM(0x30+i,u16_write.b[i]);
	}
	for(i=0;i<sizeof(s16);i++)
	{
		u16_read.b[i]=ucRead_EEPROM(0x30+i);
	}

	s16_write.a = -1234;
	for(i=0;i<sizeof(s16);i++)
	{
		vWrite_EEPROM(0x20+i,s16_write.b[i]);
	}
	for(i=0;i<sizeof(s16);i++)
	{
		s16_read.b[i]=ucRead_EEPROM(0x20+i);
	}
	
	float_write.a = 3.1415926;
	for(i=0;i<sizeof(float);i++)
	{
		vWrite_EEPROM(0x10+i,float_write.b[i]);
	}
	for(i=0;i<sizeof(float);i++)
	{
		float_read.b[i]=ucRead_EEPROM(0x10+i);
	}	
}
void vSMG_Process()
{
	smg_buf[0]=0x00;
	smg_buf[1]=0x00;
	smg_buf[2]=0x00;
	smg_buf[3]=0x00;
	smg_buf[4]=0x00;
	smg_buf[5]=0x00;
	smg_buf[6]=0x00;
	smg_buf[7]=0x00;
}

void main(void)
{
	vSystem_Init();
	vEEPROM_Process();
	vTimer2_Init();
	while(1)
	{
		vSMG_Process();
	}
}

//中断服务程序
void vTimer2_ISR() interrupt 12     //中断入口
{
	vSMG_Display();
}
