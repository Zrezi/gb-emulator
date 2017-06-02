#include <stdio.h>
#include "cartridge.hpp"

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("Give it a file input\n");
		return -1;
	}

	Cartridge *c = new Cartridge();

	c->load_rom(argv[1]);

	delete c;

	return 0;
}