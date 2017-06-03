#include <stdint.h>

#include "sprite.hpp"

Sprite::Sprite()
{
	index = 0x00;
}

Sprite::Sprite(uint8_t _index)
{
	index = _index;
}

uint8_t Sprite::get_index(void)
{
	return index;
}

void Sprite::set_index(uint8_t _index)
{
	index = _index;
}

uint8_t Sprite::get_x(void)
{
	return x;
}

void Sprite::set_x(uint8_t _x)
{
	x = _x;
}

uint8_t Sprite::get_y(void)
{
	return y;
}

void Sprite::set_y(uint8_t _y)
{
	y = _y;
}

uint8_t Sprite::get_tile_number(void)
{
	return tile_number;
}

void Sprite::set_tile_number(uint8_t _tile_number)
{
	tile_number = _tile_number;
}

uint8_t Sprite::get_attribute_flags(void)
{
	return attribute_flags;
}

void Sprite::set_attribute_flags(uint8_t _attribute_flags)
{
	attribute_flags = _attribute_flags;
}

uint8_t Sprite::is_sprite_above_background(void)
{
	return ((attribute_flags & 0x80) == 0x80);
}

uint8_t Sprite::x_flip(void)
{
	return ((attribute_flags & 0x20) == 0x20);
}

uint8_t Sprite::y_flip(void)
{
	return ((attribute_flags & 0x40) == 0x40);
}

uint8_t Sprite::gameboy_palette_number(void)
{
	return ((attribute_flags & 0x10) >> 4);
}

uint8_t Sprite::get_tile_bank(void)
{
	return ((attribute_flags & 0x08) >> 3);
}

uint8_t Sprite::color_gameboy_palette_number(void)
{
	return (attribute_flags & 0x07);
}

int8_t Sprite::compare_to(Sprite *sprite)
{
	if (x == sprite->get_x())
	{
		return sprite->get_index() - index;
	}
	else
	{
		return sprite->get_x() - x;
	}
}
