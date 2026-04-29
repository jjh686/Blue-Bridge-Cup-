#include "system.h"

u8 buzzer_ctrl;

void system_init()
{
	P2P0_ctrl(0xa0,0);
	P2P0_ctrl(0x80,0xff);
}

u8 seg_disp_mode;
u8 cnt_kbd;
u8 key_val;
u8 keyio_val;
u8 seg_find_mode;
u8 hour,second,minute;
// 定义3组触发时间的存储变量
u8 hour1[3] = {0}, minute1[3] = {0}, second1[3] = {0};

u16 cnt_sonic;
u16 dist_sonic;
void vSonic_Process()
{
  	if(cnt_sonic>=100)
	{
		cnt_sonic=0;
		dist_sonic=uiDistance_Get();
	}

}

void kbd_proc()
{
	if(cnt_kbd>=10)
	{
		cnt_kbd=0;
		key_val=kbd_read_state();
		keyio_val=kbd_keyio_state();
		if(key_val==4)  // S4 切换界面
		{
			seg_disp_mode=(seg_disp_mode+1)%3;
		}
		if(key_val==5)  // S5 记录界面子界面切换
		{
			if(seg_disp_mode==2)
			{
				seg_find_mode=(seg_find_mode+1)%3;
			}
		}
        if(key_val == 8)  // S8：清空所有记录
        {
            if(seg_disp_mode == 2)  // 仅在记录界面生效
            {
                // 清空所有记录（数组元素全部置0）
				u8 i;
                for( i=0; i<3; i++)
                {
                    hour1[i] = 0;
                    minute1[i] = 0;
                    second1[i] = 0;
                }
            }
        }
		if(keyio_val==9)  // S9 消除报警状态
		{
			buzzer_ctrl=0;
			P2P0_ctrl(0xa0,buzzer_ctrl<<6);			
		}
		else
			{
			if(dist_sonic<30)  // 报警状态由buzzer_ctrl控制
			{
				buzzer_ctrl=1;
				P2P0_ctrl(0xa0,buzzer_ctrl<<6);			
			}
			else
			{
				buzzer_ctrl=0;
				P2P0_ctrl(0xa0,buzzer_ctrl<<6);			
			}
			}	
	}
}

void ds1302_process()
{
	second=bcdtodec(Read_Ds1302_Byte(0x81));
	minute=bcdtodec(Read_Ds1302_Byte(0x83));
	hour=bcdtodec(Read_Ds1302_Byte(0x85));
}

void vReti()
{
	static u8 trig_once = 0;  // 标记单次触发
	if((dist_sonic<30) && (trig_once==0))
	{
		// 新触发时间入队，旧记录后移
		hour1[2] = hour1[1]; minute1[2] = minute1[1]; second1[2] = second1[1];
		hour1[1] = hour1[0]; minute1[1] = minute1[0]; second1[1] = second1[0];
		hour1[0] = hour; minute1[0] = minute; second1[0] = second;
		trig_once = 1;
	}
	if(dist_sonic>=30)
	{
		trig_once = 0;
	}	
}

u8 led_ctrl=0xff;
void led_proc()
{
	if(seg_disp_mode==0)
		led_ctrl&=~0x01;
	else
		led_ctrl |=0x01;
	if(seg_disp_mode==1)
		led_ctrl &=~0x02;
	else
		led_ctrl|=0x02;
	if(seg_disp_mode==2)
		led_ctrl &=~0x04;
	else
		led_ctrl|=0x04;
	P2P0_ctrl(0x80,led_ctrl);
}

void smg_process()
{
	if(seg_disp_mode==0)  // 数据界面
	{
		smg_buf[0] = 0x38;  // 标识符“L”
		smg_buf[1] = 0x00;
		smg_buf[2] = 0x00;
		smg_buf[3] = 0x00;
		smg_buf[4] = 0x00;
		if(dist_sonic>=100)
			smg_buf[5] = smg_code[dist_sonic/100%10];			
		else
			smg_buf[5] = 0x00;
		if(dist_sonic>=10)
			smg_buf[6] = smg_code[dist_sonic/10%10];			
		else
			smg_buf[6] = 0x00;		
		smg_buf[7] = smg_code[dist_sonic%10];	
	}
	else if(seg_disp_mode==1)  // 时间界面
	{
		smg_buf[0] = smg_code[hour/10];
		smg_buf[1] = smg_code[hour%10];
		smg_buf[2] = 0x40;  // 间隔符“-”
		smg_buf[3] = smg_code[minute/10];
		smg_buf[4] = smg_code[minute%10];
		smg_buf[5] = 0x40;  // 间隔符“-”
		smg_buf[6] = smg_code[second/10];
		smg_buf[7] = smg_code[second%10];			
	}
	else if(seg_disp_mode==2)  // 记录界面
	{
		smg_buf[0] = 0x77;  // 标识符“A”
		smg_buf[1] = smg_code[seg_find_mode+1];  // 索引值1、2、3
		// 根据索引值显示对应触发时间，未记录则显示“-”
		if(hour1[seg_find_mode] == 0 && minute1[seg_find_mode] == 0 && second1[seg_find_mode] == 0)
		{
			smg_buf[2] = smg_buf[3] = smg_buf[4] = smg_buf[5] = smg_buf[6] = smg_buf[7] = 0x40;
		}
		else
		{
			smg_buf[2] = smg_code[hour1[seg_find_mode]/10];
			smg_buf[3] = smg_code[hour1[seg_find_mode]%10];
			smg_buf[4] = smg_code[minute1[seg_find_mode]/10];
			smg_buf[5] = smg_code[minute1[seg_find_mode]%10];
			smg_buf[6] = smg_code[second1[seg_find_mode]/10];
			smg_buf[7] = smg_code[second1[seg_find_mode]%10];
		}
	}
}

void main()
{
	system_init();
	Timer2Init();
	clock_set(23,59,50);
	
	while(1)
	{
		smg_process();
		kbd_proc();
		vSonic_Process();
		ds1302_process();
	}
}

void t2int() interrupt 12
{
	cnt_sonic++;
	cnt_kbd++;
	smg_disp();
	vReti();
	led_proc();
}
