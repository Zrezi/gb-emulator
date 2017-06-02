#ifndef MEMORYMAP_H
#define MEMORYMAP_H

#include <stdint.h>

#include "cartridge.hpp"

#define JOYP 0x00
#define SB 0x01
#define SC 0x01
#define DIV 0x04
#define TIMA 0x05
#define TMA 0x06
#define TAC 0x07
#define IF 0x0F
#define NR10 0x10
#define NR11 0x11
#define NR12 0x12
#define NR13 0x13
#define NR14 0x14
#define NR21 0x16
#define NR22 0x17
#define NR23 0x18
#define NR24 0x19
#define NR30 0x1A
#define NR31 0x1B
#define NR32 0x1C
#define NR33 0x1D
#define NR34 0x1E
#define NR41 0x20
#define NR42 0x21
#define NR43 0x22
#define NR44 0x23
#define NR50 0x24
#define NR51 0x25
#define NR52 0x26
#define LCDC 0x40
#define STAT 0x41
#define SCY 0x42
#define SCX 0x43
#define LY 0x44
#define LYC 0x45
#define DMA 0x46
#define BGP 0x47
#define OBP0 0x48
#define OBP1 0x49
#define WY 0x4A
#define WX 0x4B

#define VBK 0x4F
#define HDMA1 0x51
#define HDMA2 0x52
#define HDMA3 0x53
#define HDMA4 0x54
#define HDMA5 0x55
#define BGPI 0x68
#define BGPD 0x69
#define OBPI 0x6A
#define OBPD 0x6B
#define SVBK 0x70

class MemoryMap
{
private:
	Cartridge *cartridge;

	uint8_t vram[2][0x2000];
	uint8_t wram[8][0x1000];
	uint8_t oam[0xA0];
	uint8_t io[0x80];
	uint8_t hram[0x7F];

	uint32_t div_timer;
	uint32_t tima_timer;

	uint8_t joy_direction;
	uint8_t joy_button;

	uint8_t current_vram_bank;
	uint8_t current_wram_bank;

	uint16_t source_address;
	uint16_t destination_address;
public:
	MemoryMap();
	~MemoryMap();

	void power_up(void);

	uint8_t read_byte(uint16_t address);
	void write_byte(uint16_t address, uint8_t value);

	Cartridge *get_cartridge(void);
};

#endif