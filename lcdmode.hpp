#ifndef LCDMODE_H
#define LCDMODE_H

#include <stdint.h>

struct lcdmode_t
{
	const char *name;
	uint16_t cycles;
};

extern const lcdmode_t lcdmodes[0x04];

const lcdmode_t *get_lcd_mode_by_index(uint8_t index);

#endif