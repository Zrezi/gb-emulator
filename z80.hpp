#ifndef Z80_H
#define Z80_H

#include <stdint.h>

#include "memorymap.hpp"

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
} registers;
// TODO: this might not work, since registers here is "global" and the macro should work on the
// registers within the Z80 instance

#define FLAGS_ZERO (1 << 7)
#define FLAGS_NEGATIVE (1 << 6)
#define FLAGS_HALFCARRY (1 << 5)
#define FLAGS_CARRY (1 << 4)

#define FLAGS_ISZERO (registers.f & FLAGS_ZERO)
#define FLAGS_ISNEGATIVE (registers.f & FLAGS_NEGATIVE)
#define FLAGS_ISCARRY (registers.f & FLAGS_CARRY)
#define FLAGS_ISHALFCARRY (registers.f & FLAGS_HALFCARRY)

#define FLAGS_ISSET(x) (registers.f & (x))
#define FLAGS_SET(x) (registers.f |= (x))
#define FLAGS_CLEAR(x) (registers.f &= ~(x))

struct instruction_t
{
	const char *name;
	uint8_t operand_length;
	void *execute;
};

extern uint8_t cycles[0x100];
extern uint8_t cb_cycles[0x100];

extern const instruction_t instructions[0x100];
extern const instruction_t cb_instructions[0x100];

class Z80
{
public:
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
	uint8_t next_instruction_operand;

	/*
		basic functions
	*/
	uint8_t increment(uint8_t value);
	uint8_t decrement(uint8_t value);
	void add_byte(uint8_t *destination, uint8_t value);
	void add_short(uint16_t *destination, uint16_t value);
	void add_carry(uint8_t value);
	void subtract_byte(uint8_t value);
	void subtract_carry(uint8_t value);
	void bitwise_and(uint8_t value);
	void bitwise_or(uint8_t value);
	void bitwise_xor(uint8_t value);
	void compare(uint8_t value);
	
	/*
		cb functions
	*/
	uint8_t rlc(uint8_t value);
	uint8_t rrc(uint8_t value);
	uint8_t rl(uint8_t value);
	uint8_t rr(uint8_t value);
	uint8_t sla(uint8_t value);
	uint8_t sra(uint8_t value);
	uint8_t swap(uint8_t value);
	uint8_t srl(uint8_t value);
	void bit(uint8_t bit, uint8_t value);
	uint8_t set(uint8_t bit, uint8_t value);

	/*
		opcode functions
	*/
	void unknown(void);

	void nop(void);                       // 0x00
	void ld_bc_nn(uint16_t operand);      // 0x01
	void ld_bcp_a(void);                  // 0x02
	void inc_bc(void);                    // 0x03
	void inc_b(void);                     // 0x04
	void dec_b(void);                     // 0x05
	void ld_b_n(uint8_t operand);         // 0x06
	void rlca(void);                      // 0x07
	void ld_nnp_sp(uint16_t operand);     // 0x08
	void add_hl_bc(void);                 // 0x09
	void ld_a_bcp(void);                  // 0x0A
	void dec_bc(void);                    // 0x0B
	void inc_c(void);                     // 0x0C
	void dec_c(void);                     // 0x0D
	void ld_c_n(uint8_t operand);         // 0x0E
	void rrca(void);                      // 0x0F
	
	void stop(uint8_t operand);                      // 0x10
	void ld_de_nn(uint16_t operand);      // 0x11
	void ld_dep_a(void);                  // 0x12
	void inc_de(void);                    // 0x13
	void inc_d(void);                     // 0x14
	void dec_d(void);                     // 0x15
	void ld_d_n(uint8_t operand);         // 0x16
	void rla(void);                       // 0x17
	void jr_n(uint8_t operand);           // 0x18
	void add_hl_de(void);                 // 0x19
	void ld_a_dep(void);                  // 0x1A
	void dec_de(void);                    // 0x1B
	void inc_e(void);                     // 0x1C
	void dec_e(void);                     // 0x1D
	void ld_e_n(uint8_t operand);         // 0x1E
	void rra(void);                       // 0x1F
	
