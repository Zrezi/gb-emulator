#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "cartridge.hpp"
#include "config.hpp"

Cartridge::Cartridge()
{
	cgb_mode = 0x00;
	sgb_mode = 0x00;
}

Cartridge::~Cartridge()
{
	if (ram)
	{
		delete[] ram;
	}
	if (rom_banks)
	{
		uint16_t i;
		for (i = 0; i < size->number_of_banks; i++)
		{
			if (rom_banks[i])
			{
				delete[] rom_banks[i];
			}
		}
		delete[] rom_banks;
	}
}

uint8_t Cartridge::load_rom(const char *filepath)
{
	FILE *f;
	uint32_t length;
	uint16_t i, j;
	
	// Open the file
	f = fopen(filepath, "rb");
	if (!f)
	{
		printf("Could not load file\n");
		return 0;
	}

	// Calculate the length of the file, create a byte array, and read the entire file into that array
	fseek(f, 0, SEEK_END);
	length = ftell(f);
	printf("SIZE: %.1f kB (%d B)\n", (length / 1000.0), length);

	fseek(f, 0, SEEK_SET);
	uint8_t data[length];
	fread(data, length, 1, f);

	// Determine the name of the ROM
	memset(name, '\0', 17);
	for(i = 0; i < 16; i++) {
		name[i] = data[i + ROM_OFFSET_NAME];
	}
	printf("ROM Name: %s\n", name);
		
	// Check if it is in color gameboy mode
	if (data[ROM_OFFSET_CGBMODE] == 0x80)
	{
		cgb_mode = 0x01;
	}

	// Check if it's in super gameboy mode
	if (data[ROM_OFFSET_SGBMODE] == 0x03)
	{
		sgb_mode = 0x01;
	}

	// Determine the cartridge type
	type = get_cartridge_type_by_byte(data[ROM_OFFSET_TYPE]);
	if (type)
	{
		printf("ROM Type: %s (0x%02x)\n", type->name, data[ROM_OFFSET_TYPE]);
	}
	else
	{
		printf("Unknown ROM Type Byte: 0x%02X\n", data[ROM_OFFSET_TYPE]);
		return 0;
	}
	
	// Determine the cartridge rom size
	size = get_cartridge_size_by_byte(data[ROM_OFFSET_ROM_SIZE]);
	if (size)
	{
		printf("RAM Banks: %d\n", size->number_of_banks);
		printf("RAM Info: %s\n", size->name);
	}
	else
	{
		printf("Unknown RAM Bank Size Byte: 0x%02X\n", data[ROM_OFFSET_ROM_SIZE]);
		return 0;
	}
	
	// Calculate the ram size
	switch (data[ROM_OFFSET_RAM_SIZE])
	{
		case 0x00:
			ram_size = 0x0;
			break;
		case 0x01:
			ram_size = 0x800; // 2 kB
			break;
		case 0x02:
			ram_size = 0x2000; // 8 kB
			break;
		case 0x03:
			ram_size = 0x8000; // 32 kB
			break;
		case 0x04:
			ram_size = 0x20000; // 128 kB
			break;
	}
	if (type->base_type == BASE_TYPE_ROM)
	{
		ram_size = 0x2000;
	}
	if (type->base_type == BASE_TYPE_MBC2)
	{
		ram_size = 0x200;
	}	
	printf("RAM Size: 0x%05x\n", ram_size);

	// Dynamically allocate space for the ram
	ram = new uint8_t[ram_size];
	for (i = 0; i < ram_size; i++)
	{
		ram[i] = 0xFF;
	}
	ram_enabled = 0x01;
	current_ram_bank = 0x00;

	// Dynamically allocate space for the rom banks
	rom_banks = new uint8_t*[size->number_of_banks];
	for (i = 0; i < size->number_of_banks; i++)
	{
		rom_banks[i] = new uint8_t[0x4000];
	}
	for (i = 0; i < size->number_of_banks; i++)
	{
		for (j = 0; j < 0x4000; j++)
		{
			rom_banks[i][j] = data[(0x4000 * i) + j];
		}
	}
	rom_mode_enabled = 0x01;
	current_rom_bank = 0x01;

	// If a battery is used (for RTC), load the ram data
	if (type->battery_used == 0x01)
	{
		// ignore this for now
		//load_ram();
	}
	
	// Close the file
	fclose(f);
	loaded = 0x01;

	return 1;
}

