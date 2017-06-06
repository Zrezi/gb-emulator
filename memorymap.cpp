#include <stdio.h>
#include <stdint.h>

#include "memorymap.hpp"
#include "cartridgetype.hpp"

MemoryMap::MemoryMap()
{
	cartridge = new Cartridge();
}

/*
	delete dynamic array for the sprite data
	
*/
MemoryMap::~MemoryMap()
{
	uint8_t i;
	for (i = 0; i < MAX_SPRITES; i++)
	{
		if (sprites[i])
		{
			delete sprites[i];
		}
	}
	
	delete cartridge;
}

void MemoryMap::power_up(void)
{
	if (cartridge->is_loaded() == 0)
	{
		printf("Cartridge has not been inserted / loaded\n");
		return;
	}

	joy_direction = 0x0F;
	joy_button = 0x0F;
	
	io[JOYP] = 0xFF; // all buttons are pressed initially?
	
	// as per power up sequence on page 18
	io[TIMA] = 0x00;
	io[TMA] = 0x00;
	io[TAC] = 0x00;
	io[NR10] = 0x80;
	io[NR11] = 0xBF;
	io[NR12] = 0xF3;
	io[NR14] = 0xBF;
	io[NR21] = 0x3F;
	io[NR22] = 0x00;
	io[NR24] = 0xBF;
	io[NR30] = 0x7F;
	io[NR31] = 0xFF;
	io[NR32] = 0x9F;
	io[NR34] = 0xBF;
	io[NR41] = 0xFF;
	io[NR42] = 0x00;
	io[NR43] = 0x00;
	io[NR44] = 0xBF;
	io[NR50] = 0x77;
	io[NR51] = 0xF3;
	io[NR52] = 0xF1;
	io[LCDC] = 0x91;
	io[SCY] = 0x00;
	io[SCX] = 0x00;
	io[LYC] = 0x00;
	io[BGP] = 0xFC;
	io[OBP0] = 0xFF;
	io[OBP1] = 0xFF;
	io[WY] = 0x00;
	io[WX] = 0x00;
	io[HDMA5] = 0xFF;
	IE = 0x00;
	
	if (cartridge->is_sgb())
	{
		io[NR52] = 0xF0;
	}

	div_timer = 0x00;
	tima_timer = 0x00;

	current_vram_bank = 0x00;
	current_wram_bank = 0x01;
	
	uint8_t i;
	for (i = 0; i < MAX_SPRITES; i++)
	{
		sprites[i] = new Sprite(i);
	}

	source_address = 0x0000;
	destination_address = 0x0000;
}

