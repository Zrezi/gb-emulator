#include <stdint.h>

#include "lcdinterrupt.hpp"

struct lcd_interrupt_t lcd_interrupts[0x04] = {
	{ "H_BLANK", 0x08 },
	{ "V_BLANK", 0x10 },
	{ "OAM",     0x20 },
	{ "LYC_LY",  0x40 }
};

lcd_interrupt_t *get_lcd_interrupt_by_index(uint8_t index)
{
	if (index < 0 || index > 4)
	{
		return nullptr;
	}
	return &lcd_interrupts[index];
}