uint8_t Cartridge::read_rom(uint8_t rom_bank_number, uint16_t address)
{
	return rom_banks[rom_bank_number][address];
}

void Cartridge::write_rom(uint8_t rom_bank_number, uint16_t address, uint8_t value)
{
	rom_banks[rom_bank_number][address] = value;
}

uint8_t Cartridge::read_ram(uint16_t address)
{
	return ram[address];
}

void Cartridge::write_ram(uint16_t address, uint8_t value)
{
	ram[address] = value;
}

cartridge_type_t *get_type(void)
{
	return type;
}

void Cartridge::enable_ram(void)
{
	ram_enabled = 0x01;
}

void Cartridge::disable_ram(void)
{
	ram_enabled = 0x00;
}

uint8_t Cartridge::is_ram_enabled(void)
{
	return ram_enabled;
}

void Cartridge::set_to_rom_mode(void)
{
	rom_mode_enabled = true;
}

void Cartridge::set_to_ram_mode(void)
{
	rom_mode_enabled = false;
}

uint8_t Cartridge::is_rom_mode(void)
{
	return rom_mode_enabled;
}

uint8_t Cartridge::get_current_rom_bank(void)
{
	return current_rom_bank;
}

void Cartridge::set_current_rom_bank(uint8_t bank)
{
	current_rom_bank = bank;
}

uint8_t Cartridge::get_current_ram_bank(void)
{
	return current_ram_bank;
}

void Cartridge::set_current_ram_bank(uint8_t bank)
{
	current_ram_bank = bank;
}

uint32_t Cartridge::get_rtc_latch(void)
{
	uint32_t time = 0x0;
	time += (1000 * rtc_latch_seconds);
	time += (1000 * 60 * rtc_latch_minutes);
	time += (1000 * 60 * 60 * rtc_latch_hours);
	time += (1000 * 60 * 60 * 24 * rtc_latch_days);
	if (rtc_carry)
	{
		time += (1000 * 60 * 60 * 24 * 511);
	}
	return time;
}

void Cartridge::set_rtc_latch(uint32_t time)
{
	printf("check rtc latch if implemented correct\n");
	rtc_latch_seconds = (time / (1000)) % 60;
	rtc_latch_minutes = (time / (1000 * 60)) % 60;
	rtc_latch_hours = (time / (1000 * 60 * 60)) % 24;
	rtc_latch_days = (time / (1000 * 60 * 60 * 24));
	rtc_carry = rtc_latch_days > 0x1FF;
	rtc_latch_days %= 0x200;
}

void Cartridge::enable_rtc_latch(void)
{
	rtc_latch_enabled = 0x01;
}

void Cartridge::disable_rtc_latch(void)
{
	rtc_latch_enabled = 0x00;
}

uint32_t Cartridge::get_rtc_latch_seconds(void)
{
	return rtc_latch_seconds;
}

void Cartridge::set_rtc_latch_seconds(uint32_t seconds)
{
	rtc_latch_seconds = seconds;
}

uint32_t Cartridge::get_rtc_latch_minutes(void)
{
	return rtc_latch_minutes;
}

void Cartridge::set_rtc_latch_minutes(uint32_t minutes)
{
	rtc_latch_minutes = minutes;
}

uint32_t Cartridge::get_rtc_latch_hours(void)
{
	return rtc_latch_hours;
}

void Cartridge::set_rtc_latch_hours(uint32_t hours)
{
	rtc_latch_hours = hours;
}

uint32_t Cartridge::get_rtc_latch_days(void)
{
	return rtc_latch_days;
}

void Cartridge::set_rtc_latch_days(uint32_t days)
{
	rtc_latch_days = days;
	rtc_carry = (rtc_latch_days > 0x1FF);
}
	
uint8_t Cartridge::set_rtc_carry(uint8_t carry)
{
	rtc_carry = carry;
}

uint8_t Cartridge::set_rtc_halt(uint8_t halt)
{
	rtc_halt = halt;
}

uint8_t Cartridge::is_rtc_halt(void)
{
	return rtc_halt;
}

uint8_t Cartridge::Cartridge::is_cgb(void)
{
	return cgb_mode;
}

uint8_t Cartridge::is_sgb(void)
{
	return sgb_mode;
}

uint8_t Cartridge::is_loaded(void)
{
	return loaded;
}