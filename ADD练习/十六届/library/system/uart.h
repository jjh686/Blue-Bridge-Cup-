#ifndef __UART_H_
#define __UART_H_

#include "system.h"

#define RXBUF 3

extern bit rx_flag;
extern u8 rx_buf[RXBUF];

void UartInit(void);
void SendData(u8 dat);
void SendString(char *s);
void vRXkx_time();

#endif
   