	void jr_nz_n(uint8_t operand);        // 0x20
	void ld_hl_nn(uint16_t operand);      // 0x21
	void ldi_hlp_a(void);                 // 0x22
	void inc_hl(void);                    // 0x23
	void inc_h(void);                     // 0x24
	void dec_h(void);                     // 0x25
	void ld_h_n(uint8_t operand);         // 0x26
	void daa(void);                       // 0x27
	void jr_z_n(uint8_t operand);         // 0x28
	void add_hl_hl(void);                 // 0x29
	void ldi_a_hlp(void);                 // 0x2A
	void dec_hl(void);                    // 0x2B
	void inc_l(void);                     // 0x2C
	void dec_l(void);                     // 0x2D
	void ld_l_n(uint8_t operand);         // 0x2E
	void cpl(void);                       // 0x2F
	
	void jr_nc_n(uint8_t operand);        // 0x30
	void ld_sp_nn(uint16_t operand);      // 0x31
	void ldd_hlp_a(void);                 // 0x32
	void inc_sp(void);                    // 0x33
	void inc_hlp(void);                   // 0x34
	void dec_hlp(void);                   // 0x35
	void ld_hlp_n(uint8_t operand);       // 0x36
	void scf(void);                       // 0x37
	void jr_c_n(uint8_t operand);         // 0x38
	void add_hl_sp(void);                 // 0x39
	void ldd_a_hlp(void);                 // 0x3A
	void dec_sp(void);                    // 0x3B
	void inc_a(void);                     // 0x3C
	void dec_a(void);                     // 0x3D
	void ld_a_n(uint8_t operand);         // 0x3E
	void ccf(void);                       // 0x3F
	
	/*     Load B into B              */  // 0x40
	void ld_b_c(void);                    // 0x41
	void ld_b_d(void);                    // 0x42
	void ld_b_e(void);                    // 0x43
	void ld_b_h(void);                    // 0x44
	void ld_b_l(void);                    // 0x45
	void ld_b_hlp(void);                  // 0x46
	void ld_b_a(void);                    // 0x47
	void ld_c_b(void);                    // 0x48
	/*     Load C into C              */  // 0x49
	void ld_c_d(void);                    // 0x4A
	void ld_c_e(void);                    // 0x4B
	void ld_c_h(void);                    // 0x4C
	void ld_c_l(void);                    // 0x4D
	void ld_c_hlp(void);                  // 0x4E
	void ld_c_a(void);                    // 0x4F
	
	void ld_d_b(void);                    // 0x50
	void ld_d_c(void);                    // 0x51
	/*     Load D into D              */  // 0x52
	void ld_d_e(void);                    // 0x53
	void ld_d_h(void);                    // 0x54
	void ld_d_l(void);                    // 0x55
	void ld_d_hlp(void);                  // 0x56
	void ld_d_a(void);                    // 0x57
	void ld_e_b(void);                    // 0x58
	void ld_e_c(void);                    // 0x59
	void ld_e_d(void);                    // 0x5A
	/*     Load E into E              */  // 0x5B
	void ld_e_h(void);                    // 0x5C
	void ld_e_l(void);                    // 0x5D
	void ld_e_hlp(void);                  // 0x5E
	void ld_e_a(void);                    // 0x5F
	
	void ld_h_b(void);                    // 0x60
	void ld_h_c(void);                    // 0x61
	void ld_h_d(void);                    // 0x62
	void ld_h_e(void);                    // 0x63
	/*     Load H into H              */  // 0x64
	void ld_h_l(void);                    // 0x65
	void ld_h_hlp(void);                  // 0x66
	void ld_h_a(void);                    // 0x67
	void ld_l_b(void);                    // 0x68
	void ld_l_c(void);                    // 0x69
	void ld_l_d(void);                    // 0x6A
	void ld_l_e(void);                    // 0x6B
	void ld_l_h(void);                    // 0x6C
	/*     Load L into L              */  // 0x6D
	void ld_l_hlp(void);                  // 0x6E
	void ld_l_a(void);                    // 0x6F
	
