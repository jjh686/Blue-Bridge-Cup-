#include "system.h"

void vSystem_Init()
{
	vP2P0_Ctrl(0x80,0xff);
	vP2P0_Ctrl(0xa0,0x00);
	vP2P0_Ctrl(0xe0,0xff);
}

u8 cnt_temp,read_temp;
void vDS18B20_Proc()
{
	if(cnt_temp>=100)
	{
		cnt_temp=0;
		read_temp=ucTemp_Read();
	}
}

u8 ch1,cnt_adc,ch1_0_5;
void vADC_Proc()
{
	if(cnt_adc>=10)
	{
		cnt_adc=0;
		ch1=ucRead_ADC(0x41);
		ch1_0_5=ch1/51;
	}
}

u8 cnt_sonic;
u16 dist_val;
void vSonic_Proc()
{
	if(cnt_sonic>=100)
	{
		cnt_sonic=0;
		dist_val=ucDistance_Get_PAC();
	}
}

u8 cnt_kbd,kbd_val,kbd_add,kbd_long;
void vKBD_Proc()
{
	if(cnt_kbd>=10)
	{
		cnt_kbd=0;
		kbd_val=ucKbd_Keyio_Read();
		kbd_long=ucKbd_Keyio_state();
		if(kbd_val)
			kbd_add++;
	}
}

u16 ne5_cnt,ne5_val;
void vNE555_Proc()
{
	ne5_cnt++;
	if(ne5_cnt>=1000)
	{
		ne5_cnt=0;
		ne5_val=(TH0<<8)|TL0;
		TH0=0;
		TL0=0;
	}
}

u8 cnt_dac;
void vDAC_Proc()
{
	static u8 dac_val=0;
	if(cnt_dac>=2)
	{
		cnt_dac=0;
		vWrite_DAC(dac_val++);
	}
}

u8 hour,minute,second;
void vDs1302_Proc()
{
	hour=bcdtodec(Read_Ds1302_Byte(0x85));
	minute=bcdtodec(Read_Ds1302_Byte(0x83));
	second=bcdtodec(Read_Ds1302_Byte(0x81));
}

u8 cnt_btn,key_val,s4;
void vBTN_Proc()
{
	if(cnt_btn>=10)
	{
		cnt_btn=0;
		key_val=vBTN_State_Read();
		if(key_val==4)
			s4++;
	}
	
}

u8 kj_val;
void vAT24C02_Proc()
{
	kj_val=ucAT24C02_Read(0x28);
	vAT24C02_Write(0x28,++kj_val);
}

void vUartRX_Proc()
{
	if(rx_flag==1)
	{
		rx_flag=0;
		vP2P0_Ctrl(0x80,rx_buf[0]);
		vP2P0_Ctrl(0xa0,rx_buf[1]);
	}
}

void vSmg_Proc()
{
	smg_buf[0]=smg_code[hour/10%10];
	smg_buf[1]=smg_code[hour%10];
	smg_buf[2]=smg_code[minute/10%10];
	smg_buf[3]=smg_code[minute%10];
	smg_buf[4]=smg_code[second/10%10];
	smg_buf[5]=smg_code[second%10];
	smg_buf[6]=smg_code[kj_val/10];
	smg_buf[7]=smg_code[kj_val%10];
}

void main()
{
	u8 uart_dat=1;
	vSystem_Init();
	Timer2Init();
	vCounter0_Init();
	vClock_init(23,55,11);
	vAT24C02_Proc();
	UartInit();
	SendString("Hhhhh\r\n");
	SendData(uart_dat+'0');
	
	while((u8)ucTemp_Read()==85) ;
	
	while(1)
	{
		vSmg_Proc();
		vDS18B20_Proc();
		vADC_Proc();
		vSonic_Proc();
		vKBD_Proc();
		vDAC_Proc();
		vDs1302_Proc();
		vBTN_Proc();
		vUartRX_Proc();
	}
	
}

void vTimer2_init() interrupt 12
{
	vSmg_Disp();
	cnt_temp++;
	cnt_adc++;
	cnt_sonic++;
	cnt_kbd++;
	vNE555_Proc();
	cnt_dac++;
	cnt_btn++;
	vRXkx_time();
}