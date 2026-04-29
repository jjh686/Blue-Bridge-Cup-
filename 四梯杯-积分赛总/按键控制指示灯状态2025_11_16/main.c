#include "system.h"

void vSystem_Init()
{
	vP2P0_Ctrl(0x80,0xff);
}

u8 cnt_kbd;
u8 key_val;
void vKbd_Process()
{
	if(cnt_kbd>=10)
	{
		cnt_kbd=0;
		key_val=kbd_keyio_state();
	}
}

u8 led_ctrl=0xff;
void vLED_process()
{
	if(key_val==4) led_ctrl&=~0x08;
	else led_ctrl|=0x08;
	if(key_val==5) led_ctrl&=~0x10;
	else led_ctrl|=0x10;
	if(key_val==6) led_ctrl&=~0x20;
	else led_ctrl|=0x20;
	if(key_val==7) led_ctrl&=~0x40;
	else led_ctrl|=0x40;
	if(key_val==8) led_ctrl&=~0x80;
	else led_ctrl|=0x80;
	if(key_val==9) led_ctrl&=0x7e;
	else led_ctrl|=0x81;
	if(key_val==10) led_ctrl&=0x7d;
	else led_ctrl|=0x82;	
	if(key_val==11) led_ctrl&=0x7a;
	else led_ctrl|=0x84;
	if(key_val==12) led_ctrl&=0x77;
	else led_ctrl|=0x88;
	if(key_val==13) led_ctrl&=0x6f;
	else led_ctrl|=0xa0;
	if(key_val==14) led_ctrl&=~(0xb0);
	else led_ctrl|=0xb0;
	if(key_val==15) led_ctrl&=~(0xd0);
	else led_ctrl|=0xd0;
	if(key_val==16) led_ctrl&=~(0xd1);
	else led_ctrl|=0xd1;
	if(key_val==17) led_ctrl&=~(0xd2);
	else led_ctrl|=0xd2;
	if(key_val==18) led_ctrl&=~(0xd4);
	else led_ctrl|=0xd4;	
	if(key_val==19) led_ctrl&=~(0xd8);
	else led_ctrl|=0xd8;
	vP2P0_Ctrl(0x80,led_ctrl);
	
}

void main()
{
	vSystem_Init();
	Timer2Init();
	
	while(1)
	{
		vKbd_Process();
	}
	
}

void t2int() interrupt 12
{
	cnt_kbd++;
	vLED_process();
}
