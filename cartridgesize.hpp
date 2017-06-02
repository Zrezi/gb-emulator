#ifndef CARTRIDGESIZE_H
#define CARTRIDGESIZE_H

#include <stdint.h>

struct cartridge_size_t {
	uint8_t index;
	const char *name;
	uint16_t number_of_banks;
};

extern struct cartridge_size_t cartridge_sizes[0x0B];

cartridge_size_t *get_cartridge_size_by_byte(uint8_t byte);

#endif