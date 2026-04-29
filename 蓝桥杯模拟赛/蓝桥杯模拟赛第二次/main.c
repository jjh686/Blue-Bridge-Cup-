#include "system.h"

// ================= 变量定义 =================
u8 set_disp_mode = 0;   // 0:检测界面, 1:阈值界面
u8 adc_mode = 0;        // 0:温度设置, 1:光照设置
u8 key_val;             // 接收键码
u8 ch3;                 // ADC原始值
u16 lux_val;            // 计算出的光照度
u16 temprt;             // 温度值(x100)

// 初始阈值
u8 temp_param = 30;     
u16 lux_param = 100;    

// ================= 系统初始化 =================
void vSystem_init()
{
    vP2P0_ctrl(0xa0, 0);      // 关闭蜂鸣器
    vP2P0_ctrl(0x80, 0xff);   // 关闭LED
    vP2P0_ctrl(0xe0, 0xff);   // 关闭数码管
}

// ================= 按键处理 =================
u8 cnt_kbd;
void kbd_process()
{
    if(cnt_kbd >= 10)
    {
        cnt_kbd = 0;
        // 调用你提供的状态机函数
        // 修复后的 kbd.c 会在按下瞬间返回一次键码，其余时间返回0
        key_val = kbd_read_state(); 

        // --- S4: 界面切换 ---
        if(key_val == 4)
        {
            set_disp_mode = (set_disp_mode + 1) % 2;
            
            // 每次进入阈值界面，重置为温度设置项
            if(set_disp_mode == 1)
            {
                adc_mode = 0; 
            }
        }
        
        // --- 阈值界面下的操作 ---
        if(set_disp_mode == 1)
        {
            // S5: 切换设置项
            if(key_val == 5)
            {
                adc_mode = (adc_mode + 1) % 2;
            }
            
            // S8: 加 (+)
            if(key_val == 8)
            {
                if(adc_mode == 0) // 温度
                {
                    temp_param += 5;
                    if(temp_param > 50) temp_param = 50; 
                }
                else // 光照
                {
                    lux_param += 20;
                    if(lux_param > 300) lux_param = 300; 
                }
            }
            
            // S9: 减 (-)
            if(key_val == 9)
            {
                if(adc_mode == 0) // 温度
                {
                    if(temp_param >= 15)
                         temp_param -= 5;
                    else
                         temp_param = 10;
                }
                else // 光照
                {
                    if(lux_param >= 120)
                        lux_param -= 20;
                    else
                        lux_param = 100;
                }
            }
        }
    }
}

// ================= 修复闪烁后的 LED 处理函数 =================
void vLed_process()
{
    u8 led_bits = 0xFF; // 默认全灭 (1为灭，0为亮)
    static u8 led_old = 0xFF; // [新增] 用于记录上一次的 LED 状态
    
    // 1. 界面指示灯
    if(set_disp_mode == 0)
    {
        led_bits &= ~0x01; // L1亮 (检测界面)
    }
    else
    {
        led_bits &= ~0x02; // L2亮 (阈值界面)
    }
    
    // 2. 报警指示灯 L8
    // 条件：(当前温度 > 阈值) 且 (当前光照 > 阈值)
    if( ((temprt / 100) > temp_param) && (lux_val > lux_param) )
    {
        led_bits &= ~0x80; // L8亮
    }
    
    // 3. [核心修改] 只有当 LED 状态发生改变时，才操作硬件
    // 这样避免了主循环一直“轰炸”P0口，解决了与数码管扫描的冲突
    if(led_bits != led_old)
    {
        vP2P0_ctrl(0x80, led_bits);
        led_old = led_bits; // 更新旧状态
    }
}

// ================= ADC与光照计算 =================
u8 cnt_adc;
void adc_process()
{
    float voltage;

    if(cnt_adc >= 10)
    {
        cnt_adc = 0;
        ch3 = read_adc(0x43); // 读取电位器
        voltage = (float)ch3 * 5.0 / 255.0;

        // 计算 Lux
        if(voltage < 0.3) lux_val = 10;
        else 
        {
            lux_val = (u16)((voltage - 0.3) * 104.256 + 10);
            if(lux_val > 500) lux_val = 500;
        }
    }
}

// ================= 温度读取 =================
u8 cnt_temp;
void ds18b20_read()
{
    if(cnt_temp >= 100)
    {
        cnt_temp = 0;
        temprt = (u16)(rd_temprature() * 100);
    }
}

// ================= 数码管显示 =================
void vSmg_process()
{
    if(set_disp_mode == 0) // 检测界面
    {
        smg_buf[0] = 0x39; // C
        smg_buf[1] = smg_code[(temprt / 100) / 10];
        smg_buf[2] = smg_code[(temprt / 100) % 10];
        smg_buf[3] = 0x00;
        smg_buf[4] = 0x76; // H
        
        // 光照高位熄灭
        if(lux_val >= 100) smg_buf[5] = smg_code[lux_val / 100];
        else smg_buf[5] = 0x00;

        if(lux_val >= 10) smg_buf[6] = smg_code[lux_val / 10 % 10];
        else smg_buf[6] = 0x00;

        smg_buf[7] = smg_code[lux_val % 10];    
    }
    else // 阈值界面
    {
        if(adc_mode == 0) // 温度阈值
        {
            smg_buf[0] = 0x73; // P
            smg_buf[1] = 0x39; // C
            smg_buf[2] = 0x00; smg_buf[3] = 0x00; smg_buf[4] = 0x00; smg_buf[5] = 0x00;
            smg_buf[6] = smg_code[temp_param / 10];
            smg_buf[7] = smg_code[temp_param % 10];           
        }
        else // 光照阈值
        {
            smg_buf[0] = 0x73; // P
            smg_buf[1] = 0x76; // H
            smg_buf[2] = 0x00; smg_buf[3] = 0x00; smg_buf[4] = 0x00;
            
            if(lux_param >= 100) smg_buf[5] = smg_code[lux_param / 100];
            else smg_buf[5] = 0x00;
            
            if(lux_param >= 10) smg_buf[6] = smg_code[lux_param / 10 % 10];
            else smg_buf[6] = 0x00;
            
            smg_buf[7] = smg_code[lux_param % 10];            
        }   
    }   
}

// ================= 主函数 =================
void main()
{
    vSystem_init();
    Timer2Init();
    
    // 初始值
    temp_param = 30;
    lux_param = 100;
    
    while(1)
    {
        ds18b20_read();
        adc_process();
        kbd_process();
        vSmg_process();
    }
}

void t2int() interrupt 12
{
    vSmg_Disp();
    cnt_kbd++;
    cnt_temp++;
    cnt_adc++;
	vLed_process();
}