	void ld_hlp_b(void);                  // 0x70
	void ld_hlp_c(void);                  // 0x71
	void ld_hlp_d(void);                  // 0x72
	void ld_hlp_e(void);                  // 0x73
	void ld_hlp_h(void);                  // 0x74
	void ld_hlp_l(void);                  // 0x75
	void halt(void);                      // 0x76
	void ld_hlp_a(void);                  // 0x77
	void ld_a_b(void);                    // 0x78
	void ld_a_c(void);                    // 0x79
	void ld_a_d(void);                    // 0x7A
	void ld_a_e(void);                    // 0x7B
	void ld_a_h(void);                    // 0x7C
	void ld_a_l(void);                    // 0x7D
	void ld_a_hlp(void);                  // 0x7E
	/*     Load A into A              */  // 0x7F
	
	void add_a_b(void);                   // 0x80
	void add_a_c(void);                   // 0x81
	void add_a_d(void);                   // 0x82
	void add_a_e(void);                   // 0x83
	void add_a_h(void);                   // 0x84
	void add_a_l(void);                   // 0x85
	void add_a_hlp(void);                 // 0x86
	void add_a_a(void);                   // 0x87
	void adc_b(void);                     // 0x88
	void adc_c(void);                     // 0x89
	void adc_d(void);                     // 0x8A
	void adc_e(void);                     // 0x8B
	void adc_h(void);                     // 0x8C
	void adc_l(void);                     // 0x8D
	void adc_hlp(void);                   // 0x8E
	void adc_a(void);                     // 0x8F
	
	void sub_b(void);                     // 0x90
	void sub_c(void);                     // 0x91
	void sub_d(void);                     // 0x92
	void sub_e(void);                     // 0x93
	void sub_h(void);                     // 0x94
	void sub_l(void);                     // 0x95
	void sub_hlp(void);                   // 0x96
	void sub_a(void);                     // 0x97
	void sbc_b(void);                     // 0x98
	void sbc_c(void);                     // 0x99
	void sbc_d(void);                     // 0x9A
	void sbc_e(void);                     // 0x9B
	void sbc_h(void);                     // 0x9C
	void sbc_l(void);                     // 0x9D
	void sbc_hlp(void);                   // 0x9E
	void sbc_a(void);                     // 0x9F
	
	void and_b(void);                     // 0xA0
	void and_c(void);                     // 0xA1
	void and_d(void);                     // 0xA2
	void and_e(void);                     // 0xA3
	void and_h(void);                     // 0xA4
	void and_l(void);                     // 0xA5
	void and_hlp(void);                   // 0xA6
	void and_a(void);                     // 0xA7
	void xor_b(void);                     // 0xA8
	void xor_c(void);                     // 0xA9
	void xor_d(void);                     // 0xAA
	void xor_e(void);                     // 0xAB
	void xor_h(void);                     // 0xAC
	void xor_l(void);                     // 0xAD
	void xor_hlp(void);                   // 0xAE
	void xor_a(void);                     // 0xAF
	
	void or_b(void);                      // 0xB0
	void or_c(void);                      // 0xB1
	void or_d(void);                      // 0xB2
	void or_e(void);                      // 0xB3
	void or_h(void);                      // 0xB4
	void or_l(void);                      // 0xB5
	void or_hlp(void);                    // 0xB6
	void or_a(void);                      // 0xB7
	void cp_b(void);                      // 0xB8
	void cp_c(void);                      // 0xB9
	void cp_d(void);                      // 0xBA
	void cp_e(void);                      // 0xBB
	void cp_h(void);                      // 0xBC
	void cp_l(void);                      // 0xBD
	void cp_hlp(void);                    // 0xBE
	void cp_a(void);                      // 0xBF
	
	void ret_nz(void);                    // 0xC0
	void pop_bc(void);                    // 0xC1
	void jp_nz_nn(uint16_t operand);      // 0xC2
	void jp_nn(uint16_t operand);         // 0xC3
	void call_nz_nn(uint16_t operand);    // 0xC4
	void push_bc(void);                   // 0xC5
	void add_a_n(uint8_t operand);        // 0xC6
	void rst_0(void);                     // 0xC7
	void ret_z(void);                     // 0xC8
	void ret(void);                       // 0xC9
	void jp_z_nn(uint16_t operand);       // 0xCA
	void cb_n(uint8_t operand);           // 0xCB
	void call_z_nn(uint16_t operand);     // 0xCC
	void call_nn(uint16_t operand);       // 0xCD
	void adc_n(uint8_t operand);          // 0xCE
	void rst_08(void);                    // 0xCF
	
