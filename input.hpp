#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

#define DPAD_RIGHT    0x01
#define DPAD_LEFT     0x02
#define DPAD_UP       0x04
#define DPAD_DOWN     0x08

#define BUTTON_A      0x01
#define BUTTON_B      0x02
#define BUTTON_SELECT 0x04
#define BUTTON_START  0x08

struct input_dpad_t
{
	const char *name;
	uint8_t mask;
};
extern struct input_dpad_t inputs_dpad[0x04];
input_dpad_t *get_input_dpad_by_index(uint8_t index);

struct input_button_t
{
	const char *name;
	uint8_t mask;
};
extern struct input_button_t inputs_button[0x04];
input_button_t *get_input_button_by_index(uint8_t index);

#endif