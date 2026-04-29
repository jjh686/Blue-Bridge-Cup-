#include "system.h"

u16 set_Pf=2000;
s16 set_Pj=0;
u8 hour,minute,second,hour1,minute1,second1;
s32 freq_ne555_jz;
s32 freq_ne555_jzmax=-900;

void vSystem_init()
{
	vP2P0_ctrl(0x80,0xff);
	vP2P0_ctrl(0xa0,0);
	vP2P0_ctrl(0xe0,0xff);
}

u8 kbd_cnt;
u8 key_val;
u8 set_disp_mode;
u8 set_disp_mode1;
u8 set_disp_mode3;
void vKbd_Proc()
{
	if(kbd_cnt>=10)
	{
		kbd_cnt=0;
		key_val=ucKBD_Read_State();
		if(key_val==4)
		{
			set_disp_mode=(set_disp_mode+1)%4;
			if(set_disp_mode==1)
				set_disp_mode1=0;
			if(set_disp_mode==3)
				set_disp_mode3=0;
		}
		if(key_val==5)
		{
			if(set_disp_mode==1)
			set_disp_mode1=(set_disp_mode1+1)%2;
			if(set_disp_mode==3)
			set_disp_mode3=(set_disp_mode3+1)%2;
		}
		if(key_val==8)
		{
			if(set_disp_mode==1&&set_disp_mode1==0)
				set_Pf=set_Pf+1000;
			if(set_disp_mode==1&&set_disp_mode1==1)
				set_Pj=set_Pj+100;
		}
		if(key_val==9)
		{
			if(set_disp_mode==1&&set_disp_mode1==0)
				set_Pf=set_Pf-1000;
			if(set_disp_mode==1&&set_disp_mode1==1)
				set_Pj=set_Pj-100;
		}		
	}
}

u8 dac_cnt;
u8 dac_val;
void vDac_Proc()
{
    if(dac_cnt >= 10)
    {
        dac_cnt = 0;
        if(freq_ne555_jz < 0) { // 错误状态：校准后为负 [cite: 94]
            dac_val = 0;
        } else if(freq_ne555_jz <= 500) {
            dac_val = 51;      // 1V
        } else if(freq_ne555_jz >= set_Pf) {
            dac_val = 255;     // 5V (0xff)
        } else {
            // 纯整数比例计算，速度提升几十倍，且不丢失精度
            dac_val = 51 + (u16)204 * (freq_ne555_jz - 500) / (set_Pf - 500); 
        }
        vWrite_Dac(dac_val);
    }
}

u16 ne555_cnt;
s32 freq_ne555;
void vNe555_Proc()
{
	ne555_cnt++;
	if(ne555_cnt>=1000)
	{
		ne555_cnt=0;
		freq_ne555=(TH0<<8)|TL0;
		TH0=0;
		TL0=0;
	}
}

void vDs1302_Proc()
{
	second=bcdtodec(Read_Ds1302_Byte(0x81));
	minute=bcdtodec(Read_Ds1302_Byte(0x83));
	hour=bcdtodec(Read_Ds1302_Byte(0x85));
}

void vPV_Proc()
{
	if(set_Pf<=1000)
		set_Pf=1000;
	else if(set_Pf>=9000)
		set_Pf=9000;
	if(set_Pj<=-900)
		set_Pj=-900;
	else if(set_Pj>=900)
		set_Pj=900;
	freq_ne555_jz=freq_ne555+set_Pj;
	if(freq_ne555_jz>freq_ne555_jzmax)
	{
		freq_ne555_jzmax=freq_ne555_jz;
		hour1=hour;
		minute1=minute;
		second1=second;
	}

}

u8 led_ctrl=0xff;
u8 led_cnt;
void vLed_Proc()
{
	static u8 blink_flag = 0; // 0和1交替，作为全局的闪烁节拍标志

	// 1. 统一产生 200 个计数周期的闪烁节拍 (解决 led_cnt 被重复清零的冲突)
	if(led_cnt >= 200)
	{
		led_cnt = 0;
		blink_flag = !blink_flag; // 每次时间到，标志位翻转 (0变1，1变0，解决 k和b 逻辑错误)
	}

	// 2. 处理 L1 (对应 0x01) 的逻辑
	if(set_disp_mode == 0)
	{
		if(blink_flag == 0)
			led_ctrl &= ~0x01; // 点亮
		else
			led_ctrl |= 0x01;  // 熄灭
	}
	else
	{
		led_ctrl |= 0x01;      // 其他模式下熄灭
	}

	// 3. 处理 L2 (对应 0x02) 的逻辑 (使用互斥结构，解决状态覆盖问题)
	if(freq_ne555_jz < 0)
	{
		led_ctrl &= ~0x02;     // 小于0时，常亮 (假设低电平点亮)
	}
	else if(freq_ne555_jz > set_Pf)
	{
		// 大于设定值时，跟随节拍闪烁
		if(blink_flag == 0)
			led_ctrl &= ~0x02; // 点亮
		else
			led_ctrl |= 0x02;  // 熄灭
	}
	else
	{
		led_ctrl |= 0x02;      // 正常范围内，熄灭
	}
    vP2P0_ctrl(0x80, led_ctrl);
}