uint8_t MemoryMap::read_byte(uint16_t address)
{
	uint8_t value = 0x00;

	switch (address & 0xF000)
	{
		case 0x0000:
		case 0x1000:
		case 0x2000:
		case 0x3000:
			value = cartridge->read_rom(0, address);
			break;
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
			value = cartridge->read_rom(cartridge->get_current_rom_bank(), address - 0x4000);
			break;
		case 0x8000:
		case 0x9000:
			value = vram[current_vram_bank][address - 0x8000];
			break;
		case 0xA000:
		case 0xB000:
			if (!cartridge->is_ram_enabled())
			{
				return 0xFF;
			}
			switch (cartridge->get_type()->base_type)
			{
				case BASE_TYPE_ROM:
					value = cartridge->read_ram(address - 0xA000);
					break;
				case BASE_TYPE_MBC1:
				case BASE_TYPE_MBC5:
					value = cartridge->read_ram((cartridge->get_current_ram_bank() * 0x2000) + (address - 0xA000));
					break;
				case BASE_TYPE_MBC2:
					if (address < 0xA200)
					{
						value = cartridge->read_ram(address - 0xA000) & 0x0F;
					}
					break;
				case BASE_TYPE_MBC3:	
					switch (cartridge->get_current_ram_bank() & 0x0F)
					{
						case 0x0:
						case 0x1:
						case 0x2:
						case 0x3:
							value = cartridge->read_ram((cartridge->get_current_ram_bank() * 0x2000) + (address - 0xA000));
							break;
						case 0x8:
							value = cartridge->get_rtc_latch_seconds();
							break;
						case 0x9:
							value = cartridge->get_rtc_latch_minutes();
							break;
						case 0xA:
							value = cartridge->get_rtc_latch_hours();
							break;
						case 0xB:
							value = cartridge->get_rtc_latch_days() & 0xFF;
							break;
						case 0xC:
							/* TODO: temp doesn't do anything?? */
							break;
					}
					break;
				default:
					value = cartridge->read_ram(address - 0xA000);
					break;
			}
			break;
		case 0xC000:
			value = wram[0][address - 0xC000];
			break;
		case 0xD000:
			value = wram[current_wram_bank][address - 0xD000];
			break;
		case 0xE000:
			value = wram[0][address - 0xE000];
			break;
		case 0xF000:
			if (address < 0xFE00)
			{
				value = wram[current_wram_bank][address - 0xF000];
			}
			else if (address < 0xFEA0)
			{
				value = oam[address - 0xFE00];
			}
			else if (address < 0xFF00)
			{
				// not used?
			}
			else if (address == 0xFF00)
			{
				if ((~io[JOYP] & (1 << 4)) == (1 << 4))
				{
					return joy_direction | 0xF0;
				}
				else if ((~io[JOYP] & (1 << 5)) == (1 << 5))
				{
					return joy_button | 0xF0;
				}
			}
			else if (address < 0xFF80)
			{
				value = io[address - 0xFF00];
				if ((address - 0xFF00) == BGPD)
				{
					value = bg_palette[io[BGPI] & 0x3F];
				}
				if ((address - 0xFF00) == OBPD)
				{
					value = ob_palette[io[OBPI] & 0x3F];
				}
			}
			else if (address < 0xFFFF)
			{
				value = hram[address - 0xFF80];
			}
			else
			{
				value = IE;
			}
			break;
		default:
			printf("Trying to read from unrecognized address: 0x%04x\n", address);
			break;
	}

	return value;
}

uint16_t MemoryMap::read_short(uint16_t address)
{
	return (read_byte(address)) | (read_byte(address + 1) << 8);
}

