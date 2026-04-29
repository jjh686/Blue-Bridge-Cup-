#include "system.h"

// ================= 定义与变量 =================
// 状态定义
#define STATE_READY     0  // 准备界面
#define STATE_SETTING   1  // 设置界面
#define STATE_GAME      2  // 比赛界面
#define STATE_RESULT    3  // 结果界面

// 系统变量
u8 system_state = STATE_READY; // 当前系统状态
u8 set_time_param = 5;         // 设置的时间参数 (0-99)
s8 countdown_time = 0;         // 倒计时时间 (有符号，方便判断<0)
u8 eliminate_id = 0;           // 淘汰选手的编号 (0代表无淘汰/EE)
u8 light_val = 0;              // 光敏电阻值
bit is_dark = 0;               // 当前环境是否为暗

// 计时辅助变量
u16 cnt_1s_timer = 0;          // 1秒计时器
bit flag_100ms = 0;            // 100ms标志位
u8 cnt_btn = 0;                // 【修复】新增按键计数变量
u16 cnt_match_time = 0;        // 兼容你原来的变量定义(若有需要)

// 标志位
bit flag_all_released = 0;     // 结果界面下，是否所有人都已松手

// ================= 辅助函数 =================

// 将P3口的按键状态转换为选手编号
u8 Get_Eliminated_Player()
{
    u8 key_state = P3 & 0x0F; // 取P3低四位
    
    if(key_state == 0x00) return 0; // 都在按下状态
    
    // 优先级判断
    if(key_state & 0x01) return 7; // P30 高 -> S7松开
    if(key_state & 0x02) return 6; // P31 高 -> S6松开
    if(key_state & 0x04) return 5; // P32 高 -> S5松开
    if(key_state & 0x08) return 4; // P33 高 -> S4松开
    
    return 0;
}

// ================= 核心处理函数 =================

void vSystem_init()
{
    vP2P0_ctrl(0xa0, 0x00); // 关闭蜂鸣器继电器
    vP2P0_ctrl(0x80, 0xff); // 关闭LED
}

// 逻辑处理核心
void vLogic_Process()
{
    u8 key_val;
    u8 player_released;

    // --- 全局光敏检测逻辑 (带迟滞) ---
    // 迟滞：>115 确认为亮，<105 确认为暗
    if(light_val > 115) is_dark = 0; 
    else if(light_val < 105) is_dark = 1;

    // --- 状态机 ---
    switch(system_state)
    {
        case STATE_READY: // 准备界面
            // 1. 环境监测切换
            if(is_dark) {
                system_state = STATE_SETTING;
                break;
            }
            
            // 2. 检测4位选手是否全部按下 (P3低4位全为0)
            if((P3 & 0x0F) == 0x00) 
            {
                countdown_time = set_time_param;
                cnt_1s_timer = 0;
                eliminate_id = 0; // 重置淘汰人
                system_state = STATE_GAME;
            }
            break;

        case STATE_SETTING: // 设置界面
            // 1. 环境监测切换
            if(!is_dark) {
                system_state = STATE_READY;
                break;
            }

            // 2. 按键调整时间
            key_val = ucBTN_Read(); 
            if(key_val == 6) // S6
            {
                if(set_time_param > 0) set_time_param--;
            }
            if(key_val == 7) // S7
            {
                if(set_time_param < 99) set_time_param++;
            }
            break;

        case STATE_GAME: // 比赛界面
            // 1. 犯规检测 (松手检测)
            player_released = Get_Eliminated_Player();
            if(player_released != 0)
            {
                eliminate_id = player_released; // 记录是谁犯规
                system_state = STATE_RESULT;   // 立即结束
            }
            
            // 2. 倒计时逻辑 (在定时器中每1000ms减一次)
            if(countdown_time < 0) 
            {
                countdown_time = 0; 
                eliminate_id = 0;   // 无人淘汰 (EE)
                system_state = STATE_RESULT;
            }
            break;

        case STATE_RESULT: // 结果界面
            if(flag_all_released == 0)
            {
                // 检测是否全部松开
                if((P3 & 0x0F) == 0x0F)
                {
                    flag_all_released = 1;
                }
            }
            else
            {
                // 检测是否有任意按下
                if((P3 & 0x0F) != 0x0F)
                {
                    flag_all_released = 0; 
                    system_state = STATE_READY;
                }
            }
            break;
    }
}

