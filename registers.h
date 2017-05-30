#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

struct registers
{
	// Two 8-bit registers a and f
	// Can be accessed as one 16-bit register af
	struct
	{
		union
		{
			struct
			{
				uint8_t a;
				uint8_t f;
			};
			uint16_t af;
		};
	};
	
	// Two 8-bit registers b and c
	// Can be accessed as one 16-bit register bc
	struct
	{
		union
		{
			struct
			{
				uint8_t b;
				uint8_t c;
			};
			uint16_t bc;
		};
	};
	
	// Two 8-bit registers d and e
	// Can be accessed as one 16-bit register de
	struct
	{
		union
		{
			struct
			{
				uint8_t d;
				uint8_t e;
			};
			uint16_t de;
		};
	};
	
	// Two 8-bit registers h and l
	// Can be accessed as one 16-bit register hl
	struct
	{
		union
		{
			struct
			{
				uint8_t h;
				uint8_t l;
			};
			uint16_t hl;
		};
	};

	// 16-bit Stack Pointer
	uint16_t sp;
	
	// 16-bit Program Counter
	uint16_t pc;
	
} extern registers;

#endif