#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

#define INTERRUPTS_VBLANK	(1 << 0)
#define INTERRUPTS_LCDSTAT	(1 << 1)
#define INTERRUPTS_TIMER	(1 << 2)
#define INTERRUPTS_SERIAL	(1 << 3)
#define INTERRUPTS_JOYPAD	(1 << 4)

struct interrupt
{
	uint8_t master;
	uint8_t enable;
	uint8_t flags;
} extern interrupt;

#endif