	void ret_nc(void);                    // 0xD0
	void pop_de(void);                    // 0xD1
	void jp_nc_nn(uint16_t operand);      // 0xD2
	/*     Unknown                    */  // 0xD3
	void call_nc_nn(uint16_t operand);    // 0xD4
	void push_de(void);                   // 0xD5
	void sub_n(uint8_t operand);          // 0xD6
	void rst_10(void);                    // 0xD7
	void ret_c(void);                     // 0xD8
	void reti(void);                      // 0xD9
	void jp_c_nn(uint16_t operand);       // 0xDA
	/*     Unknown                    */  // 0xDB
	void call_c_nn(uint16_t operand);     // 0xDC
	/*     Unknown                    */  // 0xDD
	void sbc_n(uint8_t operand);          // 0xDE
	void rst_18(void);                    // 0xDF
	
	void ld_ff_n_ap(uint8_t operand);     // 0xE0
	void pop_hl(void);                    // 0xE1
	void ld_ff_c_a(void);                 // 0xE2
	/*     Unknown                    */  // 0xE3
	/*     Unknown                    */  // 0xE4
	void push_hl(void);                   // 0xE5
	void and_n(uint8_t operand);          // 0xE6
	void rst_20(void);                    // 0xE7
	void add_sp_n(uint8_t operand);       // 0xE8
	void jp_hl(void);                     // 0xE9
	void ld_nnp_a(uint16_t operand);      // 0xEA
	/*     Unknown                    */  // 0xEB
	/*     Unknown                    */  // 0xEC
	/*     Unknown                    */  // 0xED
	void xor_n(uint8_t operand);          // 0xEE
	void rst_28(void);                    // 0xEF
	
	void ld_ff_ap_n(uint8_t operand);     // 0xF0
	void pop_af(void);                    // 0xF1
	void ld_a_ff_c(void);                 // 0xF2
	void di_inst(void);                   // 0xF3
	/*     Unknown                    */  // 0xF4
	void push_af(void);                   // 0xF5
	void or_n(uint8_t operand);           // 0xF6
	void rst_30(void);                    // 0xF7
	void ld_hl_sp_n(uint8_t operand);     // 0xF8
	void ld_sp_hl(void);                  // 0xF9
	void ld_a_nnp(uint16_t operand);      // 0xFA
	void ei(void);                        // 0xFB
	/*     Unknown                    */  // 0xFC
	/*     Unknown                    */  // 0xFD
	void cp_n(uint8_t operand);           // 0xFE
	void rst_38(void);                    // 0xFF
	
	void cb_rlc_b(void);                  // 0x00
	void cb_rlc_c(void);                  // 0x01
	void cb_rlc_d(void);                  // 0x02
	void cb_rlc_e(void);                  // 0x03
	void cb_rlc_h(void);                  // 0x04
	void cb_rlc_l(void);                  // 0x05
	void cb_rlc_hlp(void);                // 0x06
	void cb_rlc_a(void);                  // 0x07
	void cb_rrc_b(void);                  // 0x08
	void cb_rrc_c(void);                  // 0x09
	void cb_rrc_d(void);                  // 0x0A
	void cb_rrc_e(void);                  // 0x0B
	void cb_rrc_h(void);                  // 0x0C
	void cb_rrc_l(void);                  // 0x0D
	void cb_rrc_hlp(void);                // 0x0E
	void cb_rrc_a(void);                  // 0x0F
	
	void cb_rl_b(void);                   // 0x10
	void cb_rl_c(void);                   // 0x11
	void cb_rl_d(void);                   // 0x12
	void cb_rl_e(void);                   // 0x13
	void cb_rl_h(void);                   // 0x14
	void cb_rl_l(void);                   // 0x15
	void cb_rl_hlp(void);                 // 0x16
	void cb_rl_a(void);                   // 0x17
	void cb_rr_b(void);                   // 0x18
	void cb_rr_c(void);                   // 0x19
	void cb_rr_d(void);                   // 0x1A
	void cb_rr_e(void);                   // 0x1B
	void cb_rr_h(void);                   // 0x1C
	void cb_rr_l(void);                   // 0x1D
	void cb_rr_hlp(void);                 // 0x1E
	void cb_rr_a(void);                   // 0x1F
	
