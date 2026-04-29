#ifndef __KBD_H_
#define __KBD_H_

#include "system.h"

extern u8 trg_kbd;
extern u8 cont_kbd;

u8 kbd_read();
void kbd_read_threeline();

#endif
