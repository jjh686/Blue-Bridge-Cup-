#include "btn.h"

#define key_no 0
#define key_down 1
#define key_up 2

u8 btn_read()
{
	static u8 key_state=0;
	u8 key_io=0,key_val=0;
	key_io=P3&0x0f;
	switch(key_state)
	{
		case key_no:
			if(key_io!=0x0f)
				key_state=key_down;
			break;
		case key_down:
			if(key_io!=0x0f)
			{
				if(key_io==0x0e)
					key_val=7;
				if(key_io==0x0d)
					key_val=6;	
				if(key_io==0x0b)
					key_val=5;
				if(key_io==0x07)
					key_val=4;
				key_state=key_up;
			}
			else 
				key_state=key_no;
			break;
		case key_up:
			if(key_io==0x0f)
				key_state=key_no;
			break;
	}
	return key_val;
}

unsigned char trg;
unsigned char cont;
void btn_read_threeline()
{
    unsigned char read = P3^0xff;
    trg = read & (read ^ cont);
    cont = read;
}
