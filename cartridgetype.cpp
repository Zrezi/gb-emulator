#include <stdint.h>

#include "cartridgetype.hpp"

struct cartridge_type_t cartridge_types[0x1D] = {
	{ 0x00,	"ROM ONLY",					BASE_TYPE_ROM,				0,	0	},
	{ 0x01,	"MBC1",						BASE_TYPE_MBC1,				0,	0	},
	{ 0x02,	"MBC1+RAM",					BASE_TYPE_MBC1,				1,	0	},
	{ 0x03,	"MBC1+RAM+BATTERY",			BASE_TYPE_MBC1,				1,	1	},
	{ 0x05,	"MBC2",						BASE_TYPE_MBC2,				0,	0	},
	{ 0x06,	"MBC2+BATTERY",				BASE_TYPE_MBC2,				0,	1	},
	{ 0x08,	"ROM+RAM",					BASE_TYPE_ROM,				1,	0	},
	{ 0x09,	"ROM+RAM+BATTERY",			BASE_TYPE_ROM,				1,	1	},
	{ 0x0B,	"MMM01",					BASE_TYPE_MMM01,			0,	0	},
	{ 0x0C,	"MMM01+RAM",				BASE_TYPE_MMM01,			1,	0	},
	{ 0x0D,	"MMM01+RAM+BATTERY",		BASE_TYPE_MMM01,			1,	1	},
	{ 0x0F,	"MBC3+TIMER+BATTERY",		BASE_TYPE_MBC3,				0,	1	},
	{ 0x10,	"MBC3+TIMER+RAM+BATTERY",	BASE_TYPE_MBC3,				1,	1	},
	{ 0x11,	"MBC3",						BASE_TYPE_MBC3,				0,	0	},
	{ 0x12,	"MBC3+RAM",					BASE_TYPE_MBC3,				1,	0	},
	{ 0x13,	"MBC3+RAM+BATTERY",			BASE_TYPE_MBC3,				1,	1	},
	{ 0x15,	"MBC4",						BASE_TYPE_MBC4,				0,	0	},
	{ 0x16,	"MBC4+RAM",					BASE_TYPE_MBC4,				1,	0	},
	{ 0x17,	"MBC4+RAM+BATTERY",			BASE_TYPE_MBC4,				1,	1	},
	{ 0x19,	"MBC5",						BASE_TYPE_MBC5,				0,	0	},
	{ 0x1A,	"MBC5+RAM",					BASE_TYPE_MBC5,				1,	0	},
	{ 0x1B,	"MBC5+RAM+BATTERY",			BASE_TYPE_MBC5,				1,	1	},
	{ 0x1C,	"MBC5+RUMBLE",				BASE_TYPE_MBC5,				0,	0	},
	{ 0x1D,	"MBC5+RUMBLE+RAM",			BASE_TYPE_MBC5,				1,	0	},
	{ 0x1E,	"MBC5+RUMBLE+RAM+BATTERY",	BASE_TYPE_MBC5,				1,	1	},
	{ 0xFC,	"POCKETCAMERA",				BASE_TYPE_POCKET_CAMERA,	0,	0	},
	{ 0xFD,	"BANDAITAMA5",				BASE_TYPE_BANDAI_TAMA5,		0,	0	},
	{ 0xFE,	"HuC3",						BASE_TYPE_HuC3,				0,	0	},
	{ 0xFF,	"HuC1+RAM+BATTERY",			BASE_TYPE_HuC1,				1,	1	}
};

cartridge_type_t *get_cartridge_type_by_byte(uint8_t byte)
{
	cartridge_type_t *val;
	uint8_t i;
	for (i = 0; i < sizeof(cartridge_types) / sizeof(cartridge_type_t); i++)
	{
		val = &cartridge_types[i];
		if (byte == val->index)
		{
			return val;
		}
	}
	return nullptr;
}