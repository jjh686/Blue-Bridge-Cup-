#ifndef __UART_H__
#define __UART_H__

#include "system.h"

#define RXBUF 10

extern u8 rx_buf[RXBUF];
extern bit rx_flag;

void Uart1_Init(void);
void SendData(u8 dat);
void SendString(char *s);
void vRX_time_Proc();

#endif
