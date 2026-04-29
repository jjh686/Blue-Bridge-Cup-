#include "system.h"

void system_init()
{
	P2P0_ctrl(0xa0,0x00);
	P2P0_ctrl(0x80,0xff);
	P2P0_ctrl(0xc0,0x00);
}

u8 cnt_adc;
u8 ch0,ch1,ch3;
u16 ch3_volt;
u8 ch3_0_99,ch3_1_5;
void adc_process()
{
	if(cnt_adc>=2)
	{
		cnt_adc=0;
		read_adc(0x41);
		ch1=read_adc(0x41);
		read_adc(0x43);
		ch3=read_adc(0x43);
		
		ch3_volt=ch3*100/51;
		ch3_0_99=ch3/2.56;
		ch3_1_5=ch3/51.1+1;
	}
}

u8 cnt_dac;
void dac_process()
{
	static u8 dac_val=0;
	if(cnt_dac>=10)
	{
		cnt_dac=0;
		write_dac(dac_val++);		
	}
}

void smg_process()
{
	smg_buf[0]=smg_code[ch3_volt/100]|0x80;
	smg_buf[1]=smg_code[ch3_volt/10%10];
	smg_buf[2]=smg_code[ch3_volt%10];
	smg_buf[3]=0x00; 
	smg_buf[4]=smg_code[ch3_0_99/10];
	smg_buf[5]=smg_code[ch3_0_99%10];
	smg_buf[6]=0x00;
	smg_buf[7]=smg_code[ch3_1_5];
}

void main()
{
	system_init();
	Timer2Init();
	
	while(1)
	{
		smg_process();
		adc_process();
		dac_process();
	}
}

void t2int() interrupt 12
{
	cnt_dac++;
	cnt_adc++;
	smg_display();
}
