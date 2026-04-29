#include "system.h"

HexToBin led_ctrl,uln_ctrl;

void vSystem_Init(void)
{
	vDevice_Ctrl(0xa0,0);
	led_ctrl.hex=0xff;
	vDevice_Ctrl(0x80,led_ctrl.hex);	
}

u16 u16_write = 1234;
u16 u16_read = 0;

float float_write = 3.1415;
float float_read = 0;

s8 minus_write = -23;
s8 minus_read = 0;

s16 minus_s16_write = -1234;
s16 minus_s16_read = 0;

u8 str_write[11]={"hello world"};
u8 str_read[11];
void vEEPROM_Process()
{
	u8 i = 0;
	
	vWrite_EEPROM(0x00,u16_write>>8);
	vWrite_EEPROM(0x01,u16_write);
	u16_read = (ucRead_EEPROM(0x00)<<8)+ucRead_EEPROM(0x01);
	
	vWrite_EEPROM(0x03,(u16)(float_write*1000)/256);
	vWrite_EEPROM(0x04,(u16)(float_write*1000)%256);
	float_read = (ucRead_EEPROM(0x03)*256+ucRead_EEPROM(0x04))/1000.0f;
	
//	if(minus_write<0)
//	{
//		vWrite_EEPROM(0x06,'-');
//		vWrite_EEPROM(0x07,-minus_write);
//	}
//	if(ucRead_EEPROM(0x06=='-')
//	{
//		minus_read = 0 - ucRead_EEPROM(0x07);
//	}
	
	vWrite_EEPROM(0x06,minus_write);
	minus_read = ucRead_EEPROM(0x06);
	
	vWrite_EEPROM(0x08,minus_s16_write>>8);
	vWrite_EEPROM(0x09,minus_s16_write);
	minus_s16_read = (ucRead_EEPROM(0x08)<<8)+ucRead_EEPROM(0x09);
	
	for(i=0;i<sizeof(str_write);i++)
	{
		vWrite_EEPROM(0x10+i,str_write[i]);
	}
	for(i=0;i<sizeof(str_write);i++)
	{
		str_read[i]=ucRead_EEPROM(0x10+i);
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
