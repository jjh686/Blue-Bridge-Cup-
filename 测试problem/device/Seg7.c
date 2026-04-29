#include "Seg7.h"
#include "P2P0.h"

/* 1=共阳极（段码取反），0=共阴极 */
#ifndef SEG_COMMON_ANODE
#define SEG_COMMON_ANODE 0
#endif

/* 0~9 段码：a 为 bit0 … g 为 bit6 */
static unsigned char code seg_tab[10] = {
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

/* 各位位选：P2 与 P2P0_Ctrl 配合；若实际不亮，请按原理图改这四字节 */
static unsigned char code digit_sel[4] = { 0x80, 0x40, 0x20, 0x10 };

static unsigned char seg_buf[4];

void Seg_Init(void)
{
	unsigned char i;
	for (i = 0; i < 4; i++)
		seg_buf[i] = 0;
}

void Seg_SetDigit(unsigned char pos, unsigned char num)
{
	if (pos < 4 && num < 10)
		seg_buf[pos] = num;
}

void Seg_SetNumber(unsigned int n)
{
	unsigned char i;
	if (n > 9999)
		n = 9999;
	for (i = 4; i > 0; i--)
	{
		seg_buf[i - 1] = (unsigned char)(n % 10);
		n /= 10;
	}
}

static void seg_delay(void)
{
	unsigned char i, j;
	for (i = 0; i < 8; i++)
		for (j = 0; j < 255; j++)
			;
}

void Seg_Scan(void)
{
	unsigned char i;
	for (i = 0; i < 4; i++)
	{
#if SEG_COMMON_ANODE
		P2P0_Ctrl(digit_sel[i], (unsigned char)(~seg_tab[seg_buf[i]]));
#else
		P2P0_Ctrl(digit_sel[i], seg_tab[seg_buf[i]]);
#endif
		seg_delay();
	}
}
