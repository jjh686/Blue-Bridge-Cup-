#ifndef __SEG7_H_
#define __SEG7_H_

void Seg_Init(void);
void Seg_SetDigit(unsigned char pos, unsigned char num);
void Seg_SetNumber(unsigned int n);
void Seg_Scan(void);

#endif
