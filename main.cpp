#include <stdio.h>
#include "z80.hpp"

void shutdown()
{
	exit(0);
}

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("Give it a file input\n");
		return -1;
	}

	/*MemoryMap *m = new MemoryMap();

	m->get_cartridge()->load_rom(argv[1]);

	m->power_up();

	delete m;*/

	Z80 *z = new Z80();
	z->get_memory()->get_cartridge()->load_rom(argv[1]);
	z->init();
	delete z;

	return 0;
}