void MemoryMap::write_byte(uint16_t address, uint8_t value)
{
	uint8_t is_halt = 0, should_screen_be_on = 0, io_addr = 0; // for use in switch
	switch (address & 0xF000)
	{
		case 0x0000:
		case 0x1000:
			switch (cartridge->get_type()->base_type)
			{
				case BASE_TYPE_MBC1:
				case BASE_TYPE_MBC3:
				case BASE_TYPE_MBC5:
					if ((value & 0x0F) == 0x0A)
					{
						cartridge->enable_ram();
					}
					else
					{
						cartridge->disable_ram();
					}
					break;
				case BASE_TYPE_MBC2:
					if ((address & 0x0100) == 0x0000)
					{
						cartridge->enable_ram();
					}
					else
					{
						cartridge->disable_ram();
					}
					break;
				default:
					break;
			}
			break;
		case 0x2000:
		case 0x3000:
			switch (cartridge->get_type()->base_type)
			{
				case BASE_TYPE_MBC1:
					value &= 0x1F;
					if (value == 0x00)
					{
						value++;
					}
					cartridge->set_current_rom_bank((cartridge->get_current_rom_bank() & 0x60) | value);
					break;
				case BASE_TYPE_MBC2:
					if ((address & 0x0100) == 0x0000)
					{
						cartridge->set_current_rom_bank(value & 0x0F);
					}
					break;
				case BASE_TYPE_MBC3:
					value &= 0x7F;
					if (value == 0x00)
					{
						value++;
					}
					cartridge->set_current_rom_bank(value);
					break;
				case BASE_TYPE_MBC5:
					if (address < 0x3000)
					{
						cartridge->set_current_rom_bank((cartridge->get_current_rom_bank() & 0x0100) | value);
					}
					else
					{
						cartridge->set_current_rom_bank((cartridge->get_current_rom_bank() & 0x00FF) | ((value & 0x01) << 8));
					}
					break;
				default:
					break;
			}
			break;
		case 0x4000:
		case 0x5000:
			switch (cartridge->get_type()->base_type)
			{
				case BASE_TYPE_MBC1:
					if (cartridge->is_rom_mode())
					{
						cartridge->set_current_rom_bank((cartridge->get_current_rom_bank() & 0x1F) | ((value & 0x03) << 5));
						uint8_t crb = cartridge->get_current_rom_bank();
						if (crb == 0x20 || crb == 0x40 || crb == 0x60)
						{
							cartridge->set_current_rom_bank(crb + 1);
						}
					}
					else
					{
						cartridge->set_current_ram_bank(value & 0x03);
					}
					break;
				case BASE_TYPE_MBC3:
				case BASE_TYPE_MBC5:
					cartridge->set_current_ram_bank(value & 0x0F);
					break;
				default:
					break;
			}
			break;
		case 0x6000:
		case 0x7000:
			switch (cartridge->get_type()->base_type)
			{
				case BASE_TYPE_MBC1:
					if ((value & 0x01) == 0x00)
					{
						cartridge->set_to_rom_mode();
					}
					else
					{
						cartridge->set_to_ram_mode();
					}
					break;
				case BASE_TYPE_MBC3:
					if (cartridge->is_ram_enabled())
					{
						if (value == 0x01)
						{
							// TODO implement set rtc latch function based on a time
							cartridge->set_rtc_latch(0);
						}
						else if (value == 0x00)
						{
							cartridge->enable_rtc_latch();
						}
						else {
							cartridge->disable_rtc_latch();
						}
					}
					break;
				default:
					break;
			}
			break;
		case 0x8000:
		case 0x9000:
			vram[current_vram_bank][address - 0x8000] = value;
			break;
		case 0xA000:
		case 0xB000:
			if (!cartridge->is_ram_enabled())
			{
				return;
			}
			switch (cartridge->get_type()->base_type)
			{
				case BASE_TYPE_ROM:
					cartridge->write_ram((address - 0xA000), value);
					break;
				case BASE_TYPE_MBC1:
				case BASE_TYPE_MBC5:
					cartridge->write_ram((cartridge->get_current_ram_bank() * 0x2000) + (address - 0xA000), value);
					break;
				case BASE_TYPE_MBC2:
					if (address < 0xA200)
					{
						cartridge->write_ram((address - 0xA000), (value & 0x0F));
					}
					break;
				case BASE_TYPE_MBC3:
					switch (cartridge->get_current_ram_bank() & 0x0F)
					{
						case 0x00:
						case 0x01:
						case 0x02:
						case 0x03:
							cartridge->write_ram((cartridge->get_current_ram_bank() * 0x2000) + (address - 0xA000), value);
							break;
						case 0x08:
							cartridge->set_rtc_latch_seconds(value);
							break;
						case 0x09:
							cartridge->set_rtc_latch_minutes(value);
							break;
						case 0x0A:
							cartridge->set_rtc_latch_hours(value);
							break;
						case 0x0B:
							cartridge->set_rtc_latch_days((cartridge->get_rtc_latch_days() & 0x0100) | value);
							break;
						case 0x0C:
							cartridge->set_rtc_latch_days((cartridge->get_rtc_latch_days() & 0x00FF) | ((value & 0x01) << 8));
							is_halt = ((value & 0x40) == 0x40);
							cartridge->set_rtc_carry((value & 0x80) == 0x80);
							if (is_halt)
							{
								cartridge->set_rtc_halt_time(cartridge->get_rtc_latch());
							}
							if (cartridge->is_rtc_halt() && !is_halt)
							{
								//uint32_t halt_time = cartridge->get_rtc_latch() - cartridge->get_rtc_halt_time();
								// TODO
								// cart.getStartTime().setTimeInMillis(cart.getStartTime().getTimeInMillis() + haltTime);
							}
							cartridge->set_rtc_halt(is_halt);
							break;
						default:
							break;
					}
					break;
				default:
					cartridge->write_ram((address - 0xA000), value);
					break;
			}
			break;
		case 0xC000:
			wram[0][address - 0xC000] = value;
			break;
		case 0xD000:
			wram[current_wram_bank][address - 0xD000] = value;
			break;
		case 0xE000:
			wram[0][address - 0xE000] = value;
			break;
		case 0xF000:
			if (address < 0xFE00)
			{
				wram[current_wram_bank][address - 0xF000] = value;
			}
			else if (address < 0xFEA0)
			{
				oam[address - 0xFE00] = value;
				update_sprite(address - 0xFE00, value);
			}
			else if (address < 0xFF00)
			{
				// do nothing
			}
			else if (address < 0xFF80)
			{
				io_addr = (address - 0xFF00);
				if (io_addr == SB)
				{
					//
				}
				if (io_addr == DIV)
				{
					value = 0x00;
					div_timer = 0x00;
				}
				if (io_addr == TAC)
				{
					value &= 0x07;
					if (((value & 0x3) != (io[TAC] & 0x3)) || ((value & 0x4) == 0))
					{
                        tima_timer = 0;
                        io[TIMA] = io[TMA];
					}
				}
				if (io_addr == IF)
				{
					value |= 0xE0;
				}
				if (io_addr == STAT)
				{
					value = (value & ~0x07) | (io[STAT] & 0x07);
				}
				if (io_addr == LY)
				{
					value = 0x00;
				}
				if (io_addr == LCDC)
				{
					if (((value & 0x80) == 0x80))
					{
						should_screen_be_on = 1;
					}
					if (is_screen_on() && !should_screen_be_on)
					{
						io[LY] = 0x00;
						io[STAT] &= ~0x03;
						
						check_lyc();
					}
					else if(!is_screen_on() && should_screen_be_on == 1)
					{
						// reset lcdmodetimer?
					}
				}
				if (io_addr == DMA)
				{
					uint8_t i;
					for (i = 0x00; i < 0x80; i++) {
						uint16_t temp_addr = 0x0000;
						temp_addr |= (value << 8);
						temp_addr |= i;
                        oam[i] = read_byte(temp_addr);
                        update_sprite(i, oam[i]);
					}
				}
				if (io_addr == VBK && cartridge->is_cgb())
				{
					current_vram_bank = value & 0x01;
				}
				if (io_addr == HDMA5 && cartridge->is_cgb())
				{
					// TODO A LOT OF STUFF
				}
				if (address - 0xFF00 == BGPD)
				{
					bg_palette[io[BGPI] & 0x3F] = value;
					if ((io[BGPI] & 0x80) == 0x80)
					{
						io[BGPI]++;
						io[BGPI] &= ~0x40;
					}
				}
				if (address - 0xFF00 == OBPD)
				{
					ob_palette[io[OBPI] & 0x3F] = value;
					if ((io[OBPI] & 0x80) == 0x80)
					{
						io[OBPI]++;
						io[OBPI] &= ~0x40;
					}
				}
				if (io_addr == SVBK && cartridge->is_cgb())
				{
					current_wram_bank = value & 0x07;
					if (current_wram_bank == 0)
					{
						current_wram_bank++;
					}
				}
				io[io_addr] = value;
				if (io_addr == LYC)
				{
					check_lyc();
				}
			}
			else if (address < 0xFFFF)
			{
				hram[address - 0xFF80] = value;
			}
			else {
				IE = value;
			}
			break;
	}
}

