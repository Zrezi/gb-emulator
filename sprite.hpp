#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

class Sprite
{
private:
	uint8_t index;
	uint8_t x;
	uint8_t y;
	uint8_t tile_number;
	uint8_t attribute_flags;
public:
	Sprite();
	Sprite(uint8_t _index);

	uint8_t get_index(void);
	void set_index(uint8_t _index);
	uint8_t get_x(void);
	void set_x(uint8_t _x);
	uint8_t get_y(void);
	void set_y(uint8_t _y);
	uint8_t get_tile_number(void);
	void set_tile_number(uint8_t _tile_number);
	uint8_t get_attribute_flags(void);
	void set_attribute_flags(uint8_t _attribute_flags);

	uint8_t is_sprite_above_background(void);
	uint8_t x_flip(void);
	uint8_t y_flip(void);
	uint8_t gameboy_palette_number(void);
	uint8_t get_tile_bank(void);
	uint8_t color_gameboy_palette_number(void);
	
	int8_t compare_to(Sprite *sprite);
};

#endif