#ifndef CARTRIDGETYPE_H
#define CARTRIDGETYPE_H

#include <stdint.h>

enum base_type_t
{
	BASE_TYPE_ROM,
	BASE_TYPE_MBC1,
	BASE_TYPE_MMM01,
	BASE_TYPE_MBC2,
	BASE_TYPE_MBC3,
	BASE_TYPE_MBC4,
	BASE_TYPE_MBC5,
	BASE_TYPE_POCKET_CAMERA,
	BASE_TYPE_BANDAI_TAMA5,
	BASE_TYPE_HuC1,
	BASE_TYPE_HuC3
};

struct cartridge_type_t
{
	uint8_t index;
	const char *name;
	base_type_t base_type;
	uint8_t ram_used;
	uint8_t battery_used;
};

extern struct cartridge_type_t cartridge_types[0x1D];

cartridge_type_t *get_cartridge_type_by_byte(uint8_t byte);

#endif