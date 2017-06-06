#include <stdint.h>

#include "lcdmode.hpp"

/*
	Probably from page 54:

	"Mode 0 is present between 201-207 clks, 2
	about 77-83 clks, and 3 about 169-175 clks. A
	complete cycle through these states takes 456
	clks. VBlank lasts 4560 clks. A complete screen
	refresh occurs every 70224 clks."

	Not sure yet why V_BLANK is set to 456
*/
const lcdmode_t lcdmodes[0x04] = {
	{ "H_BLANK",            204 },
	{ "V_BLANK",            456 },
	{ "OAM-RAM",            80  },
	{ "Data -> LCD Driver", 172 }
};

const lcdmode_t *get_lcd_mode_by_index(uint8_t index)
{
	if (index < 0 || index > 4)
	{
		return nullptr;
	}
	return &lcdmodes[index];
};