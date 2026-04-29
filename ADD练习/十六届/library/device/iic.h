#ifndef __IIC_H_
#define __IIC_H_

#include "system.h"

u8 ucRead_ADC(u8 dat);
void vWrite_DAC(u8 dat);
void vAT24C02_Write(u8 add,u8 dat);
u8 ucAT24C02_Read(u8 add);

#endif
