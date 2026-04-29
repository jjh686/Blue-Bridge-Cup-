#ifndef __BTN_H_
#define __BTN_H_

#include "system.h"

extern unsigned char trg;
extern unsigned char cont;

void btn_read_threeline();
u8 btn_read();
#endif