void MemoryMap::write_short(uint16_t address, uint16_t value)
{
	write_byte(address, (uint8_t) (value & 0x00FF));
	write_byte(address + 1, (uint8_t) ((value & 0xFF00) >> 8));
}

Cartridge *MemoryMap::get_cartridge(void)
{
	return cartridge;
}

uint8_t MemoryMap::is_screen_on(void)
{
	return ((io[LCDC] & 0x80) == 0x80);
}

/*
	checks if any enabled interrupts (bits who have 1's in io[IF]) is "triggered" in IE
	by bitwise AND-ing and checking if that result isn't zero (meaning at least one interrupt
	that is enabled has been triggered)
*/
uint8_t MemoryMap::is_interrupt_triggered(void)
{
	return ((IE & io[IF]) != 0x00);
}

uint8_t MemoryMap::is_interrupt_enabled(const interrupt_t *interrupt)
{
	return ((IE & interrupt->mask) == interrupt->mask);
}

/*
	checks a pointed to interrupt's mask against the value in FF0F (io[IF])
	bit 4 = pin jumps (input)
	bit 3 = serial
	bit 2 = timer
	bit 1 = lcdc
	bit 0 = v blank
	
	this corresponds with the initialized mask values in interrupt.cpp
	page 39-40
*/
uint8_t MemoryMap::is_interrupt_set(const interrupt_t *interrupt)
{
	return ((io[IF] & interrupt->mask) == interrupt->mask);
}

/*
	or equals the current interrupt flags with a given interrupt's mask
*/
void MemoryMap::set_interrupt(const interrupt_t *interrupt)
{
	io[IF] |= interrupt->mask;
}

