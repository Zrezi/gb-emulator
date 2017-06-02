#include <stdint.h>

#include "cartridgesize.hpp"

struct cartridge_size_t cartridge_sizes[0x0B] = {
	{ 0x00, "32 kilobyte (no ROM banking)",                         2   },
	{ 0x01, "64 kilobyte (4 banks)",                                4   },
	{ 0x02, "128 kilobyte (8 banks)",                               8   },
	{ 0x03, "256 kilobyte (16 banks)",                              16  },
	{ 0x04, "512 kilobyte (32 banks)",                              32  },
	{ 0x05, "1 megabyte (64 banks) - only 63 banks used by MBC1",   64  },
	{ 0x06, "2 megabyte (128 banks) - only 125 banks used by MBC1", 128 },
	{ 0x07, "4 megabyte (256 banks)",                               256 },
	{ 0x52, "1.1 megabyte (72 banks)",                              72  },
	{ 0x53, "1.2 megabyte (80 banks)",                              80  },
	{ 0x54, "1.5 megabyte (96 banks)",                              96  }
};

cartridge_size_t *get_cartridge_size_by_byte(uint8_t byte)
{
	cartridge_size_t *val;
	uint16_t i;
	for (i = 0; i < sizeof(cartridge_sizes) / sizeof(cartridge_size_t); i++)
	{
		val = &cartridge_sizes[i];
		if (byte == val->index)
		{
			return val;
		}
	}
	return nullptr;
}