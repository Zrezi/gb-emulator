#ifndef LCDINTERRUPT_H
#define LCDINTERRUPT_H

#include <stdint.h>

#define LCD_INTERRUPT_HBLANK 0x00
#define LCD_INTERRUPT_VBLANK 0x01
#define LCD_INTERRUPT_OAM    0x02
#define LCD_INTERRUPT_LYC_LY 0x03

struct lcd_interrupt_t
{
	const char *name;
	uint8_t mask;
};

extern struct lcd_interrupt_t lcd_interrupts[0x04];

lcd_interrupt_t *get_lcd_interrupt_by_index(uint8_t index);

#endif