/*
	and equals the current interrupt flags with the inverse of the interrupt's mask
	for example:
	if timer, lcdc, and vblank are enabled, io[IF] is equal to 0b00000111
	if you want to disable timer, timer's mask is 0b00000100
	the inverse of timer mask is 0b11111011
	and equals-ing with that inverse mask gives you 1's in anything that was already a 1 (leaving
	all other enabled interrupts alone), but clearing the timer bit of the value
*/
void MemoryMap::disable_interrupt(const interrupt_t *interrupt)
{
	io[IF] &= ~interrupt->mask;
}

uint8_t MemoryMap::is_lcd_interrupt_enabled(const lcd_interrupt_t *interrupt)
{
	return ((io[STAT] & interrupt->mask) == interrupt->mask);
}

void MemoryMap::increment_div_timer(uint8_t cycles)
{
	div_timer += cycles;
	if (div_timer >= DIV_RATE)
	{
		div_timer -= DIV_RATE;
		increment_div();
	}
}

void MemoryMap::increment_div(void)
{
	io[DIV]++;
}

/*
	Checks bit 2 of TAC which is the "Timer Stop" bit for timer control
	page 39
*/
uint8_t MemoryMap::is_timer_start(void)
{
	return ((io[TAC] & 0x04) == 0x04);
}

/*
	input clock cycles
	note that clock speed is 4.194... MHz so these values are based on the below information
	00 = 4.096   KHz
	01 = 262.144 KHz
	10 = 65.536  KHz
	11 = 16.384  KHz
	page 39
	
	and-ing with 0x03 gets the last 2 bits (bit 1 and bit 0) which are the only ones we want
*/
uint16_t TAC_CYCLES[0x04] = {
	1024,
	16,
	64,
	256
};
uint16_t MemoryMap::get_tac_cycles(void)
{
	return TAC_CYCLES[(io[TAC] & 0x03)];
}

/*
	increments TIMA (timer counter)
	Generates interrupt when overflows and resets it to TMA (timer modulo)
	Page 38
*/
void MemoryMap::increment_tima(void)
{
	if (io[TIMA] == 0xFF)
	{
		io[TIMA] = io[TMA];
		set_interrupt(get_interrupt_by_index(INTERRUPT_TIMER));
	}
	else
	{
		io[TIMA]++;
	}
}

/*
	Select bit 7 of LCDC (lcd control operation)
	0 = stop completely (no picture on screen)
	1 = operation
	page 51-52
*/
uint8_t MemoryMap::is_lcd_enabled(void)
{
	return ((io[LCDC] & 0x80) == 0x80);
}

/*
	Select bit 5 of LCDC (window display) unless color gameboy
	0 = off
	1 = on
	page 52
*/
uint8_t MemoryMap::is_window_display_enabled(void)
{
	if (cartridge->is_cgb())
	{
		return bg_display_enabled();
	}
	else
	{
		return ((io[LCDC] & 0x20) == 0x20);
	}
}

/*uint16_t MemoryMap::get_tile_address(uint8_t tile_number)
{
	uint16_t tile_address = 0x0000;
	
	if ((io[LCDC] & 0x10) == 0x10)
	{
		tile_address = 0x0000;
	}
	else
	{
		tile_address = 0x1000;
		if (tile_number > 127)
		{
			tileN
	}
	
	return tile_address;
}*/

/*
	select bit 6 of LCDC which is "Window Tile Map Display Select"
	0 = 0x9800 thru 0x9BFF
	1 = 0x9C00 thru 0x9FFF
	page 52
	
	this provides an offset from 0x8000
*/
uint16_t MemoryMap::get_window_tilemap_address()
{
	if ((io[LCDC] & 0x40) == 0x00)
	{
		return 0x1800;
	}
	else
	{
		return 0x1C00;
	}
}

/*
	works same way as get_window_tilemap_address(), but checking bit 3 instead
	page 52
*/
uint16_t MemoryMap::get_background_tilemap_address()
{
	if ((io[LCDC] & 0x08) == 0x00)
	{
		return 0x1800;
	}
	else
	{
		return 0x1C00;
	}
}

