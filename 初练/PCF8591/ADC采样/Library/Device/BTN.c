#include "BTN.h"

#define KEY_NO	 0
#define KEY_DOWN 1
#define KEY_UP   2

u8 vBTN_Read_State(void)
{
	static u8 key_state = 0;
	u8 key_io = 0, key_return = 0;
	
	key_io = P3 & 0x0f;
	switch(key_state)
	{
		case KEY_NO:
			if(key_io!=0x0f) key_state = KEY_DOWN;
		break;
		
		case KEY_DOWN:
			if(key_io!=0x0f)
			{
				if(key_io==0x0e) key_return = 7;
				if(key_io==0x0d) key_return = 6;
				if(key_io==0x0b) key_return = 5;
				if(key_io==0x07) key_return = 4;
				key_state = KEY_UP;
			}
			else
				key_state = KEY_NO;
		break;
			
		case KEY_UP:
			if(key_io==0x0f) key_state = KEY_NO;
		break;
	}
	return key_return;
}

unsigned char Trg;
unsigned char Cont;
void vBTN_Read_ThreeLine( void )
{
	unsigned char ReadData = P3^0xff;
	Trg = ReadData & (ReadData ^ Cont);
	Cont = ReadData;
}
