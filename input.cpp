#include <stdint.h>

#include "input.hpp"

struct input_dpad_t inputs_dpad[0x04] = {
	{ "RIGHT", 0x01 },
	{ "LEFT",  0x02 },
	{ "UP",    0x03 },
	{ "DOWN",  0x04 }
};

input_dpad_t *get_input_dpad_by_index(uint8_t index)
{
	if (index < 0 || index > 4)
	{
		return nullptr;
	}
	return &inputs_dpad[index];
}

struct input_button_t inputs_button[0x04] = {
	{ "A",      0x01 },
	{ "B",      0x02 },
	{ "SELECT", 0x04 },
	{ "START",  0x08 }
};

input_button_t *get_input_button_by_index(uint8_t index)
{
	if (index < 0 || index > 4)
	{
		return nullptr;
	}
	return &inputs_button[index];
}