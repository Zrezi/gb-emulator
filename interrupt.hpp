#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#define INTERRUPT_VBLANK  0x00
#define INTERRUPT_LCDSTAT 0x01
#define INTERRUPT_TIMER   0x02
#define INTERRUPT_SERIAL  0x03
#define INTERRUPT_JOYPAD  0x04

struct interrupt_t
{
	const char *name;
	uint8_t mask;
	uint8_t vector;
};

extern struct interrupt_t interrupts[0x05];

interrupt_t *get_interrupt_by_index(uint8_t index);

#endif