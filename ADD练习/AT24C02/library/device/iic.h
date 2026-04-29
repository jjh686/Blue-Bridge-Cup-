#ifndef _IIC_H
#define _IIC_H

#include "system.h"

void write_eeprom(u8 add,u8 dat);
u8 read_eeprom(u8 add);

#endif