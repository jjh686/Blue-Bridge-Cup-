#ifndef __IIC_H_
#define __IIC_H_

#include "system.h"

void vWrite_EEPROM(u8 add,u8 dat);
u8 ucRead_EEPROM(u8 add);

#endif
