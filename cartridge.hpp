#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include <stdint.h>

#include "cartridgesize.hpp"
#include "cartridgetype.hpp"

#define ROM_OFFSET_NAME 0x134
#define ROM_OFFSET_CGBMODE 0x143
#define ROM_OFFSET_SGBMODE 0x146
#define ROM_OFFSET_TYPE 0x147
#define ROM_OFFSET_ROM_SIZE 0x148
#define ROM_OFFSET_RAM_SIZE 0x149

/*enum mode_t
{
	CGB_SUPPORT,
	CGB_ONLY,
	GB
};*/

class Cartridge
{
private:
	char name[17];

	cartridge_type_t *type;
	cartridge_size_t *size;
	//mode_t mode;

	uint8_t cgb_mode;
	uint8_t sgb_mode;

	uint8_t **rom_banks;
	uint8_t current_rom_bank;
	uint8_t rom_mode_enabled;

	uint32_t ram_size;
	uint8_t *ram;
	uint8_t current_ram_bank;
	uint8_t ram_enabled;

	uint8_t rtc_latch_enabled;
	uint32_t start_time;
	uint32_t rtc_latch_seconds;
	uint32_t rtc_latch_minutes;
	uint32_t rtc_latch_hours;
	uint32_t rtc_latch_days;
	uint8_t rtc_carry;
	uint8_t rtc_halt;
	uint32_t rtc_halt_time;

	uint8_t loaded;

public:
	Cartridge();
	~Cartridge();

	uint8_t load_rom(const char *filepath);

	uint8_t read_rom(uint8_t rom_bank_number, uint16_t address);
	void write_rom(uint8_t rom_bank_number, uint16_t address, uint8_t value);
	uint8_t read_ram(uint16_t address);
	void write_ram(uint16_t address, uint8_t value);
	
	cartridge_type_t *get_type(void);

	void enable_ram(void);
	void disable_ram(void);
	uint8_t is_ram_enabled(void);

	void set_to_rom_mode(void);
	void set_to_ram_mode(void);
	uint8_t is_rom_mode(void);

	uint8_t get_current_rom_bank(void);
	void set_current_rom_bank(uint8_t bank);
	uint8_t get_current_ram_bank(void);
	void set_current_ram_bank(uint8_t bank);

	uint8_t is_cgb(void);
	uint8_t is_sgb(void);
	
	void enable_rtc_latch(void);
	void disable_rtc_latch(void);
	uint32_t get_rtc_latch(void);
	uint32_t get_rtc_latch_seconds(void);
	void set_rtc_latch_seconds(uint32_t seconds);
	uint32_t get_rtc_latch_minutes(void);
	void set_rtc_latch_minutes(uint32_t minutes);
	uint32_t get_rtc_latch_hours(void);
	void set_rtc_latch_hours(uint32_t hours);
	uint32_t get_rtc_latch_days(void);
	void set_rtc_latch_days(uint32_t days);
	void set_rtc_latch(uint32_t time);
	
	void set_rtc_carry(uint8_t carry);
	void set_rtc_halt(uint8_t halt);
	void set_rtc_halt_time(uint32_t halt_time);

	uint8_t is_rtc_halt(void);

	uint8_t is_loaded(void);
};

#endif