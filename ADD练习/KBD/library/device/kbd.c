#include "kbd.h"

u8 kbd_keyio()
{
	u8 keyio=0;
	P3=0xf0;
	P4=0xff;
	if(P44==0)
		keyio=4;
	if(P42==0)
		keyio=8;
	if(P35==0)
		keyio=12;
	if(P34==0)
		keyio=16;
	
	P3=0x0f;
	P4=0x00;
	if(P33==0)
		keyio+=0;
	if(P32==0)
		keyio+=1;
	if(P31==0)
		keyio+=2;
	if(P30==0)
		keyio+=3;
	return keyio;
}

#define key_no 0
#define key_down 1
#define key_up 2
u8 kbd_read()
{
	static u8 key_state=0;
	u8 key_io=0,key_val=0;
	
	key_io=kbd_keyio();
	switch(key_state)
	{
		case key_no:
			if(key_io)
				key_state=key_down;
			break;
		case key_down:
			if(key_io)
			{
				key_state=key_up;
				key_val=key_io;
			}
			else
				key_state=key_no;
			break;
		case key_up:
			if(key_io==0)
				key_state=key_no;
			break;
	}
	return key_val;
}

u8 kbd_keyio_threeline()
{
	u8 key_io =0xff;
	P3=0xf0;
	P4=0xff;
	if(P44==0) key_io=0x70;
	if(P42==0) key_io=0xb0;
	if(P35==0) key_io=0xd0;
	if(P34==0) key_io=0xe0;	
	
	P3=0x0f;
	P4=0x00;
	if(P33==0) key_io|=0x07;
	if(P32==0) key_io|=0x0b;
	if(P31==0) key_io|=0x0d;
	if(P30==0) key_io|=0x0e;	
	
	return key_io;
}

u8 trg_kbd;
u8 cont_kbd;
void kbd_read_threeline()
{
	u8 read=kbd_keyio_threeline()^0xff;
	trg_kbd=read&(read^cont_kbd);
	cont_kbd=read;
}