	void cb_sla_b(void);                  // 0x20
	void cb_sla_c(void);                  // 0x21
	void cb_sla_d(void);                  // 0x22
	void cb_sla_e(void);                  // 0x23
	void cb_sla_h(void);                  // 0x24
	void cb_sla_l(void);                  // 0x25
	void cb_sla_hlp(void);                // 0x26
	void cb_sla_a(void);                  // 0x27
	void cb_sra_b(void);                  // 0x28
	void cb_sra_c(void);                  // 0x29
	void cb_sra_d(void);                  // 0x2A
	void cb_sra_e(void);                  // 0x2B
	void cb_sra_h(void);                  // 0x2C
	void cb_sra_l(void);                  // 0x2D
	void cb_sra_hlp(void);                // 0x2E
	void cb_sra_a(void);                  // 0x2F
	
	void cb_swap_b(void);                 // 0x30
	void cb_swap_c(void);                 // 0x31
	void cb_swap_d(void);                 // 0x32
	void cb_swap_e(void);                 // 0x33
	void cb_swap_h(void);                 // 0x34
	void cb_swap_l(void);                 // 0x35
	void cb_swap_hlp(void);               // 0x36
	void cb_swap_a(void);                 // 0x37
	void cb_srl_b(void);                  // 0x38
	void cb_srl_c(void);                  // 0x39
	void cb_srl_d(void);                  // 0x3A
	void cb_srl_e(void);                  // 0x3B
	void cb_srl_h(void);                  // 0x3C
	void cb_srl_l(void);                  // 0x3D
	void cb_srl_hlp(void);                // 0x3E
	void cb_srl_a(void);                  // 0x3F
	
	void cb_bit_0_b(void);                // 0x40
	void cb_bit_0_c(void);                // 0x41
	void cb_bit_0_d(void);                // 0x42
	void cb_bit_0_e(void);                // 0x43
	void cb_bit_0_h(void);                // 0x44
	void cb_bit_0_l(void);                // 0x45
	void cb_bit_0_hlp(void);              // 0x46
	void cb_bit_0_a(void);                // 0x47
	void cb_bit_1_b(void);                // 0x48
	void cb_bit_1_c(void);                // 0x49
	void cb_bit_1_d(void);                // 0x4A
	void cb_bit_1_e(void);                // 0x4B
	void cb_bit_1_h(void);                // 0x4C
	void cb_bit_1_l(void);                // 0x4D
	void cb_bit_1_hlp(void);              // 0x4E
	void cb_bit_1_a(void);                // 0x4F
	
	void cb_bit_2_b(void);                // 0x50
	void cb_bit_2_c(void);                // 0x51
	void cb_bit_2_d(void);                // 0x52
	void cb_bit_2_e(void);                // 0x53
	void cb_bit_2_h(void);                // 0x54
	void cb_bit_2_l(void);                // 0x55
	void cb_bit_2_hlp(void);              // 0x56
	void cb_bit_2_a(void);                // 0x57
	void cb_bit_3_b(void);                // 0x58
	void cb_bit_3_c(void);                // 0x59
	void cb_bit_3_d(void);                // 0x5A
	void cb_bit_3_e(void);                // 0x5B
	void cb_bit_3_h(void);                // 0x5C
	void cb_bit_3_l(void);                // 0x5D
	void cb_bit_3_hlp(void);              // 0x5E
	void cb_bit_3_a(void);                // 0x5F
	
