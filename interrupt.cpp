#include <stdint.h>

#include "interrupt.hpp"

struct interrupt_t interrupts[0x05] = {
	{ "V_BLANK",  0x01, 0x40 },
	{ "LCD_STAT", 0x02, 0x48 },
	{ "TIMER",    0x04, 0x50 },
	{ "SERIAL",   0x08, 0x58 },
	{ "JOYPAD",   0x10, 0x60 }
};

interrupt_t *get_interrupt_by_index(uint8_t index)
{
	if (index < 0 || index > 4)
	{
		return nullptr;
	}
	return &interrupts[index];
}