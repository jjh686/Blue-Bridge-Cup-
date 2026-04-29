#include "system.h"

void vSystem_init()
{
	vP2p0_ctrl(0x80,0xff);
	vP2p0_ctrl(0xa0,0);
	vP2p0_ctrl(0xc0,0);
}

u8 cnt_kbd;
u8 key_val;
u8 s12_number;
void vKBD_Process()
{
	if(cnt_kbd>=10)
	{
		cnt_kbd=0;
		key_val=uc_KBD_Read_State();
		if(key_val==12)
		{
			static u8 L1=0x01;
			L1=(((~L1)&0x01)|0xfe);
			s12_number++;
			vP2p0_ctrl(0x80,L1);
		}
		if(key_val==4)
		{
			s12_number=0;
		}
	}
}

u8 s12_times;
void vEEPROM_Process()
{
	s12_times=ucRead_EEPROM(0x25);
	vWrite_EEPROM(0x25,s12_number);
}

void vSmg_Process()
{
	smg_buf[0]=0x00;
	smg_buf[1]=0x00;
	smg_buf[2]=0x00;
	smg_buf[3]=0x00;
	smg_buf[4]=0x00;
	smg_buf[5]=0x00;
	smg_buf[6]=smg_code[s12_number/10%10];
	smg_buf[7]=smg_code[s12_number%10];	
}

void main()
{
	vSystem_init();
	vTimer2_Init();
	
	while(1)
	{
		vSmg_Process();
		vKBD_Process();
		vEEPROM_Process();
	}
	
}

void t2int() interrupt 12
{
	cnt_kbd++;
	vSmg_Disp();
}