// 显示处理
void vDisplay_Process()
{
    u8 led_data = 0xFF; 
    u8 i; 
    
    // 【关键步骤1】定义一个静态变量，用来记住“上一次”LED是什么样子的
    // static 意味着这个变量在函数退出后数值不会丢失
    static u8 led_data_old = 0xAA; // 给个初始值，确保第一次能刷新

    // --- LED 控制逻辑 (保持不变) ---
    switch(system_state)
    {
        case STATE_READY:   led_data &= ~0x01; break; // L1亮
        case STATE_SETTING: led_data &= ~0x02; break; // L2亮
        case STATE_GAME:    led_data &= ~0x04; break; // L3亮
        case STATE_RESULT:  led_data &= ~0x08; break; // L4亮
    }

    // 【关键步骤2】只有当新状态和旧状态不一样时，才操作硬件！
    if(led_data != led_data_old)
    {
        vP2P0_ctrl(0x80, led_data); // 操作硬件
        led_data_old = led_data;    // 更新旧状态，以此为基准
    }

    // --- 数码管显示 (保持不变) ---
    for(i=0; i<8; i++) smg_buf[i] = 0x00; 

    switch(system_state)
    {
        // ... (数码管部分的代码不用动) ...
        case STATE_READY:
            smg_buf[0] = 0x76; smg_buf[1] = 0x79; smg_buf[2] = 0x38; 
            smg_buf[3] = 0x38; smg_buf[4] = 0x3F; 
            break;
        case STATE_SETTING:
            smg_buf[0] = 0x3E; 
            if(set_time_param >= 10) smg_buf[6] = smg_code[set_time_param / 10];
            else smg_buf[6] = smg_code[0]; 
            smg_buf[7] = smg_code[set_time_param % 10];
            break;
        case STATE_GAME:
            smg_buf[0] = 0x77; 
            if(countdown_time >= 0) {
                if(countdown_time >= 10) smg_buf[6] = smg_code[countdown_time / 10];
                else smg_buf[6] = smg_code[0];
                smg_buf[7] = smg_code[countdown_time % 10];
            }
            break;
        case STATE_RESULT:
            smg_buf[0] = 0x79; 
            if(eliminate_id == 0) {
                smg_buf[6] = 0x79; smg_buf[7] = 0x79; 
            } else {
                smg_buf[6] = 0x00; smg_buf[7] = smg_code[eliminate_id]; 
            }
            break;
    }
}

void vADC_Process()
{
    if(flag_100ms)
    {
        flag_100ms = 0;
        light_val = ucADC_Read(0x41); 
    }
}

void main()
{
    vSystem_init();
    Timer2Init(); 
    EA = 1;       

    while(1)
    {
        vLogic_Process();   
        vDisplay_Process(); 
        vADC_Process();     
    }
}

// ================= 中断服务函数 =================

void t2_init() interrupt 12
{
    static u8 tick_10ms = 0;
    static u16 tick_1000ms = 0;
    
    // 数码管扫描
    vSmg_Display(); 
    
    // 按键计数器
    cnt_btn++; 
    
    // 100ms 计时
    tick_10ms++;
    if(tick_10ms >= 100) // 假设中断是1ms一次
    {
        tick_10ms = 0;
        flag_100ms = 1;
    }

    // 1000ms 倒计时逻辑
    if(system_state == STATE_GAME)
    {
        tick_1000ms++;
        if(tick_1000ms >= 1000)
        {
            tick_1000ms = 0;
            countdown_time--; 
        }
    }
    else
    {
        tick_1000ms = 0; 
    }
}