	void cb_bit_4_b(void);                // 0x60
	void cb_bit_4_c(void);                // 0x61
	void cb_bit_4_d(void);                // 0x62
	void cb_bit_4_e(void);                // 0x63
	void cb_bit_4_h(void);                // 0x64
	void cb_bit_4_l(void);                // 0x65
	void cb_bit_4_hlp(void);              // 0x66
	void cb_bit_4_a(void);                // 0x67
	void cb_bit_5_b(void);                // 0x68
	void cb_bit_5_c(void);                // 0x69
	void cb_bit_5_d(void);                // 0x6A
	void cb_bit_5_e(void);                // 0x6B
	void cb_bit_5_h(void);                // 0x6C
	void cb_bit_5_l(void);                // 0x6D
	void cb_bit_5_hlp(void);              // 0x6E
	void cb_bit_5_a(void);                // 0x6F
	
	void cb_bit_6_b(void);                // 0x70
	void cb_bit_6_c(void);                // 0x71
	void cb_bit_6_d(void);                // 0x72
	void cb_bit_6_e(void);                // 0x73
	void cb_bit_6_h(void);                // 0x74
	void cb_bit_6_l(void);                // 0x75
	void cb_bit_6_hlp(void);              // 0x76
	void cb_bit_6_a(void);                // 0x77
	void cb_bit_7_b(void);                // 0x78
	void cb_bit_7_c(void);                // 0x79
	void cb_bit_7_d(void);                // 0x7A
	void cb_bit_7_e(void);                // 0x7B
	void cb_bit_7_h(void);                // 0x7C
	void cb_bit_7_l(void);                // 0x7D
	void cb_bit_7_hlp(void);              // 0x7E
	void cb_bit_7_a(void);                // 0x7F
	
	void cb_res_0_b(void);                // 0x80
	void cb_res_0_c(void);                // 0x81
	void cb_res_0_d(void);                // 0x82
	void cb_res_0_e(void);                // 0x83
	void cb_res_0_h(void);                // 0x84
	void cb_res_0_l(void);                // 0x85
	void cb_res_0_hlp(void);              // 0x86
	void cb_res_0_a(void);                // 0x87
	void cb_res_1_b(void);                // 0x88
	void cb_res_1_c(void);                // 0x89
	void cb_res_1_d(void);                // 0x8A
	void cb_res_1_e(void);                // 0x8B
	void cb_res_1_h(void);                // 0x8C
	void cb_res_1_l(void);                // 0x8D
	void cb_res_1_hlp(void);              // 0x8E
	void cb_res_1_a(void);                // 0x8F
	
	void cb_res_2_b(void);                // 0x90
	void cb_res_2_c(void);                // 0x91
	void cb_res_2_d(void);                // 0x92
	void cb_res_2_e(void);                // 0x93
	void cb_res_2_h(void);                // 0x94
	void cb_res_2_l(void);                // 0x95
	void cb_res_2_hlp(void);              // 0x96
	void cb_res_2_a(void);                // 0x97
	void cb_res_3_b(void);                // 0x98
	void cb_res_3_c(void);                // 0x99
	void cb_res_3_d(void);                // 0x9A
	void cb_res_3_e(void);                // 0x9B
	void cb_res_3_h(void);                // 0x9C
	void cb_res_3_l(void);                // 0x9D
	void cb_res_3_hlp(void);              // 0x9E
	void cb_res_3_a(void);                // 0x9F
	
	void cb_res_4_b(void);                // 0xA0
	void cb_res_4_c(void);                // 0xA1
	void cb_res_4_d(void);                // 0xA2
	void cb_res_4_e(void);                // 0xA3
	void cb_res_4_h(void);                // 0xA4
	void cb_res_4_l(void);                // 0xA5
	void cb_res_4_hlp(void);              // 0xA6
	void cb_res_4_a(void);                // 0xA7
	void cb_res_5_b(void);                // 0xA8
	void cb_res_5_c(void);                // 0xA9
	void cb_res_5_d(void);                // 0xAA
	void cb_res_5_e(void);                // 0xAB
	void cb_res_5_h(void);                // 0xAC
	void cb_res_5_l(void);                // 0xAD
	void cb_res_5_hlp(void);              // 0xAE
	void cb_res_5_a(void);                // 0xAF
	
