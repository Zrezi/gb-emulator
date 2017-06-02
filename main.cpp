#include <stdio.h>
#include "memorymap.hpp"

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("Give it a file input\n");
		return -1;
	}

	MemoryMap *m = new MemoryMap();

	m->get_cartridge()->load_rom(argv[1]);

	m->power_up();

	delete m;

	return 0;
}