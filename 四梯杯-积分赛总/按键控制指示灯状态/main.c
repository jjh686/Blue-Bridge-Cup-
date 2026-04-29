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
u16 times_s4,times_s5,times_s6,times_s7,times_s8,times_s9,times_s10,times_s11;
void vKBD_Process()
{
	if(cnt_kbd>=10)
	{
		cnt_kbd=0;
		key_val=ucKBD_KeyIO_State();
		if(key_val==4)
		{
			times_s4++;
		}
		else
			times_s4=0;
		if(key_val==5)
		{
			times_s5++;
		}
		else 
			times_s5=0;
		if(key_val==6)
		{
			times_s6++;
		}
		else 
			times_s6=0;
		if(key_val==7)
		{
			times_s7++;
		}
		else 
			times_s7=0;
		if(key_val==8)
		{
			times_s8++;
		}
		else 
			times_s8=0;
		if(key_val==9)
		{
			times_s9++;
		}
		else 
			times_s9=0;
		if(key_val==10)
		{
			times_s10++;
		}
		else 
			times_s10=0;
		if(key_val==11)
		{
			times_s11++;
		}
		else 
			times_s11=0;
	}
}

u8 cnt_L1,cnt_L2,cnt_L3,cnt_L4,cnt_L5,cnt_L6,cnt_L7,cnt_L8;
void vLED_Process()
{
	if(key_val==4)
	{
		static u8 L1=0x01;
		if(times_s4<=100)
		{
			L1=(((~L1)&0x01)|0xfe);		
			vP2p0_ctrl(0x80,L1);		
		}
		else if(times_s4>100)
		{
			while(cnt_L1>=100)
			{
				cnt_L1=0;
				L1=(((~L1)&0x01)|0xfe);
				vP2p0_ctrl(0x80,L1);					
			}
		}
	}
	if(key_val==5)
	{
		static u8 L=0x02;
		if(times_s5<=100)
		{
			L=(((~L)&0x02)|0xfd);		
			vP2p0_ctrl(0x80,L);		
		}
		else if(times_s5>100)
		{
			while(cnt_L2>=100)
			{
				cnt_L2=0;
				L=(((~L)&0x02)|0xfd);
				vP2p0_ctrl(0x80,L);					
			}
		}
	}
	if(key_val==6)
	{
		static u8 L=0x04;
		if(times_s6<=100)
		{
			L=(((~L)&0x04)|0xfb);		
			vP2p0_ctrl(0x80,L);		
		}
		else if(times_s6>100)
		{
			while(cnt_L3>=100)
			{
				cnt_L3=0;
				L=(((~L)&0x04)|0xfb);
				vP2p0_ctrl(0x80,L);					
			}
		}
	}
	if(key_val==7)
	{
		static u8 L=0x08;
		if(times_s7<=100)
		{
			L=(((~L)&0x08)|0xf7);		
			vP2p0_ctrl(0x80,L);		
		}
		else if(times_s7>100)
		{
			while(cnt_L4>=100)
			{
				cnt_L4=0;
				L=(((~L)&0x08)|0xf7);		
				vP2p0_ctrl(0x80,L);					
			}
		}
	}
	if(key_val==8)
	{
		static u8 L=0x10;
		if(times_s8<=100)
		{
			L=(((~L)&0x10)|0xef);		
			vP2p0_ctrl(0x80,L);		
		}
		else if(times_s8>100)
		{
			while(cnt_L5>=100)
			{
				cnt_L5=0;
				L=(((~L)&0x10)|0xef);		
				vP2p0_ctrl(0x80,L);					
			}
		}
	}
	if(key_val==9)
	{
		static u8 L=0x20;
		if(times_s9<=100)
		{
			L=(((~L)&0x20)|0xdf);		
			vP2p0_ctrl(0x80,L);		
		}
		else if(times_s9>100)
		{
			while(cnt_L6>=100)
			{
				cnt_L6=0;
				L=(((~L)&0x20)|0xdf);		
				vP2p0_ctrl(0x80,L);					
			}
		}
	}
	if(key_val==10)
	{
		static u8 L=0x40;
		if(times_s10<=100)
		{
			L=(((~L)&0x40)|0xbf);		
			vP2p0_ctrl(0x80,L);		
		}
		else if(times_s10>100)
		{
			while(cnt_L7>=100)
			{
				cnt_L7=0;
				L=(((~L)&0x40)|0xbf);		
				vP2p0_ctrl(0x80,L);					
			}
		}
	}
	if(key_val==11)
	{
		static u8 L=0x80;
		if(times_s11<=100)
		{
			L=(((~L)&0x80)|0x7f);		
			vP2p0_ctrl(0x80,L);		
		}
		else if(times_s11>100)
		{
			while(cnt_L8>=100)
			{
				cnt_L8=0;
				L=(((~L)&0x80)|0x7f);		
				vP2p0_ctrl(0x80,L);					
			}
		}
	}
}

void main()
{
	vSystem_init();
	vTimer2_Init();
	
	while(1)
	{
		vKBD_Process();
		vLED_Process();
	}
	
}

void t2int() interrupt 12
{
	cnt_kbd++;
	vLED_Process();
	cnt_L1++;
	cnt_L2++;
	cnt_L3++;
	cnt_L4++;
	cnt_L5++;
	cnt_L6++;
	cnt_L7++;
	cnt_L8++;
}


