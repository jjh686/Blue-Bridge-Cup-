#include "system.h"

void system_init()
{
	P2P0_ctrl(0xa0,0x00);
	P2P0_ctrl(0x80,0xff);
	P2P0_ctrl(0xc0,0x00);
}

union eeprom_u16
{
	u16 a;
	u8 b[2];
}u16_write,u16_read;

union eeprom_s16
{
	s16 a;
	s8 b[2];
}s16_write,s16_read;

union eeprom_float
{
	float a;
	u8 b[4];
}float_write,float_read;
//u8 start_times;
//u16 u16_write=1234;
//u16 u16_read=0;

//float float_write=3.1415;
//float float_read=0;

//s8 minus_write=-23;
//s8 minus_read=0;

//s16 minus_s16_write=-1234;
//s16 minus_s16_read=0;

//u8 str_write[11]={"hello world"};
//u8 str_read[11];

void eeprom_process()
{
//	u8 i;	
//	start_times=read_eeprom(0x20);
//	write_eeprom(0x20,++start_times);
//	write_eeprom(0x00,u16_write>>8);
//	write_eeprom(0x01,u16_write);
//	u16_read=(read_eeprom(0x00)<<8)+read_eeprom(0x01);
//	
//	write_eeprom(0x03,(u16)(float_write*1000)/256);
//	write_eeprom(0x04,(u16)(float_write*1000)%256);
//	float_read=(read_eeprom(0x03)*256+read_eeprom(0x04))/1000.0;
//	
//	write_eeprom(0x06,minus_write);
//	minus_read=read_eeprom(0x06);
//	
//	write_eeprom(0x08,minus_s16_write>>8);
//	write_eeprom(0x09,minus_s16_write);
//	minus_s16_read=(read_eeprom(0x08)<<8)+read_eeprom(0x09);

//	for(i=0;i<sizeof(str_write);i++)
//	{
//		write_eeprom(0x10+i,str_write[i]);
//	}
//	for(i=0;i<sizeof(str_write);i++)
//	{
//		str_read[i]=read_eeprom(0x10+i);
//	}	
	u8 i=0;
	
	u16_write.a=1234;
	for(i=0;i<sizeof(u16);i++)
	{
		write_eeprom(0x30+i,u16_write.b[i]);
	}
	for(i=0;i<sizeof(u16);i++)
	{
		u16_read.b[i]=read_eeprom(0x30+i);
	}	

	s16_write.a=-1234;
	for(i=0;i<sizeof(s16);i++)
	{
		write_eeprom(0x20+i,s16_write.b[i]);
	}
	for(i=0;i<sizeof(s16);i++)
	{
		s16_read.b[i]=read_eeprom(0x20+i);
	}
	float_write.a=3.1415926;
	for(i=0;i<sizeof(float);i++)
	{
		write_eeprom(0x10+i,float_write.b[i]);
	}
	for(i=0;i<sizeof(float);i++)
	{
		float_read.b[i]=read_eeprom(0x10+i);
	}
}

void smg_process()
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

void main()
{
	system_init();
	eeprom_process();
	
	while(1)
	{
		smg_process();
		smg_display();
	}
}
