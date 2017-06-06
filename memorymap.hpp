#ifndef MEMORYMAP_H
#define MEMORYMAP_H

#include <stdint.h>

#include "sprite.hpp"
#include "cartridge.hpp"
#include "interrupt.hpp"
#include "lcdinterrupt.hpp"

#define DIV_RATE    0x100 // 256
#define MAX_SPRITES 0x28  // 40

#define JOYP  0x00
#define SB    0x01
#define SC    0x01
#define DIV   0x04
#define TIMA  0x05
#define TMA   0x06
#define TAC   0x07
#define IF    0x0F
#define NR10  0x10
#define NR11  0x11
#define NR12  0x12
#define NR13  0x13
#define NR14  0x14
#define NR21  0x16
#define NR22  0x17
#define NR23  0x18
#define NR24  0x19
#define NR30  0x1A
#define NR31  0x1B
#define NR32  0x1C
#define NR33  0x1D
#define NR34  0x1E
#define NR41  0x20
#define NR42  0x21
#define NR43  0x22
#define NR44  0x23
#define NR50  0x24
#define NR51  0x25
#define NR52  0x26
#define LCDC  0x40
#define STAT  0x41
#define SCY   0x42
#define SCX   0x43
#define LY    0x44
#define LYC   0x45
#define DMA   0x46
#define BGP   0x47
#define OBP0  0x48
#define OBP1  0x49
#define WY    0x4A
#define WX    0x4B
#define VBK   0x4F
#define HDMA1 0x51
#define HDMA2 0x52
#define HDMA3 0x53
#define HDMA4 0x54
#define HDMA5 0x55
#define BGPI  0x68
#define BGPD  0x69
#define OBPI  0x6A
#define OBPD  0x6B
#define SVBK  0x70

class MemoryMap
{
private:
	Cartridge *cartridge;

	uint8_t vram[0x2][0x2000];
	uint8_t wram[0x8][0x1000];
	uint8_t oam[0xA0];
	uint8_t io[0x80];
	uint8_t hram[0x7F];
	
	Sprite *sprites[MAX_SPRITES];
	
	uint8_t bg_palette[0x40];
	uint8_t ob_palette[0x40];

	uint32_t div_timer;
	uint32_t tima_timer;

	uint8_t joy_direction;
	uint8_t joy_button;

	uint8_t current_vram_bank;
	uint8_t current_wram_bank;

	uint16_t source_address;
	uint16_t destination_address;
	
	uint8_t IE;
public:
	MemoryMap();
	~MemoryMap();

	void power_up(void);

	uint8_t read_byte(uint16_t address);
	uint16_t read_short(uint16_t address);
	void write_byte(uint16_t address, uint8_t value);
	void write_short(uint16_t address, uint16_t value);

	Cartridge *get_cartridge(void);
	uint8_t is_screen_on(void);

	uint8_t is_interrupt_triggered(void);
	uint8_t is_interrupt_enabled(const interrupt_t *interrupt);
	uint8_t is_interrupt_set(const interrupt_t *interrupt);
	void set_interrupt(const interrupt_t *interrupt);
	void disable_interrupt(const interrupt_t *interrupt);

	uint8_t is_lcd_interrupt_enabled(const lcd_interrupt_t *interrupt);
	
	void increment_div_timer(uint8_t cycles);
	void increment_div(void);
	
	uint8_t is_timer_start(void);
	uint16_t get_tac_cycles(void);
	void increment_tima(void);
	
	uint8_t is_lcd_enabled(void);
	uint8_t is_window_display_enabled(void);
	//uint16_t get_tile_address(uint8_t tile_number);
	uint16_t get_window_tilemap_address();
	uint16_t get_background_tilemap_address();
	uint8_t get_window_tile_number(uint16_t row, uint16_t col);
	uint8_t get_background_tile_number(uint16_t row, uint16_t col);
	uint8_t get_window_tile_info(uint16_t row, uint16_t col);
	uint8_t get_background_tile_info(uint16_t row, uint16_t col);
	
	uint8_t obj_size(void);
	uint8_t obj_display_enabled(void);
	uint8_t bg_display_enabled(void);
	
	uint8_t scy(void);
	uint8_t scx(void);
	uint8_t wy(void);
	uint8_t wx(void);
	uint8_t bgp(void);
	uint8_t obp0(void);
	uint8_t obp1(void);
	void check_lyc(void);
	
	void update_sprite(uint16_t address, uint8_t value);
	Sprite **get_sprites(void);
	
	/*
	void key_press(???);
	void key_release(???);
	*/
};

extern uint16_t TAC_CYCLES[0x04];

#endif