void vSmg_Process()
{
	if(set_disp_mode==0)
	{
		if(freq_ne555_jz>=0)
		{
			smg_buf[0]=0x71;
			smg_buf[1]=0x00;
			smg_buf[2]=0x00;
			if(freq_ne555>=10000)
				smg_buf[3]=smg_code[freq_ne555/10000];
			else
				smg_buf[3]=0x00;
			if(freq_ne555>=1000)
				smg_buf[4]=smg_code[freq_ne555/1000%10];
			else
				smg_buf[4]=0x00;	
			if(freq_ne555>=100)
					smg_buf[5]=smg_code[freq_ne555/100%10];
			else
				smg_buf[5]=0x00;
				if(freq_ne555>=10)
				smg_buf[6]=smg_code[freq_ne555/10%10];
			else
				smg_buf[6]=0x00;	
			smg_buf[7]=smg_code[freq_ne555%10];			
		}
		else
		{
			smg_buf[0]=0x71;
			smg_buf[1]=0x00;
			smg_buf[2]=0x00;
			smg_buf[3]=0x00;
			smg_buf[4]=0x00;
			smg_buf[5]=0x00;
			smg_buf[6]=0x38;
			smg_buf[7]=0x38;
		}

	}
	if(set_disp_mode==1)
	{
		if(set_disp_mode1==0)
		{
			smg_buf[0]=0x73;
			smg_buf[1]=smg_code[1];
			smg_buf[2]=0x00;
			smg_buf[3]=0x00;
			smg_buf[4]=smg_code[set_Pf/1000];
			smg_buf[5]=smg_code[set_Pf/100%10];
			smg_buf[6]=smg_code[set_Pf/10%10];
			smg_buf[7]=smg_code[set_Pf%10];
		}
		if(set_disp_mode1==1)
		{
			smg_buf[0]=0x73;
			smg_buf[1]=smg_code[2];
			smg_buf[2]=0x00;
			smg_buf[3]=0x00;
			if(set_Pj<0)
			{
				smg_buf[4]=0x40;
				smg_buf[5]=smg_code[-set_Pj/100];
				smg_buf[6]=smg_code[-set_Pj/10%10];
				smg_buf[7]=smg_code[-set_Pj%10];				
			}
			else if(set_Pj>0)
			{
				smg_buf[4]=0x00;
				smg_buf[5]=smg_code[set_Pj/100];
				smg_buf[6]=smg_code[set_Pj/10%10];
				smg_buf[7]=smg_code[set_Pj%10];				
			}
			else
			{
				smg_buf[4]=0x00;
				smg_buf[5]=0x00;
				smg_buf[6]=0x00;
				smg_buf[7]=smg_code[0];					
			}
		}
	}
	if(set_disp_mode==2)
	{
		smg_buf[0]=smg_code[hour/10];
		smg_buf[1]=smg_code[hour%10];
		smg_buf[2]=0x40;
		smg_buf[3]=smg_code[minute/10];
		smg_buf[4]=smg_code[minute%10];
		smg_buf[5]=0x40;
		smg_buf[6]=smg_code[second/10];
		smg_buf[7]=smg_code[second%10];
	}
	if(set_disp_mode==3)
	{
		if(set_disp_mode3==0)
		{
			if(freq_ne555_jzmax<0)
			{
				smg_buf[0]=0x76;
				smg_buf[1]=0x71;
				smg_buf[2]=0x00;
				smg_buf[3]=0x00;
				smg_buf[4]=0x00;
				smg_buf[5]=0x00;
				smg_buf[6]=0x38;
				smg_buf[7]=0x38;				
			}
			else
			{
				smg_buf[0]=0x76;
				smg_buf[1]=0x71;
				smg_buf[2]=0x00;
			if(freq_ne555_jzmax>=10000)
				smg_buf[3]=smg_code[freq_ne555_jzmax/10000];
			else
				smg_buf[3]=0x00;
			if(freq_ne555_jzmax>=1000)
				smg_buf[4]=smg_code[freq_ne555_jzmax/1000%10];
			else
				smg_buf[4]=0x00;	
			if(freq_ne555_jzmax>=100)
					smg_buf[5]=smg_code[freq_ne555_jzmax/100%10];
			else
				smg_buf[5]=0x00;
				if(freq_ne555_jzmax>=10)
				smg_buf[6]=smg_code[freq_ne555_jzmax/10%10];
			else
				smg_buf[6]=0x00;	
			smg_buf[7]=smg_code[freq_ne555_jzmax%10];						
			}
		}
		if(set_disp_mode3==1)
		{
				smg_buf[0]=0x76;
				smg_buf[1]=0x77;			
				smg_buf[2]=smg_code[hour1/10];
				smg_buf[3]=smg_code[hour1%10];
				smg_buf[4]=smg_code[minute1/10];
				smg_buf[5]=smg_code[minute1%10];
				smg_buf[6]=smg_code[second1/10];
				smg_buf[7]=smg_code[second1%10];
		}
	}
}

void main()
{
	vSystem_init();
	Timer2Init();
	vCounter0_Init();
	vClock_Set(0,0,0);
	
	while(1)
	{
		vSmg_Process();
		vKbd_Proc();
		vDac_Proc();
		vDs1302_Proc();
		vPV_Proc();
	}
}

void vTimer2_ISR() interrupt 12
{
	dac_cnt++;
	kbd_cnt++;
	vNe555_Proc();
	vSmg_Disp();
	led_cnt++;
	vLed_Proc();
}
