#include <stdio.h>
#include <stdint.h>

#include "memorymap.hpp"

MemoryMap::MemoryMap()
{
	cartridge = new Cartridge();
}

MemoryMap::~MemoryMap()
{
	delete cartridge;
}

void MemoryMap::power_up(void)
{
	if (cartridge->is_loaded() == 0)
	{
		printf("Cartridge has not been inserted / loaded\n");
		return;
	}

	joy_direction = 0x0F;
	joy_button = 0x0F;

	io[JOYP] = 0xFF;
	io[TIMA] = 0x00;
	io[TMA] = 0x00;
	io[TAC] = 0x00;
	io[NR10] = 0x80;
	io[NR11] = 0xBF;
	io[NR12] = 0xF3;
	io[NR14] = 0xBF;
	io[NR21] = 0x3F;
	io[NR22] = 0x00;
	io[NR24] = 0xBF;
	io[NR30] = 0x7F;
	io[NR31] = 0xFF;
	io[NR32] = 0x9F;
	io[NR34] = 0xBF;
	io[NR41] = 0xFF;
	io[NR42] = 0x00;
	io[NR43] = 0x00;
	io[NR44] = 0xBF;
	io[NR50] = 0x77;
	io[NR51] = 0xF3;
	io[NR52] = 0xF1;

	if (cartridge->is_sgb())
	{
		io[NR52] = 0xF0;
	}

	io[LCDC] = 0x91;
	io[SCY] = 0x00;
	io[SCX] = 0x00;
	io[LYC] = 0x00;
	io[BGP] = 0xFC;
	io[OBP0] = 0xFF;
	io[OBP1] = 0xFF;
	io[WY] = 0x00;
	io[WX] = 0x00;
	io[HDMA5] = 0xFF;

	div_timer = 0x00;
	tima_timer = 0x00;

	current_vram_bank = 0x00;
	current_wram_bank = 0x01;

	source_address = 0x0000;
	destination_address = 0x0000;
}

uint8_t MemoryMap::read_byte(uint16_t address)
{
	uint8_t value = 0x00;

	/* do stuff to get value */

	return value;
}


void MemoryMap::write_byte(uint16_t address, uint8_t value)
{

}

Cartridge *MemoryMap::get_cartridge(void)
{
	return cartridge;
}