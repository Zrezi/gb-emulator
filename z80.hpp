#ifndef Z80_H
#define Z80_H

#include <stdint.h>

#include "memorymap.hpp"

#define Z_FLAG 0x80
#define N_FLAG 0x40
#define H_FLAG 0x20
#define C_FLAG 0x10

struct registers_t
{
	struct {
		union {
			struct {
				uint8_t a;
				uint8_t f;
			};
			uint16_t af;
		};
	};
	struct {
		union {
			struct {
				uint8_t b;
				uint8_t c;
			};
			uint16_t bc;
		};
	};
	struct {
		union {
			struct {
				uint8_t d;
				uint8_t e;
			};
			uint16_t de;
		};
	};
	struct {
		union {
			struct {
				uint8_t h;
				uint8_t l;
			};
			uint16_t hl;
		};
	};
	uint16_t sp;
	uint16_t pc;
};

extern uint8_t cycles[0x100];
extern uint8_t cycles_cb[0x100];

class Z80
{
private:
	MemoryMap *memory;
	registers_t registers;

	uint8_t stop_flag;
	uint8_t halt_flag;

	/*
		ime = interrupt master enable
	*/
	uint8_t set_ime_flag;
	uint8_t disable_ime_flag;
	uint8_t ime;

	uint16_t cycles_run;
	uint8_t next_instruction;
public:
	Z80();
	~Z80();

	MemoryMap *get_memory(void);
	void init();

	uint16_t execute_one(void);
};

#endif