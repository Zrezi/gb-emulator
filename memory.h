#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define CART_BEGIN_ADDRESS 0x0000
#define CART_END_ADDRESS 0x7FFF

#define VRAM_BEGIN_ADDRESS 0x8000
#define VRAM_END_ADDRESS 0x9FFF

#define SRAM_BEGIN_ADDRESS 0xA000
#define SRAM_END_ADDRESS 0xBFFF

#define WRAM_BEGIN_ADDRESS 0xC000
#define WRAM_END_ADDRESS 0xDFFF

#define WRAM_COPY_BEGIN_ADDRESS 0xE000
#define WRAM_COPY_END_ADDRESS 0xFDFF

#define OAM_BEGIN_ADDRESS 0xFE00
// this might be wrong, keep an eye on this. might be 0xFEFF
#define OAM_END_ADDRESS 0xFE9F

#define IO_BEGIN_ADDRESS 0xFF00
#define IO_END_ADDRESS 0xFF7F

#define HRAM_BEGIN_ADDRESS 0xFF80
#define HRAM_END_ADDRESS 0xFFFE

/*
	0x8000 = 32768
	0x8000 uint8_t (1 byte) = 32768 B = 32 kB = cartridge size

	this includes the 16 kilobyte rom bank
	and the 16 kilobyte switchable rom bank

	memory location
	[0x0000] thru [0x7FFF]
*/
extern uint8_t cart[0x8000];

/*
	0x2000 = 8192
	vram (video ram) is 8 kB


	memory location
	[0x8000] thru [0x9FFF]
*/
extern uint8_t vram[0x2000];

/*
	again 0x2000 = 8192
	sram (switchable ram) is 8 kB

	"If present on the cartridge, this area is mapped to the RAM on the cartridge"

	memory location
	[0xA000] thru [0xBFFF]
*/
extern uint8_t sram[0x2000];

/*
	again, 0x2000 = 8192 = 8 kB
	wram (internal working ram) is 8 kB

	memory location
	[0xC000] thru [0xDFFF]
*/
extern uint8_t wram[0x2000];

/*
	"object attribute memory"
	
	everywhere I've read says this data should be 160 bytes long.
	holds sprite data, 40 total sprites * 4 bytes per sprite

	Byte 1: X location
	Byte 2: Y location
	Byte 3: tile number from character ram (which is in vram [0x8000] thru [0x97FF])
	Byte 4: attributes
	
	note that the (x, y) location is offset by (-8, -16), so to set sprite location at top left
	you would need to set byte 1 = 8 and byte 2 = 16
	
	memory location
	[0xFE00] thru [0xFE9F]
*/
extern uint8_t oam[0xA0];

/*
	"i/o registers"
	
	0x80 = 128
	should definitely be 0x80 because both the manual and the site I was using
	say that io space is [0xFF00] thru [0xFF7F] which is 0x80 bytes long
*/
extern uint8_t io[0x80];

/*
	"high ram" also called "zero page"
	apparently used as a quicker ram location, but isn't used?
	127 bytes
	
	memory location
	[0xFF80] thru [0xFFFE]
*/
extern uint8_t hram[0x7F];

/*
	not sure if this is needed, but added for completeness
	
	memory location
	[0xFFFF]
*/
extern uint8_t interrupt_enable;

uint8_t readByte(uint16_t address);
uint16_t readShort(uint16_t address);
void writeByte(uint16_t address, uint8_t value);
void writeShort(uint16_t address, uint16_t value);
void copy(uint16_t destination, uint16_t source, size_t length);

#endif