	void cb_res_6_b(void);                // 0xB0
	void cb_res_6_c(void);                // 0xB1
	void cb_res_6_d(void);                // 0xB2
	void cb_res_6_e(void);                // 0xB3
	void cb_res_6_h(void);                // 0xB4
	void cb_res_6_l(void);                // 0xB5
	void cb_res_6_hlp(void);              // 0xB6
	void cb_res_6_a(void);                // 0xB7
	void cb_res_7_b(void);                // 0xB8
	void cb_res_7_c(void);                // 0xB9
	void cb_res_7_d(void);                // 0xBA
	void cb_res_7_e(void);                // 0xBB
	void cb_res_7_h(void);                // 0xBC
	void cb_res_7_l(void);                // 0xBD
	void cb_res_7_hlp(void);              // 0xBE
	void cb_res_7_a(void);                // 0xBF
	
	void cb_set_0_b(void);                // 0xC0
	void cb_set_0_c(void);                // 0xC1
	void cb_set_0_d(void);                // 0xC2
	void cb_set_0_e(void);                // 0xC3
	void cb_set_0_h(void);                // 0xC4
	void cb_set_0_l(void);                // 0xC5
	void cb_set_0_hlp(void);              // 0xC6
	void cb_set_0_a(void);                // 0xC7
	void cb_set_1_b(void);                // 0xC8
	void cb_set_1_c(void);                // 0xC9
	void cb_set_1_d(void);                // 0xCA
	void cb_set_1_e(void);                // 0xCB
	void cb_set_1_h(void);                // 0xCC
	void cb_set_1_l(void);                // 0xCD
	void cb_set_1_hlp(void);              // 0xCE
	void cb_set_1_a(void);                // 0xCF
	
	void cb_set_2_b(void);                // 0xD0
	void cb_set_2_c(void);                // 0xD1
	void cb_set_2_d(void);                // 0xD2
	void cb_set_2_e(void);                // 0xD3
	void cb_set_2_h(void);                // 0xD4
	void cb_set_2_l(void);                // 0xD5
	void cb_set_2_hlp(void);              // 0xD6
	void cb_set_2_a(void);                // 0xD7
	void cb_set_3_b(void);                // 0xD8
	void cb_set_3_c(void);                // 0xD9
	void cb_set_3_d(void);                // 0xDA
	void cb_set_3_e(void);                // 0xDB
	void cb_set_3_h(void);                // 0xDC
	void cb_set_3_l(void);                // 0xDD
	void cb_set_3_hlp(void);              // 0xDE
	void cb_set_3_a(void);                // 0xDF
	
	void cb_set_4_b(void);                // 0xE0
	void cb_set_4_c(void);                // 0xE1
	void cb_set_4_d(void);                // 0xE2
	void cb_set_4_e(void);                // 0xE3
	void cb_set_4_h(void);                // 0xE4
	void cb_set_4_l(void);                // 0xE5
	void cb_set_4_hlp(void);              // 0xE6
	void cb_set_4_a(void);                // 0xE7
	void cb_set_5_b(void);                // 0xE8
	void cb_set_5_c(void);                // 0xE9
	void cb_set_5_d(void);                // 0xEA
	void cb_set_5_e(void);                // 0xEB
	void cb_set_5_h(void);                // 0xEC
	void cb_set_5_l(void);                // 0xED
	void cb_set_5_hlp(void);              // 0xEE
	void cb_set_5_a(void);                // 0xEF
	
	void cb_set_6_b(void);                // 0xF0
	void cb_set_6_c(void);                // 0xF1
	void cb_set_6_d(void);                // 0xF2
	void cb_set_6_e(void);                // 0xF3
	void cb_set_6_h(void);                // 0xF4
	void cb_set_6_l(void);                // 0xF5
	void cb_set_6_hlp(void);              // 0xF6
	void cb_set_6_a(void);                // 0xF7
	void cb_set_7_b(void);                // 0xF8
	void cb_set_7_c(void);                // 0xF9
	void cb_set_7_d(void);                // 0xFA
	void cb_set_7_e(void);                // 0xFB
	void cb_set_7_h(void);                // 0xFC
	void cb_set_7_l(void);                // 0xFD
	void cb_set_7_hlp(void);              // 0xFE
	void cb_set_7_a(void);                // 0xFF
	
	Z80();
	~Z80();

	MemoryMap *get_memory(void);
	void init();

	uint16_t execute_one(void);
	
};

#endif