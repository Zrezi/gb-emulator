#include <stdint.h>

#include "memory.h"

uint8_t readByte(uint16_t address)
{
	if (address <= CART_END_ADDRESS)
	{
		return cart[address];
	}
	
	if (address >= VRAM_BEGIN_ADDRESS && address <= VRAM_END_ADDRESS)
	{
		return vram[address - VRAM_BEGIN_ADDRESS];
	}
	
	if (address >= SRAM_BEGIN_ADDRESS && address <= SRAM_END_ADDRESS)
	{
		return sram[address - SRAM_BEGIN_ADDRESS];
	}
	
	if (address >= WRAM_BEGIN_ADDRESS && address <= WRAM_END_ADDRESS)
	{
		return wram[address - WRAM_BEGIN_ADDRESS];
	}
	
	if (address >= WRAM_COPY_BEGIN_ADDRESS && address <= WRAM_COPY_END_ADDRESS)
	{
		return wram[address - WRAM_COPY_BEGIN_ADDRESS];
	}
	
	if (address >= OAM_BEGIN_ADDRESS && address <= OAM_END_ADDRESS)
	{
		return oam[address - OAM_BEGIN_ADDRESS];
	}
	
	if (address >= HRAM_BEGIN_ADDRESS && address <= HRAM_END_ADDRESS)
	{
		return hram[address - HRAM_BEGIN_ADDRESS];
	}
	
	if (address >= IO_BEGIN_ADDRESS && address <= IO_END_ADDRESS)
	{
		return io[address - IO_BEGIN_ADDRESS];
	}
	
	return 0;
}

uint16_t readShort(uint16_t address)
{
	// bitwise or a given location and the location directly after it, shifted left 8 bits
	return readByte(address) | (readByte(address + 1) << 8);
}

void writeByte(uint16_t address, uint8_t value)
{
	if (address >= VRAM_BEGIN_ADDRESS && address <= VRAM_END_ADDRESS)
	{
		vram[address - VRAM_BEGIN_ADDRESS] = value;
		/*
		Cinoop has a call to updateTile(address, value) which i assume is an OpenGL call
		*/
		return;
	}
	
	if (address >= SRAM_BEGIN_ADDRESS && address <= SRAM_END_ADDRESS)
	{
		sram[address - SRAM_BEGIN_ADDRESS] = value;
		return;
	}
	
	if (address >= WRAM_BEGIN_ADDRESS && address <= WRAM_END_ADDRESS)
	{
		wram[address - WRAM_BEGIN_ADDRESS] = value;
		return;
	}
	
	if (address >= WRAM_COPY_BEGIN_ADDRESS && address <= WRAM_COPY_END_ADDRESS)
	{
		wram[address - WRAM_COPY_BEGIN_ADDRESS] = value;
		return;
	}
	
	if (address >= OAM_BEGIN_ADDRESS && address <= OAM_END_ADDRESS)
	{
		oam[address - OAM_BEGIN_ADDRESS] = value;
		return;
	}
	
	if (address >= HRAM_BEGIN_ADDRESS && address <= HRAM_END_ADDRESS)
	{
		hram[address - HRAM_BEGIN_ADDRESS] = value;
		return;
	}
	
	if (address == 0xFF00)
	{
		// one of these blocks for every specific I/O call
		return;
	}
	else if (address >= IO_BEGIN_ADDRESS && address <= IO_END_ADDRESS)
	{
		io[address - IO_BEGIN_ADDRESS] = value;
		return;
	}
}

void writeShort(uint16_t address, uint16_t value)
{
	writeByte(address, (uint8_t) (value & 0x00FF));
	writeByte(address + 1, (uint8_t) ((value & 0xFF00) >> 8));
}

void copy(uint16_t destination, uint16_t source, size_t length)
{
	size_t i;
	for (i = 0; i < length; i++)
	{
		writeByte(destination + i, readByte(source + i));
	}
}