/*
	Consolidates a lot of memory calculates by using the 2 functions above
	This is for getting tile data from vram
	tile information starts on page 22
*/
uint8_t MemoryMap::get_window_tile_number(uint16_t row, uint16_t col)
{
	return vram[0][get_window_tilemap_address() + (row * 32) + col];
}
uint8_t MemoryMap::get_background_tile_number(uint16_t row, uint16_t col)
{
	return vram[0][get_background_tilemap_address() + (row * 32) + col];
}
uint8_t MemoryMap::get_window_tile_info(uint16_t row, uint16_t col)
{
	return vram[1][get_window_tilemap_address() + (row * 32) + col];
}
uint8_t MemoryMap::get_background_tile_info(uint16_t row, uint16_t col)
{
	return vram[1][get_background_tilemap_address() + (row * 32) + col];
}

/*
	select bit 2 of LCDC (obj size data)
	0 = 8
	1 = 16
	page 52
*/
uint8_t MemoryMap::obj_size(void)
{
	return ((io[LCDC] & 0x04) == 0x04) ? 16 : 8;
}

/*
	select bit 1 of LCDC (obj (sprite) display)
	0 = off
	1 = on
	page 52
*/
uint8_t MemoryMap::obj_display_enabled(void)
{
	return ((io[LCDC] & 0x02) == 0x02);
};

/*
	select bit 0 of LCDC (background and window display)
	0 = off
	1 = on
	page 52
*/
uint8_t MemoryMap::bg_display_enabled(void)
{
	return ((io[LCDC] & 0x01) == 0x01);
};

/*
	scroll background y position
	page 54
*/
uint8_t MemoryMap::scy(void)
{
	return io[SCY];
}

/*
	scroll background x position
	page 55
*/
uint8_t MemoryMap::scx(void)
{
	return io[SCX];
}

/*
	window y position
	page 58
*/
uint8_t MemoryMap::wy(void)
{
	return io[WY];
}

/*
	window x position
	page 58-59
*/
uint8_t MemoryMap::wx(void)
{
	return io[WX];
}

/*
	background window palette data
	page 57
*/
uint8_t MemoryMap::bgp(void)
{
	return io[BGP];
}

/*
	object palette 0 data
	page 58
*/
uint8_t MemoryMap::obp0(void)
{
	return io[OBP0];
}

/*
	object palette 1 data
	page 58
*/
uint8_t MemoryMap::obp1(void)
{
	return io[OBP1];
}

void MemoryMap::check_lyc(void)
{
	if (io[LY] == io[LYC])
	{
		io[STAT] |= 0x04;
		if (is_lcd_interrupt_enabled(get_lcd_interrupt_by_index(LCD_INTERRUPT_LYC_LY)) && is_screen_on())
		{
			set_interrupt(get_interrupt_by_index(INTERRUPT_LCDSTAT));
		}
	}
	else
	{
		io[STAT] &= ~0x04;
	}
}

/*
	Since we know all sprites take 4 bytes of data, we can bitshift the address right twice to ignore the
	lower 2 bits. Now checking the lower two bits (which started as bits 2 and 3) allows us to
	calculate an index easily (since it's a multiple of 4)
	
	once we have our index, we can loop thru a for loop with i checking against all of our sprites,
	stopping when we have a match
	
	bitwise and-ing the original address with 0x03 gives us 4 different values to edit, which are:
	00 = y position
	01 = x position
	10 = pattern number
	11 = flags
		bit 7 = priority
		bit 6 = y flip
		bit 5 = x flip
		bit 4 = palette number
			0 = take from obj0
			1 = take from obj1
		bit 3 = ignored
		bit 2 = ignored
		bit 1 = ignored
		bit 0 = ignored
	
	sprite information starts on page 25
*/
void MemoryMap::update_sprite(uint16_t address, uint8_t value)
{
	uint16_t index = address >> 2;
	uint8_t i;
	for (i = 0; i < 0x28; i++)
	{
		if (sprites[i]->get_index() == index)
		{
			break;
		}
	}
	switch (address & 0x03)
	{
		case 0x00:
			sprites[i]->set_y(value);
			break;
		case 0x01:
			sprites[i]->set_x(value);
			break;
		case 0x02:
			sprites[i]->set_tile_number(value);
			break;
		case 0x03:
			sprites[i]->set_attribute_flags(value);
			break;
	}
}

Sprite **MemoryMap::get_sprites(void)
{
	return sprites;
}
