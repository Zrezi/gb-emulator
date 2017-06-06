#include <iostream>
#include <stdint.h>

#include "z80.hpp"
#include "main.hpp"

uint8_t cycles[0x100] = {
	 4, 12,  8,  8,  4,  4,  8,  4, 20,  8,  8,  8,  4,  4,  8,  4,
	 4, 12,  8,  8,  4,  4,  8,  4, 12,  8,  8,  8,  4,  4,  8,  4,
	12, 12,  8,  8,  4,  4,  8,  4, 12,  8,  8,  8,  4,  4,  8,  4,
	12, 12,  8,  8, 12, 12, 12,  4, 12,  8,  8,  8,  4,  4,  8,  4,

	 4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	 4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	 4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	 8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4,  4,  4,  4,  8,  4,

	 4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	 4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	 4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,
	 4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4,  4,  4,  4,  8,  4,

	20, 12, 16, 16, 24, 16,  8, 16, 20, 16, 16,  0, 24, 24,  8, 16,
	20, 12, 16,  0, 24, 16,  8, 16, 20, 16, 16,  0, 24,  0,  8, 16,
	12, 12,  8,  0,  0, 16,  8, 16, 16,  4, 16,  0,  0,  0,  8, 16,
	12, 12,  8,  4,  0, 16,  8, 16, 12,  8, 16,  4,  0,  0,  8, 16
};

uint8_t cb_cycles[0x100] = {
	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,
	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,
	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,
	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,

	 8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8,
	 8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8,
	 8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8,
	 8,  8,  8,  8,  8,  8, 12,  8,  8,  8,  8,  8,  8,  8, 12,  8,

	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,
	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,
	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,
	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,

	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,
	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,
	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8,
	 8,  8,  8,  8,  8,  8, 16,  8,  8,  8,  8,  8,  8,  8, 16,  8
};

const instruction_t instructions[0x100] = {
	{ "No Op",                       0, &Z80::nop          }, // 0x00
	{ "Load nn -> BC",               2, &Z80::ld_bc_nn     }, // 0x01
	{ "Load A -> (BC)",              0, &Z80::ld_bcp_a     }, // 0x02
	{ "Increment BC",                0, &Z80::inc_bc       }, // 0x03
	{ "Increment B",                 0, &Z80::inc_b        }, // 0x04
	{ "Decrement B",                 0, &Z80::dec_b        }, // 0x05
	{ "Load n -> B",                 1, &Z80::ld_b_n       }, // 0x06
	{ "Rotate A Left",               0, &Z80::rlca         }, // 0x07
	{ "Load SP -> (nn)",             2, &Z80::ld_nnp_sp    }, // 0x08
	{ "HL + BC -> HL",               0, &Z80::add_hl_bc    }, // 0x09
	{ "Load (BC) -> A",              0, &Z80::ld_a_bcp     }, // 0x0A
	{ "Decrement BC",                0, &Z80::dec_bc       }, // 0x0B
	{ "Increment C",                 0, &Z80::inc_c        }, // 0x0C
	{ "Decrement C",                 0, &Z80::dec_c        }, // 0x0D
	{ "Load n -> C",                 1, &Z80::ld_c_n       }, // 0x0E
	{ "Rotate A Right",              0, &Z80::rrca         }, // 0x0F

	{ "Stop",                        1, &Z80::stop         }, // 0x10
	{ "Load nn -> DE",               2, &Z80::ld_de_nn     }, // 0x11
	{ "Load A -> (DE)",              0, &Z80::ld_dep_a     }, // 0x12
	{ "Increment DE",                0, &Z80::inc_de       }, // 0x13
	{ "Increment D",                 0, &Z80::inc_d        }, // 0x14
	{ "Decrement D",                 0, &Z80::dec_d        }, // 0x15
	{ "Load n -> D",                 1, &Z80::ld_d_n       }, // 0x16
	{ "Rotate Left Thru Carry",      0, &Z80::rla          }, // 0x17
	{ "Jump",                        1, &Z80::jr_n         }, // 0x18
	{ "HL + DE -> HL",               0, &Z80::add_hl_de    }, // 0x19
	{ "Load (DE) -> A",              0, &Z80::ld_a_dep     }, // 0x1A
	{ "Decrement DE",                0, &Z80::dec_de       }, // 0x1B
	{ "Increment E",                 0, &Z80::inc_e        }, // 0x1C
	{ "Decrement E",                 0, &Z80::dec_e        }, // 0x1D
	{ "Load n -> E",                 1, &Z80::ld_e_n       }, // 0x1E
	{ "Rotate Right Thru Carry",     0, &Z80::rra          }, // 0x1F

	{ "Jump If Zero Flag Reset",     1, &Z80::jr_nz_n      }, // 0x20
	{ "Load nn -> HL",               2, &Z80::ld_hl_nn     }, // 0x21
	{ "Load A -> (HL), HL++",        0, &Z80::ldi_hlp_a    }, // 0x22
	{ "Increment HL",                0, &Z80::inc_hl       }, // 0x23
	{ "Increment H",                 0, &Z80::inc_h        }, // 0x24
	{ "Decrement H",                 0, &Z80::dec_h        }, // 0x25
	{ "Load n -> H",                 1, &Z80::ld_h_n       }, // 0x26
	{ "Decimal Adjust A",            0, &Z80::daa          }, // 0x27
	{ "Jump If Zero Flag Set",       1, &Z80::jr_z_n       }, // 0x28
	{ "HL + HL -> HL",               0, &Z80::add_hl_hl    }, // 0x29
	{ "Load (HL) -> A",              0, &Z80::ldi_a_hlp    }, // 0x2A
	{ "Decrement HL",                0, &Z80::dec_hl       }, // 0x2B
	{ "Increment L",                 0, &Z80::inc_l        }, // 0x2C
	{ "Decrement L",                 0, &Z80::dec_l        }, // 0x2D
	{ "Load n -> L",                 1, &Z80::ld_l_n       }, // 0x2E
	{ "Complement A",                0, &Z80::cpl          }, // 0x2F

	{ "Jump If Carry Flag Reset",    1, &Z80::jr_nc_n      }, // 0x30
	{ "Load nn -> SP",               2, &Z80::ld_sp_nn     }, // 0x31
	{ "Load A -> (HL), HL--",        0, &Z80::ldd_hlp_a    }, // 0x32
	{ "Increment SP",                0, &Z80::inc_sp       }, // 0x33
	{ "Increment (HL)",              0, &Z80::inc_hlp      }, // 0x34
	{ "Decrement (HL)",              0, &Z80::dec_hlp      }, // 0x35
	{ "Load n -> (HL)",              1, &Z80::ld_hlp_n     }, // 0x36
	{ "Set Carry Flag",              0, &Z80::scf          }, // 0x37
	{ "Jump If Carry Flag Set",      1, &Z80::jr_c_n       }, // 0x38
	{ "HL + SP -> HL",               0, &Z80::add_hl_sp    }, // 0x39
	{ "Load (HL) -> A, HL--",        0, &Z80::ldd_a_hlp    }, // 0x3A
	{ "Decrement SP",                0, &Z80::dec_sp       }, // 0x3B
	{ "Increment A",                 0, &Z80::inc_a        }, // 0x3C
	{ "Decrement A",                 0, &Z80::dec_a        }, // 0x3D
	{ "Load n -> A",                 0, &Z80::ld_a_n       }, // 0x3E
	{ "Complement Carry Flag",       0, &Z80::ccf          }, // 0x3F

	{ "Load B -> B",                 0, &Z80::nop          }, // 0x40
	{ "Load C -> B",                 0, &Z80::ld_b_c       }, // 0x41
	{ "Load D -> B",                 0, &Z80::ld_b_d       }, // 0x42
	{ "Load E -> B",                 0, &Z80::ld_b_e       }, // 0x43
	{ "Load H -> B",                 0, &Z80::ld_b_h       }, // 0x44
	{ "Load L -> B",                 0, &Z80::ld_b_l       }, // 0x45
	{ "Load (HL) -> B",              0, &Z80::ld_b_hlp     }, // 0x46
	{ "Load A -> B",                 0, &Z80::ld_b_a       }, // 0x47
	{ "Load B -> C",                 0, &Z80::ld_c_b       }, // 0x48
	{ "Load C -> C",                 0, &Z80::nop          }, // 0x49
	{ "Load D -> C",                 0, &Z80::ld_c_d       }, // 0x4A
	{ "Load E -> C",                 0, &Z80::ld_c_e       }, // 0x4B
	{ "Load H -> C",                 0, &Z80::ld_c_h       }, // 0x4C
	{ "Load L -> C",                 0, &Z80::ld_c_l       }, // 0x4D
	{ "Load (HL) -> C",              0, &Z80::ld_c_hlp     }, // 0x4E
	{ "Load A -> C",                 0, &Z80::ld_c_a       }, // 0x4F

	{ "Load B -> D",                 0, &Z80::ld_d_b       }, // 0x50
	{ "Load C -> D",                 0, &Z80::ld_d_c       }, // 0x51
	{ "Load D -> D",                 0, &Z80::nop          }, // 0x52
	{ "Load E -> D",                 0, &Z80::ld_d_e       }, // 0x53
	{ "Load H -> D",                 0, &Z80::ld_d_h       }, // 0x54
	{ "Load L -> D",                 0, &Z80::ld_d_l       }, // 0x55
	{ "Load (HL) -> D",              0, &Z80::ld_d_hlp     }, // 0x56
	{ "Load A -> D",                 0, &Z80::ld_d_a       }, // 0x57
	{ "Load B -> E",                 0, &Z80::ld_e_b       }, // 0x58
	{ "Load C -> E",                 0, &Z80::ld_e_c       }, // 0x59
	{ "Load D -> E",                 0, &Z80::ld_e_d       }, // 0x5A
	{ "Load E -> E",                 0, &Z80::nop          }, // 0x5B
	{ "Load H -> E",                 0, &Z80::ld_e_h       }, // 0x5C
	{ "Load L -> E",                 0, &Z80::ld_e_l       }, // 0x5D
	{ "Load (HL) -> E",              0, &Z80::ld_e_hlp     }, // 0x5E
	{ "Load A -> E",                 0, &Z80::ld_e_a       }, // 0x5F

	{ "Load B -> H",                 0, &Z80::ld_h_b       }, // 0x60
	{ "Load C -> H",                 0, &Z80::ld_h_c       }, // 0x61
	{ "Load D -> H",                 0, &Z80::ld_h_d       }, // 0x62
	{ "Load E -> H",                 0, &Z80::ld_h_e       }, // 0x63
	{ "Load H -> H",                 0, &Z80::nop          }, // 0x64
	{ "Load L -> H",                 0, &Z80::ld_h_l       }, // 0x65
	{ "Load (HL) -> H",              0, &Z80::ld_h_hlp     }, // 0x66
	{ "Load A -> H",                 0, &Z80::ld_h_a       }, // 0x67
	{ "Load B -> L",                 0, &Z80::ld_l_b       }, // 0x68
	{ "Load C -> L",                 0, &Z80::ld_l_c       }, // 0x69
	{ "Load D -> L",                 0, &Z80::ld_l_d       }, // 0x6A
	{ "Load E -> L",                 0, &Z80::ld_l_e       }, // 0x6B
	{ "Load H -> L",                 0, &Z80::ld_l_h       }, // 0x6C
	{ "Load L -> L",                 0, &Z80::nop          }, // 0x6D
	{ "Load (HL) -> L",              0, &Z80::ld_l_hlp     }, // 0x6E
	{ "Load A -> L",                 0, &Z80::ld_l_a       }, // 0x6F

	{ "Load B -> (HL)",              0, &Z80::ld_hlp_b     }, // 0x70
	{ "Load C -> (HL)",              0, &Z80::ld_hlp_c     }, // 0x71
	{ "Load D -> (HL)",              0, &Z80::ld_hlp_d     }, // 0x72
	{ "Load E -> (HL)",              0, &Z80::ld_hlp_e     }, // 0x73
	{ "Load H -> (HL)",              0, &Z80::ld_hlp_h     }, // 0x74
	{ "Load L -> (HL)",              0, &Z80::ld_hlp_l     }, // 0x75
	{ "Halt",                        0, &Z80::halt         }, // 0x76
	{ "Load A -> (HL)",              0, &Z80::ld_hlp_a     }, // 0x77
	{ "Load B -> A",                 0, &Z80::ld_a_b       }, // 0x78
	{ "Load C -> A",                 0, &Z80::ld_a_c       }, // 0x79
	{ "Load D -> A",                 0, &Z80::ld_a_d       }, // 0x7A
	{ "Load E -> A",                 0, &Z80::ld_a_e       }, // 0x7B
	{ "Load H -> A",                 0, &Z80::ld_a_h       }, // 0x7C
	{ "Load L -> A",                 0, &Z80::ld_a_l       }, // 0x7D
	{ "Load (HL) -> A",              0, &Z80::ld_a_hlp     }, // 0x7E
	{ "Load A -> A",                 0, &Z80::nop          }, // 0x7F

	{ "A += B",                      0, &Z80::add_a_b      }, // 0x80
	{ "A += C",                      0, &Z80::add_a_c      }, // 0x81
	{ "A += D",                      0, &Z80::add_a_d      }, // 0x82
	{ "A += E",                      0, &Z80::add_a_e      }, // 0x83
	{ "A += H",                      0, &Z80::add_a_h      }, // 0x84
	{ "A += L",                      0, &Z80::add_a_l      }, // 0x85
	{ "A += (HL)",                   0, &Z80::add_a_hlp    }, // 0x86
	{ "A += A",                      0, &Z80::add_a_a      }, // 0x87
	{ "A += Carry + B",              0, &Z80::adc_b        }, // 0x88
	{ "A += Carry + C",              0, &Z80::adc_c        }, // 0x89
	{ "A += Carry + D",              0, &Z80::adc_d        }, // 0x8A
	{ "A += Carry + E",              0, &Z80::adc_e        }, // 0x8B
	{ "A += Carry + H",              0, &Z80::adc_h        }, // 0x8C
	{ "A += Carry + L",              0, &Z80::adc_l        }, // 0x8D
	{ "A += Carry + (HL)",           0, &Z80::adc_hlp      }, // 0x8E
	{ "A += Carry + A",              0, &Z80::adc_a        }, // 0x8F

	{ "A -= B",                      0, &Z80::sub_b        }, // 0x90
	{ "A -= C",                      0, &Z80::sub_c        }, // 0x91
	{ "A -= D",                      0, &Z80::sub_d        }, // 0x92
	{ "A -= E",                      0, &Z80::sub_e        }, // 0x93
	{ "A -= H",                      0, &Z80::sub_h        }, // 0x94
	{ "A -= L",                      0, &Z80::sub_l        }, // 0x95
	{ "A -= (HL)",                   0, &Z80::sub_hlp      }, // 0x96
	{ "A -= A",                      0, &Z80::sub_a        }, // 0x97
	{ "A -= B + Carry",              0, &Z80::sbc_b        }, // 0x98
	{ "A -= C + Carry",              0, &Z80::sbc_c        }, // 0x99
	{ "A -= D + Carry",              0, &Z80::sbc_d        }, // 0x9A
	{ "A -= E + Carry",              0, &Z80::sbc_e        }, // 0x9B
	{ "A -= H + Carry",              0, &Z80::sbc_h        }, // 0x9C
	{ "A -= L + Carry",              0, &Z80::sbc_l        }, // 0x9D
	{ "A -= (HL) + Carry",           0, &Z80::sbc_hlp      }, // 0x9E
	{ "A -= A + Carry",              0, &Z80::sbc_a        }, // 0x9F

	{ "A &= B",                      0, &Z80::and_b        }, // 0xA0
	{ "A &= C",                      0, &Z80::and_c        }, // 0xA1
	{ "A &= D",                      0, &Z80::and_d        }, // 0xA2
	{ "A &= E",                      0, &Z80::and_e        }, // 0xA3
	{ "A &= H",                      0, &Z80::and_h        }, // 0xA4
	{ "A &= L",                      0, &Z80::and_l        }, // 0xA5
	{ "A &= (HL)",                   0, &Z80::and_hlp      }, // 0xA6
	{ "A &= A",                      0, &Z80::and_a        }, // 0xA7
	{ "A ^= B",                      0, &Z80::xor_b        }, // 0xA8
	{ "A ^= C",                      0, &Z80::xor_c        }, // 0xA9
	{ "A ^= D",                      0, &Z80::xor_d        }, // 0xAA
	{ "A ^= E",                      0, &Z80::xor_e        }, // 0xAB
	{ "A ^= H",                      0, &Z80::xor_h        }, // 0xAC
	{ "A ^= L",                      0, &Z80::xor_l        }, // 0xAD
	{ "A ^= (HL)",                   0, &Z80::xor_hlp      }, // 0xAE
	{ "A ^= A",                      0, &Z80::xor_a        }, // 0xAF

	{ "A |= B",                      0, &Z80::or_b         }, // 0xB0
	{ "A |= C",                      0, &Z80::or_c         }, // 0xB1
	{ "A |= D",                      0, &Z80::or_d         }, // 0xB2
	{ "A |= E",                      0, &Z80::or_e         }, // 0xB3
	{ "A |= H",                      0, &Z80::or_h         }, // 0xB4
	{ "A |= L",                      0, &Z80::or_l         }, // 0xB5
	{ "A |= (HL)",                   0, &Z80::or_hlp       }, // 0xB6
	{ "A |= A",                      0, &Z80::or_a         }, // 0xB7
	{ "Compare A and B",             0, &Z80::cp_b         }, // 0xB8
	{ "Compare A and C",             0, &Z80::cp_c         }, // 0xB9
	{ "Compare A and D",             0, &Z80::cp_d         }, // 0xBA
	{ "Compare A and E",             0, &Z80::cp_e         }, // 0xBB
	{ "Compare A and H",             0, &Z80::cp_h         }, // 0xBC
	{ "Compare A and L",             0, &Z80::cp_l         }, // 0xBD
	{ "Compare A and (HL)",          0, &Z80::cp_hlp       }, // 0xBE
	{ "Compare A and A",             0, &Z80::cp_a         }, // 0xBF

	{ "Return If Zero Flag Reset",   0, &Z80::ret_nz       }, // 0xC0
	{ "Pop 2 Bytes -> BC",           0, &Z80::pop_bc       }, // 0xC1
	{ "Jump If Zero Flag Reset",     2, &Z80::jp_nz_nn     }, // 0xC2
	{ "Jump",                        2, &Z80::jp_nn        }, // 0xC3
	{ "Call If Zero Flag Reset",     2, &Z80::call_nz_nn   }, // 0xC4
	{ "Push BC -> Stack",            0, &Z80::push_bc      }, // 0xC5
	{ "A += n",                      1, &Z80::add_a_n      }, // 0xC6
	{ "PC -> SP, PC = 0x0000",       0, &Z80::rst_0        }, // 0xC7
	{ "Return If Zero Flag Set",     0, &Z80::ret_z        }, // 0xC8
	{ "Return",                      0, &Z80::ret          }, // 0xC9
	{ "Jump If Zero Flag Set",       2, &Z80::jp_z_nn      }, // 0xCA
	{ "Extended Instructions",       1, &Z80::cb_n         }, // 0xCB
	{ "Call If Zero Flag Set",       2, &Z80::call_z_nn    }, // 0xCC
	{ "Call",                        2, &Z80::call_nn      }, // 0xCD
	{ "A += Carry + n",              1, &Z80::adc_n        }, // 0xCE
	{ "PC -> SP, PC = 0x0008",       0, &Z80::rst_08       }, // 0xCF

	{ "Return If Carry Flag Reset",  0, &Z80::ret_nc       }, // 0xD0
	{ "Pop 2 Bytes -> DE",           0, &Z80::pop_de       }, // 0xD1
	{ "Jump If Carry Flag Reset",    2, &Z80::jp_nc_nn     }, // 0xD2
	{ "Unknown",                     0, &Z80::unknown      }, // 0xD3
	{ "Call If Carry Flag Reset",    2, &Z80::call_nc_nn   }, // 0xD4
	{ "Push DE -> Stack",            0, &Z80::push_de      }, // 0xD5
	{ "A -= n",                      1, &Z80::sub_n        }, // 0xD6
	{ "PC -> SP, PC = 0x0010",       0, &Z80::rst_10       }, // 0xD7
	{ "Return If Carry Set",         0, &Z80::ret_c        }, // 0xD8
	{ "Return From Interrupts",      0, &Z80::reti         }, // 0xD9
	{ "Jump If Carry Flag Set",      2, &Z80::jp_c_nn      }, // 0xDA
	{ "Unknown",                     0, &Z80::unknown      }, // 0xDB
	{ "Call If Carry Flag Set",      2, &Z80::call_c_nn    }, // 0xDC
	{ "Unknown",                     0, &Z80::unknown      }, // 0xDD
	{ "A -= Carry + n",              1, &Z80::sbc_n        }, // 0xDE
	{ "PC -> SP, PC = 0x0018",       0, &Z80::rst_18       }, // 0xDF

	{ "A -> 0xFF00 + n",             1, &Z80::ld_ff_n_ap   }, // 0xE0
	{ "Pop 2 Bytes -> HL",           0, &Z80::pop_hl       }, // 0xE1
	{ "A -> 0xFF00 + C",             0, &Z80::ld_ff_c_a    }, // 0xE2
	{ "Unknown",                     0, &Z80::unknown      }, // 0xE3
	{ "Unknown",                     0, &Z80::unknown      }, // 0xE4
	{ "Push HL -> Stack",            0, &Z80::push_hl      }, // 0xE5
	{ "A &= n",                      1, &Z80::and_n        }, // 0xE6
	{ "PC -> SP, PC = 0x0020",       0, &Z80::rst_20       }, // 0xE7
	{ "SP += n",                     1, &Z80::add_sp_n     }, // 0xE8
	{ "Jump to HL",                  0, &Z80::jp_hl        }, // 0xE9
	{ "A -> (nn)",                   2, &Z80::ld_nnp_a     }, // 0xEA
	{ "Unknown",                     0, &Z80::unknown      }, // 0xEB
	{ "Unknown",                     0, &Z80::unknown      }, // 0xEC
	{ "Unknown",                     0, &Z80::unknown      }, // 0xED
	{ "A ^= n",                      1, &Z80::xor_n        }, // 0xEE
	{ "PC -> SP, PC = 0x0028",       0, &Z80::rst_28       }, // 0xEF

	{ "(0xFF00 + n) -> A",           1, &Z80::ld_ff_ap_n   }, // 0xF0
	{ "Pop 2 Bytes -> AF",           0, &Z80::pop_af       }, // 0xF1
	{ "(0xFF00 + C) -> A",           0, &Z80::ld_a_ff_c    }, // 0xF2
	{ "Disable Interrupts",          0, &Z80::di_inst      }, // 0xF3
	{ "Unknown",                     0, &Z80::unknown      }, // 0xF4
	{ "Push AF -> Stack",            0, &Z80::push_af      }, // 0xF5
	{ "A |= n",                      1, &Z80::or_n         }, // 0xF6
	{ "PC -> SP, PC = 0x0030",       0, &Z80::rst_30       }, // 0xF7
	{ "SP + n -> HL",                1, &Z80::ld_hl_sp_n   }, // 0xF8
	{ "HL -> SP",                    0, &Z80::ld_sp_hl     }, // 0xF9
	{ "Load (nn) -> A",              2, &Z80::ld_a_nnp     }, // 0xFA
	{ "Enable Interrupts",           0, &Z80::ei           }, // 0xFB
	{ "Unknown",                     0, &Z80::unknown      }, // 0xFC
	{ "Unknown",                     0, &Z80::unknown      }, // 0xFD
	{ "Compare A and n",             1, &Z80::cp_n         }, // 0xFE
	{ "PC -> SP, PC = 0x0038",       0, &Z80::rst_38       }  // 0xFF
};

const instruction_t cb_instructions[0x100] = {
	{ "Rotate B Left, Carry 7",      0, &Z80::cb_rlc_b     }, // 0x00
	{ "Rotate C Left, Carry 7",      0, &Z80::cb_rlc_c     }, // 0x01
	{ "Rotate D Left, Carry 7",      0, &Z80::cb_rlc_d     }, // 0x02
	{ "Rotate E Left, Carry 7",      0, &Z80::cb_rlc_e     }, // 0x03
	{ "Rotate H Left, Carry 7",      0, &Z80::cb_rlc_h     }, // 0x04
	{ "Rotate L Left, Carry 7",      0, &Z80::cb_rlc_l     }, // 0x05
	{ "Rotate (HL) Left, Carry 7",   0, &Z80::cb_rlc_hlp   }, // 0x06
	{ "Rotate A Left, Carry 7",      0, &Z80::cb_rlc_a     }, // 0x07
	{ "Rotate B Right, Carry 7",     0, &Z80::cb_rrc_b     }, // 0x08
	{ "Rotate C Right, Carry 7",     0, &Z80::cb_rrc_c     }, // 0x09
	{ "Rotate D Right, Carry 7",     0, &Z80::cb_rrc_d     }, // 0x0A
	{ "Rotate E Right, Carry 7",     0, &Z80::cb_rrc_e     }, // 0x0B
	{ "Rotate H Right, Carry 7",     0, &Z80::cb_rrc_h     }, // 0x0C
	{ "Rotate L Right, Carry 7",     0, &Z80::cb_rrc_l     }, // 0x0D
	{ "Rotate (HL) Right, Carry 7",  0, &Z80::cb_rrc_hlp   }, // 0x0E
	{ "Rotate A Right, Carry 7",     0, &Z80::cb_rrc_a     }, // 0x0F
	
	{ "Rotate B Left",               0, &Z80::cb_rl_b      }, // 0x10
	{ "Rotate C Left",               0, &Z80::cb_rl_c      }, // 0x11
	{ "Rotate D Left",               0, &Z80::cb_rl_d      }, // 0x12
	{ "Rotate E Left",               0, &Z80::cb_rl_e      }, // 0x13
	{ "Rotate H Left",               0, &Z80::cb_rl_h      }, // 0x14
	{ "Rotate L Left",               0, &Z80::cb_rl_l      }, // 0x15
	{ "Rotate (HL) Left",            0, &Z80::cb_rl_hlp    }, // 0x16
	{ "Rotate A Left",               0, &Z80::cb_rl_a      }, // 0x17
	{ "Rotate B Right",              0, &Z80::cb_rr_b      }, // 0x18
	{ "Rotate C Right",              0, &Z80::cb_rr_c      }, // 0x19
	{ "Rotate D Right",              0, &Z80::cb_rr_d      }, // 0x1A
	{ "Rotate E Right",              0, &Z80::cb_rr_e      }, // 0x1B
	{ "Rotate H Right",              0, &Z80::cb_rr_h      }, // 0x1C
	{ "Rotate L Right",              0, &Z80::cb_rr_l      }, // 0x1D
	{ "Rotate (HL) Right",           0, &Z80::cb_rr_hlp    }, // 0x1E
	{ "Rotate A Right",              0, &Z80::cb_rr_a      }, // 0x1F
	
	{ "Shift B Left Into Carry",     0, &Z80::cb_sla_b     }, // 0x20
	{ "Shift C Left Into Carry",     0, &Z80::cb_sla_c     }, // 0x21
	{ "Shift D Left Into Carry",     0, &Z80::cb_sla_d     }, // 0x22
	{ "Shift E Left Into Carry",     0, &Z80::cb_sla_e     }, // 0x23
	{ "Shift H Left Into Carry",     0, &Z80::cb_sla_h     }, // 0x24
	{ "Shift L Left Into Carry",     0, &Z80::cb_sla_l     }, // 0x25
	{ "Shift (HL) Left Into Carry",  0, &Z80::cb_sla_hlp   }, // 0x26
	{ "Shift A Left Into Carry",     0, &Z80::cb_sla_a     }, // 0x27
	{ "Shift B Right Into Carry",    0, &Z80::cb_sra_b     }, // 0x28
	{ "Shift C Right Into Carry",    0, &Z80::cb_sra_c     }, // 0x29
	{ "Shift D Right Into Carry",    0, &Z80::cb_sra_d     }, // 0x2A
	{ "Shift E Right Into Carry",    0, &Z80::cb_sra_e     }, // 0x2B
	{ "Shift H Right Into Carry",    0, &Z80::cb_sra_h     }, // 0x2C
	{ "Shift L Right Into Carry",    0, &Z80::cb_sra_l     }, // 0x2D
	{ "Shift (HL) Right Into Carry", 0, &Z80::cb_sra_hlp   }, // 0x2E
	{ "Shift A Right Into Carry",    0, &Z80::cb_sra_a     }, // 0x2F
	
	{ "Swap High Low Bits of B",     0, &Z80::cb_swap_b    }, // 0x30
	{ "Swap High Low Bits of C",     0, &Z80::cb_swap_c    }, // 0x31
	{ "Swap High Low Bits of D",     0, &Z80::cb_swap_d    }, // 0x32
	{ "Swap High Low Bits of E",     0, &Z80::cb_swap_e    }, // 0x33
	{ "Swap High Low Bits of H",     0, &Z80::cb_swap_h    }, // 0x34
	{ "Swap High Low Bits of L",     0, &Z80::cb_swap_l    }, // 0x35
	{ "Swap High Low Bits of (HL)",  0, &Z80::cb_swap_hlp  }, // 0x36
	{ "Swap High Low Bits of A",     0, &Z80::cb_swap_a    }, // 0x37
	{ "Shift B Right Into Carry",    0, &Z80::cb_srl_b     }, // 0x38
	{ "Shift C Right Into Carry",    0, &Z80::cb_srl_c     }, // 0x39
	{ "Shift D Right Into Carry",    0, &Z80::cb_srl_d     }, // 0x3A
	{ "Shift E Right Into Carry",    0, &Z80::cb_srl_e     }, // 0x3B
	{ "Shift H Right Into Carry",    0, &Z80::cb_srl_h     }, // 0x3C
	{ "Shift L Right Into Carry",    0, &Z80::cb_srl_l     }, // 0x3D
	{ "Shift (HL) Right Into Carry", 0, &Z80::cb_srl_hlp   }, // 0x3E
	{ "Shift A Right Into Carry",    0, &Z80::cb_srl_a     }, // 0x3F
	
	{ "Test Bit 0 of B",             0, &Z80::cb_bit_0_b   }, // 0x40
	{ "Test Bit 0 of C",             0, &Z80::cb_bit_0_c   }, // 0x41
	{ "Test Bit 0 of D",             0, &Z80::cb_bit_0_d   }, // 0x42
	{ "Test Bit 0 of E",             0, &Z80::cb_bit_0_e   }, // 0x43
	{ "Test Bit 0 of H",             0, &Z80::cb_bit_0_h   }, // 0x44
	{ "Test Bit 0 of L",             0, &Z80::cb_bit_0_l   }, // 0x45
	{ "Test Bit 0 of (HL)",          0, &Z80::cb_bit_0_hlp }, // 0x46
	{ "Test Bit 0 of A",             0, &Z80::cb_bit_0_a   }, // 0x47
	{ "Test Bit 1 of B",             0, &Z80::cb_bit_1_b   }, // 0x48
	{ "Test Bit 1 of C",             0, &Z80::cb_bit_1_c   }, // 0x49
	{ "Test Bit 1 of D",             0, &Z80::cb_bit_1_d   }, // 0x4A
	{ "Test Bit 1 of E",             0, &Z80::cb_bit_1_e   }, // 0x4B
	{ "Test Bit 1 of H",             0, &Z80::cb_bit_1_h   }, // 0x4C
	{ "Test Bit 1 of L",             0, &Z80::cb_bit_1_l   }, // 0x4D
	{ "Test Bit 1 of (HL)",          0, &Z80::cb_bit_1_hlp }, // 0x4E
	{ "Test Bit 1 of A",             0, &Z80::cb_bit_1_a   }, // 0x4F
	
	{ "Test Bit 2 of B",             0, &Z80::cb_bit_2_b   }, // 0x50
	{ "Test Bit 2 of C",             0, &Z80::cb_bit_2_c   }, // 0x51
	{ "Test Bit 2 of D",             0, &Z80::cb_bit_2_d   }, // 0x52
	{ "Test Bit 2 of E",             0, &Z80::cb_bit_2_e   }, // 0x53
	{ "Test Bit 2 of H",             0, &Z80::cb_bit_2_h   }, // 0x54
	{ "Test Bit 2 of L",             0, &Z80::cb_bit_2_l   }, // 0x55
	{ "Test Bit 2 of (HL)",          0, &Z80::cb_bit_2_hlp }, // 0x56
	{ "Test Bit 2 of A",             0, &Z80::cb_bit_2_a   }, // 0x57
	{ "Test Bit 3 of B",             0, &Z80::cb_bit_3_b   }, // 0x58
	{ "Test Bit 3 of C",             0, &Z80::cb_bit_3_c   }, // 0x59
	{ "Test Bit 3 of D",             0, &Z80::cb_bit_3_d   }, // 0x5A
	{ "Test Bit 3 of E",             0, &Z80::cb_bit_3_e   }, // 0x5B
	{ "Test Bit 3 of H",             0, &Z80::cb_bit_3_h   }, // 0x5C
	{ "Test Bit 3 of L",             0, &Z80::cb_bit_3_l   }, // 0x5D
	{ "Test Bit 3 of (HL)",          0, &Z80::cb_bit_3_hlp }, // 0x5E
	{ "Test Bit 3 of A",             0, &Z80::cb_bit_3_a   }, // 0x5F
	
	{ "Test Bit 4 of B",             0, &Z80::cb_bit_4_b   }, // 0x60
	{ "Test Bit 4 of C",             0, &Z80::cb_bit_4_c   }, // 0x61
	{ "Test Bit 4 of D",             0, &Z80::cb_bit_4_d   }, // 0x62
	{ "Test Bit 4 of E",             0, &Z80::cb_bit_4_e   }, // 0x63
	{ "Test Bit 4 of H",             0, &Z80::cb_bit_4_h   }, // 0x64
	{ "Test Bit 4 of L",             0, &Z80::cb_bit_4_l   }, // 0x65
	{ "Test Bit 4 of (HL)",          0, &Z80::cb_bit_4_hlp }, // 0x66
	{ "Test Bit 4 of A",             0, &Z80::cb_bit_4_a   }, // 0x67
	{ "Test Bit 5 of B",             0, &Z80::cb_bit_5_b   }, // 0x68
	{ "Test Bit 5 of C",             0, &Z80::cb_bit_5_c   }, // 0x69
	{ "Test Bit 5 of D",             0, &Z80::cb_bit_5_d   }, // 0x6A
	{ "Test Bit 5 of E",             0, &Z80::cb_bit_5_e   }, // 0x6B
	{ "Test Bit 5 of H",             0, &Z80::cb_bit_5_h   }, // 0x6C
	{ "Test Bit 5 of L",             0, &Z80::cb_bit_5_l   }, // 0x6D
	{ "Test Bit 5 of (HL)",          0, &Z80::cb_bit_5_hlp }, // 0x6E
	{ "Test Bit 5 of A",             0, &Z80::cb_bit_5_a   }, // 0x6F
	
	{ "Test Bit 6 of B",             0, &Z80::cb_bit_6_b   }, // 0x70
	{ "Test Bit 6 of C",             0, &Z80::cb_bit_6_c   }, // 0x71
	{ "Test Bit 6 of D",             0, &Z80::cb_bit_6_d   }, // 0x72
	{ "Test Bit 6 of E",             0, &Z80::cb_bit_6_e   }, // 0x73
	{ "Test Bit 6 of H",             0, &Z80::cb_bit_6_h   }, // 0x74
	{ "Test Bit 6 of L",             0, &Z80::cb_bit_6_l   }, // 0x75
	{ "Test Bit 6 of (HL)",          0, &Z80::cb_bit_6_hlp }, // 0x76
	{ "Test Bit 6 of A",             0, &Z80::cb_bit_6_a   }, // 0x77
	{ "Test Bit 7 of B",             0, &Z80::cb_bit_7_b   }, // 0x78
	{ "Test Bit 7 of C",             0, &Z80::cb_bit_7_c   }, // 0x79
	{ "Test Bit 7 of D",             0, &Z80::cb_bit_7_d   }, // 0x7A
	{ "Test Bit 7 of E",             0, &Z80::cb_bit_7_e   }, // 0x7B
	{ "Test Bit 7 of H",             0, &Z80::cb_bit_7_h   }, // 0x7C
	{ "Test Bit 7 of L",             0, &Z80::cb_bit_7_l   }, // 0x7D
	{ "Test Bit 7 of (HL)",          0, &Z80::cb_bit_7_hlp }, // 0x7E
	{ "Test Bit 7 of A",             0, &Z80::cb_bit_7_a   }, // 0x7F
	
	{ "Reset Bit 0 of B",            0, &Z80::cb_res_0_b   }, // 0x80
	{ "Reset Bit 0 of C",            0, &Z80::cb_res_0_c   }, // 0x81
	{ "Reset Bit 0 of D",            0, &Z80::cb_res_0_d   }, // 0x82
	{ "Reset Bit 0 of E",            0, &Z80::cb_res_0_e   }, // 0x83
	{ "Reset Bit 0 of H",            0, &Z80::cb_res_0_h   }, // 0x84
	{ "Reset Bit 0 of L",            0, &Z80::cb_res_0_l   }, // 0x85
	{ "Reset Bit 0 of (HL)",         0, &Z80::cb_res_0_hlp }, // 0x86
	{ "Reset Bit 0 of A",            0, &Z80::cb_res_0_a   }, // 0x87
	{ "Reset Bit 1 of B",            0, &Z80::cb_res_1_b   }, // 0x88
	{ "Reset Bit 1 of C",            0, &Z80::cb_res_1_c   }, // 0x89
	{ "Reset Bit 1 of D",            0, &Z80::cb_res_1_d   }, // 0x8A
	{ "Reset Bit 1 of E",            0, &Z80::cb_res_1_e   }, // 0x8B
	{ "Reset Bit 1 of H",            0, &Z80::cb_res_1_h   }, // 0x8C
	{ "Reset Bit 1 of L",            0, &Z80::cb_res_1_l   }, // 0x8D
	{ "Reset Bit 1 of (HL)",         0, &Z80::cb_res_1_hlp }, // 0x8E
	{ "Reset Bit 1 of A",            0, &Z80::cb_res_1_a   }, // 0x8F
	
	{ "Reset Bit 2 of B",            0, &Z80::cb_res_2_b   }, // 0x90
	{ "Reset Bit 2 of C",            0, &Z80::cb_res_2_c   }, // 0x91
	{ "Reset Bit 2 of D",            0, &Z80::cb_res_2_d   }, // 0x92
	{ "Reset Bit 2 of E",            0, &Z80::cb_res_2_e   }, // 0x93
	{ "Reset Bit 2 of H",            0, &Z80::cb_res_2_h   }, // 0x94
	{ "Reset Bit 2 of L",            0, &Z80::cb_res_2_l   }, // 0x95
	{ "Reset Bit 2 of (HL)",         0, &Z80::cb_res_2_hlp }, // 0x96
	{ "Reset Bit 2 of A",            0, &Z80::cb_res_2_a   }, // 0x97
	{ "Reset Bit 3 of B",            0, &Z80::cb_res_3_b   }, // 0x98
	{ "Reset Bit 3 of C",            0, &Z80::cb_res_3_c   }, // 0x99
	{ "Reset Bit 3 of D",            0, &Z80::cb_res_3_d   }, // 0x9A
	{ "Reset Bit 3 of E",            0, &Z80::cb_res_3_e   }, // 0x9B
	{ "Reset Bit 3 of H",            0, &Z80::cb_res_3_h   }, // 0x9C
	{ "Reset Bit 3 of L",            0, &Z80::cb_res_3_l   }, // 0x9D
	{ "Reset Bit 3 of (HL)",         0, &Z80::cb_res_3_hlp }, // 0x9E
	{ "Reset Bit 3 of A",            0, &Z80::cb_res_3_a   }, // 0x9F
	
	{ "Reset Bit 4 of B",            0, &Z80::cb_res_4_b   }, // 0xA0
	{ "Reset Bit 4 of C",            0, &Z80::cb_res_4_c   }, // 0xA1
	{ "Reset Bit 4 of D",            0, &Z80::cb_res_4_d   }, // 0xA2
	{ "Reset Bit 4 of E",            0, &Z80::cb_res_4_e   }, // 0xA3
	{ "Reset Bit 4 of H",            0, &Z80::cb_res_4_h   }, // 0xA4
	{ "Reset Bit 4 of L",            0, &Z80::cb_res_4_l   }, // 0xA5
	{ "Reset Bit 4 of (HL)",         0, &Z80::cb_res_4_hlp }, // 0xA6
	{ "Reset Bit 4 of A",            0, &Z80::cb_res_4_a   }, // 0xA7
	{ "Reset Bit 5 of B",            0, &Z80::cb_res_5_b   }, // 0xA8
	{ "Reset Bit 5 of C",            0, &Z80::cb_res_5_c   }, // 0xA9
	{ "Reset Bit 5 of D",            0, &Z80::cb_res_5_d   }, // 0xAA
	{ "Reset Bit 5 of E",            0, &Z80::cb_res_5_e   }, // 0xAB
	{ "Reset Bit 5 of H",            0, &Z80::cb_res_5_h   }, // 0xAC
	{ "Reset Bit 5 of L",            0, &Z80::cb_res_5_l   }, // 0xAD
	{ "Reset Bit 5 of (HL)",         0, &Z80::cb_res_5_hlp }, // 0xAE
	{ "Reset Bit 5 of A",            0, &Z80::cb_res_5_a   }, // 0xAF
	
	{ "Reset Bit 6 of B",            0, &Z80::cb_res_6_b   }, // 0xB0
	{ "Reset Bit 6 of C",            0, &Z80::cb_res_6_c   }, // 0xB1
	{ "Reset Bit 6 of D",            0, &Z80::cb_res_6_d   }, // 0xB2
	{ "Reset Bit 6 of E",            0, &Z80::cb_res_6_e   }, // 0xB3
	{ "Reset Bit 6 of H",            0, &Z80::cb_res_6_h   }, // 0xB4
	{ "Reset Bit 6 of L",            0, &Z80::cb_res_6_l   }, // 0xB5
	{ "Reset Bit 6 of (HL)",         0, &Z80::cb_res_6_hlp }, // 0xB6
	{ "Reset Bit 6 of A",            0, &Z80::cb_res_6_a   }, // 0xB7
	{ "Reset Bit 7 of B",            0, &Z80::cb_res_7_b   }, // 0xB8
	{ "Reset Bit 7 of C",            0, &Z80::cb_res_7_c   }, // 0xB9
	{ "Reset Bit 7 of D",            0, &Z80::cb_res_7_d   }, // 0xBA
	{ "Reset Bit 7 of E",            0, &Z80::cb_res_7_e   }, // 0xBB
	{ "Reset Bit 7 of H",            0, &Z80::cb_res_7_h   }, // 0xBC
	{ "Reset Bit 7 of L",            0, &Z80::cb_res_7_l   }, // 0xBD
	{ "Reset Bit 7 of (HL)",         0, &Z80::cb_res_7_hlp }, // 0xBE
	{ "Reset Bit 7 of A",            0, &Z80::cb_res_7_a   }, // 0xBF
	
	{ "Set Bit 0 of B",              0, &Z80::cb_set_0_b   }, // 0xC0
	{ "Set Bit 0 of C",              0, &Z80::cb_set_0_c   }, // 0xC1
	{ "Set Bit 0 of D",              0, &Z80::cb_set_0_d   }, // 0xC2
	{ "Set Bit 0 of E",              0, &Z80::cb_set_0_e   }, // 0xC3
	{ "Set Bit 0 of H",              0, &Z80::cb_set_0_h   }, // 0xC4
	{ "Set Bit 0 of L",              0, &Z80::cb_set_0_l   }, // 0xC5
	{ "Set Bit 0 of (HL)",           0, &Z80::cb_set_0_hlp }, // 0xC6
	{ "Set Bit 0 of A",              0, &Z80::cb_set_0_a   }, // 0xC7
	{ "Set Bit 1 of B",              0, &Z80::cb_set_1_b   }, // 0xC8
	{ "Set Bit 1 of C",              0, &Z80::cb_set_1_c   }, // 0xC9
	{ "Set Bit 1 of D",              0, &Z80::cb_set_1_d   }, // 0xCA
	{ "Set Bit 1 of E",              0, &Z80::cb_set_1_e   }, // 0xCB
	{ "Set Bit 1 of H",              0, &Z80::cb_set_1_h   }, // 0xCC
	{ "Set Bit 1 of L",              0, &Z80::cb_set_1_l   }, // 0xCD
	{ "Set Bit 1 of (HL)",           0, &Z80::cb_set_1_hlp }, // 0xCE
	{ "Set Bit 1 of A",              0, &Z80::cb_set_1_a   }, // 0xCF
	
	{ "Set Bit 2 of B",              0, &Z80::cb_set_2_b   }, // 0xD0
	{ "Set Bit 2 of C",              0, &Z80::cb_set_2_c   }, // 0xD1
	{ "Set Bit 2 of D",              0, &Z80::cb_set_2_d   }, // 0xD2
	{ "Set Bit 2 of E",              0, &Z80::cb_set_2_e   }, // 0xD3
	{ "Set Bit 2 of H",              0, &Z80::cb_set_2_h   }, // 0xD4
	{ "Set Bit 2 of L",              0, &Z80::cb_set_2_l   }, // 0xD5
	{ "Set Bit 2 of (HL)",           0, &Z80::cb_set_2_hlp }, // 0xD6
	{ "Set Bit 2 of A",              0, &Z80::cb_set_2_a   }, // 0xD7
	{ "Set Bit 3 of B",              0, &Z80::cb_set_3_b   }, // 0xD8
	{ "Set Bit 3 of C",              0, &Z80::cb_set_3_c   }, // 0xD9
	{ "Set Bit 3 of D",              0, &Z80::cb_set_3_d   }, // 0xDA
	{ "Set Bit 3 of E",              0, &Z80::cb_set_3_e   }, // 0xDB
	{ "Set Bit 3 of H",              0, &Z80::cb_set_3_h   }, // 0xDC
	{ "Set Bit 3 of L",              0, &Z80::cb_set_3_l   }, // 0xDD
	{ "Set Bit 3 of (HL)",           0, &Z80::cb_set_3_hlp }, // 0xDE
	{ "Set Bit 3 of A",              0, &Z80::cb_set_3_a   }, // 0xDF
	
	{ "Set Bit 4 of B",              0, &Z80::cb_set_4_b   }, // 0xE0
	{ "Set Bit 4 of C",              0, &Z80::cb_set_4_c   }, // 0xE1
	{ "Set Bit 4 of D",              0, &Z80::cb_set_4_d   }, // 0xE2
	{ "Set Bit 4 of E",              0, &Z80::cb_set_4_e   }, // 0xE3
	{ "Set Bit 4 of H",              0, &Z80::cb_set_4_h   }, // 0xE4
	{ "Set Bit 4 of L",              0, &Z80::cb_set_4_l   }, // 0xE5
	{ "Set Bit 4 of (HL)",           0, &Z80::cb_set_4_hlp }, // 0xE6
	{ "Set Bit 4 of A",              0, &Z80::cb_set_4_a   }, // 0xE7
	{ "Set Bit 5 of B",              0, &Z80::cb_set_5_b   }, // 0xE8
	{ "Set Bit 5 of C",              0, &Z80::cb_set_5_c   }, // 0xE9
	{ "Set Bit 5 of D",              0, &Z80::cb_set_5_d   }, // 0xEA
	{ "Set Bit 5 of E",              0, &Z80::cb_set_5_e   }, // 0xEB
	{ "Set Bit 5 of H",              0, &Z80::cb_set_5_h   }, // 0xEC
	{ "Set Bit 5 of L",              0, &Z80::cb_set_5_l   }, // 0xED
	{ "Set Bit 5 of (HL)",           0, &Z80::cb_set_5_hlp }, // 0xEE
	{ "Set Bit 5 of A",              0, &Z80::cb_set_5_a   }, // 0xEF
	
	{ "Set Bit 6 of B",              0, &Z80::cb_set_6_b   }, // 0xF0
	{ "Set Bit 6 of C",              0, &Z80::cb_set_6_c   }, // 0xF1
	{ "Set Bit 6 of D",              0, &Z80::cb_set_6_d   }, // 0xF2
	{ "Set Bit 6 of E",              0, &Z80::cb_set_6_e   }, // 0xF3
	{ "Set Bit 6 of H",              0, &Z80::cb_set_6_h   }, // 0xF4
	{ "Set Bit 6 of L",              0, &Z80::cb_set_6_l   }, // 0xF5
	{ "Set Bit 6 of (HL)",           0, &Z80::cb_set_6_hlp }, // 0xF6
	{ "Set Bit 6 of A",              0, &Z80::cb_set_6_a   }, // 0xF7
	{ "Set Bit 7 of B",              0, &Z80::cb_set_7_b   }, // 0xF8
	{ "Set Bit 7 of C",              0, &Z80::cb_set_7_c   }, // 0xF9
	{ "Set Bit 7 of D",              0, &Z80::cb_set_7_d   }, // 0xFA
	{ "Set Bit 7 of E",              0, &Z80::cb_set_7_e   }, // 0xFB
	{ "Set Bit 7 of H",              0, &Z80::cb_set_7_h   }, // 0xFC
	{ "Set Bit 7 of L",              0, &Z80::cb_set_7_l   }, // 0xFD
	{ "Set Bit 7 of (HL)",           0, &Z80::cb_set_7_hlp }, // 0xFE
	{ "Set Bit 7 of A",              0, &Z80::cb_set_7_a   }  // 0xFF
};

Z80::Z80()
{
	memory = new MemoryMap();
}

Z80::~Z80()
{
	if (memory)
	{
		delete memory;
	}
}

MemoryMap *Z80::get_memory(void)
{
	return memory;
}

void Z80::init()
{
	registers.a = 0x01;
	registers.f = 0xB0;
	registers.b = 0x00;
	registers.c = 0x13;
	registers.d = 0x00;
	registers.e = 0xD8;
	registers.h = 0x01;
	registers.l = 0x4D;
	registers.sp = 0xFFFE;
	registers.pc = 0x0100;

	memory->power_up();

	/*
		register A is 0x11 if gameboy color
		page 17
	*/
	if (memory->get_cartridge()->is_cgb())
	{
		registers.a = 0x11;
	}

	stop_flag = 0x00;
	halt_flag = 0x00;
	set_ime_flag = 0x00;
	disable_ime_flag = 0x00;
	ime = 0x00;
	cycles_run = 0x0000;

	next_instruction = 0x00;
	next_instruction_operand = 0x00;
}

/*
	read one instruction from the program counter and execute it
*/
uint16_t Z80::execute_one(void)
{
	cycles_run = 0x0000;

	if (halt_flag != 0x00)
	{
		next_instruction = memory->read_byte(registers.pc);
		registers.pc++;
		
		if (instructions[next_instruction].operand_length == 1)
		{
			next_instruction_operand = (uint16_t) memory->read_byte(registers.pc);
		}
		if (instructions[next_instruction].operand_length == 2)
		{
			next_instruction_operand = memory->read_short(registers.pc);
		}
		registers.pc += instructions[next_instruction].operand_length;

		cycles_run += cycles[next_instruction];

		switch(instructions[next_instruction].operand_length) {
			case 0:
				((void (*)(void))instructions[next_instruction].execute)();
				break;
			
			case 1:
				((void (*)(uint8_t))instructions[next_instruction].execute)((uint8_t) (next_instruction_operand));
				break;
			
			case 2:
				((void (*)(uint16_t))instructions[next_instruction].execute)(next_instruction_operand);
				break;
		}
		
		//std::cout << "Ran instruction: " << instructions[next_instruction].name << std::endl;
	}

	return cycles_run;
}

uint8_t Z80::increment(uint8_t value)
{
	if ((value & 0x0F) == 0x0F)
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	
	value++;
	
	if (value)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
	
	return value;
}

uint8_t Z80::decrement(uint8_t value)
{
	if ((value & 0x0F) == 0x0F)
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	
	value--;
	
	if (value)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
	
	return value;
}

void Z80::add_byte(uint8_t *destination, uint8_t value)
{
	uint16_t result = *destination + value;
	
	if (result & 0xFF00)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	*destination = (uint8_t) (result & 0x00FF);
	
	if (*destination)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	if (((*destination & 0x0F) + (value & 0x0F)) > 0x0F)
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
}

void Z80::add_short(uint16_t *destination, uint16_t value)
{
	uint32_t result = *destination + value;
	
	if (result & 0xFFFF0000)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	*destination = (uint16_t) (result & 0xFFFF);

	if (((*destination & 0x0F) + (value & 0x0F)) > 0x0F)
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
}

void Z80::add_carry(uint8_t value)
{
	if (FLAGS_ISCARRY)
	{
		value++;
	}
	
	uint16_t result = registers.a + value;
	
	if (result & 0xFF00)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	if (registers.a == value)
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	
	if (((value & 0x0F) + (registers.a & 0x0F)) > 0x0F)
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	
	FLAGS_SET(FLAGS_NEGATIVE);
	
	registers.a = (uint8_t) (result & 0xFF);
}

void Z80::subtract_byte(uint8_t value)
{
	FLAGS_SET(FLAGS_NEGATIVE);
	
	if (value > registers.a)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	if ((value & 0x0F) > (registers.a & 0x0F))
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	
	registers.a -= value;
	
	if (registers.a)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
}

void Z80::subtract_carry(uint8_t value)
{
	if (FLAGS_ISCARRY)
	{
		value++;
	}
	
	FLAGS_SET(FLAGS_NEGATIVE);
	
	if (value > registers.a)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	if (value == registers.a)
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	
	if ((value & 0x0F) > (registers.a & 0x0F))
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	
	registers.a -= value;
}

void Z80::bitwise_and(uint8_t value)
{
	registers.a &= value;
	
	if (registers.a)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE);
	FLAGS_SET(FLAGS_HALFCARRY);
}

void Z80::bitwise_or(uint8_t value)
{
	registers.a |= value;
	
	if (registers.a)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

void Z80::bitwise_xor(uint8_t value)
{
	registers.a ^= value;
	
	if (registers.a)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_CARRY | FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

void Z80::compare(uint8_t value)
{
	if (registers.a == value)
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	
	if (value > registers.a)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	if ((value & 0x0F) > (registers.a & 0x0F))
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	
	FLAGS_SET(FLAGS_NEGATIVE);
}

/*
	OpCode:      ----
	Description: Called whenever we don't know what the op code represents
	Manual Page: ----
*/
void Z80::unknown(void)
{
	registers.pc--;
	uint8_t instruction_that_failed = memory->read_byte(registers.pc);
	printf("Unknown Function: 0x%02x\n", instruction_that_failed);
	shutdown();
}

/*
	OpCode:      0x00
	Description: No operation
	Manual Page: 97
*/
void Z80::nop(void)
{
	// no operation...do nothing
}

/*
	OpCode:      0x01
	Description: Load 16-bit operand into BC
	Manual Page: 76
*/
void Z80::ld_bc_nn(uint16_t operand)
{
	registers.bc = operand;
}

/*
	OpCode:      0x02
	Description: Load A into address (at BC)
	Manual Page: 69
*/
void Z80::ld_bcp_a(void)
{
	memory->write_byte(registers.bc, registers.a);
}

/*
	OpCode:      0x03
	Description: Increment BC
	Manual Page: 92
*/
void Z80::inc_bc(void)
{
	registers.bc++;
}

/*
	OpCode:      0x04
	Description: Increment B
	Manual Page: 88
*/
void Z80::inc_b(void)
{
	registers.b = increment(registers.b);
}

/*
	OpCode:      0x05
	Description: Decrement B
	Manual Page: 89
*/
void Z80::dec_b(void)
{
	registers.b = decrement(registers.b);
}

/*
	OpCode:      0x06
	Description: Load 8-bit operand into B
	Manual Page: 65
*/
void Z80::ld_b_n(uint8_t operand)
{
	registers.b = operand;
}

/*
	OpCode:      0x07
	Description: Rotate A left
	Manual Page: 99
*/
void Z80::rlca(void)
{
	uint8_t carry = registers.a & 0x01;
	
	if (carry)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	registers.a <<= 1;
	registers.a += carry;
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

/*
	OpCode:      0x08
	Description: Put stack pointer value into address (at operand)
	Manual Page: 78
*/
void Z80::ld_nnp_sp(uint16_t operand)
{
	memory->write_short(operand, registers.sp);
}

/*
	OpCode:      0x09
	Description: Add BC to HL, store in HL
	Manual Page: 90
*/
void Z80::add_hl_bc(void)
{
	add_short(&registers.hl, registers.bc);
}

/*
	OpCode:      0x0A
	Description: Load value (at address BC) into A
	Manual Page: 68
*/
void Z80::ld_a_bcp(void)
{
	registers.a = memory->read_byte(registers.bc);
}

/*
	OpCode:      0x0B
	Description: Decrement BC
	Manual Page: 93
*/
void Z80::dec_bc(void)
{
	registers.bc--;
}

/*
	OpCode:      0x0C
	Description: Increment C
	Manual Page: 88
*/
void Z80::inc_c(void)
{
	registers.c = increment(registers.c);
}

/*
	OpCode:      0x0D
	Description: Decrement C
	Manual Page: 89
*/
void Z80::dec_c(void)
{
	registers.c = decrement(registers.c);
}

/*
	OpCode:      0x0E
	Description: Load 8-bit operand into C
	Manual Page: 65
*/
void Z80::ld_c_n(uint8_t operand)
{
	registers.c = operand;
}

/*
	OpCode:      0x0F
	Description: Rotate A right
	Manual Page: 100
*/
void Z80::rrca(void)
{
	uint8_t carry = registers.a & 0x01;

	if (carry)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	registers.a >>= 1;
	if (carry)
	{
		registers.a |= 0x80;
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

/*
	OpCode:      0x10
	Description: Set stop flag
	Manual Page: 97
*/
void Z80::stop(uint8_t operand)
{
	stop_flag = 0x01;
}

/*
	OpCode:      0x11
	Description: Load 16-bit operand into DE
	Manual Page: 76
*/
void Z80::ld_de_nn(uint16_t operand)
{
	registers.de = operand;
}

/*
	OpCode:      0x12
	Description: Load A into address (at DE)
	Manual Page: 
*/
void Z80::ld_dep_a(void)
{
	memory->write_byte(registers.de, registers.a);
}

/*
	OpCode:      0x13
	Description: Increment DE
	Manual Page: 92
*/
void Z80::inc_de(void)
{
	registers.de++;
}

/*
	OpCode:      0x14
	Description: Increment D
	Manual Page: 88
*/
void Z80::inc_d(void)
{
	registers.d = increment(registers.d);
}

/*
	OpCode:      0x15
	Description: Decrement D
	Manual Page: 89
*/
void Z80::dec_d(void)
{
	registers.d = decrement(registers.d);
}

/*
	OpCode:      0x16
	Description: Load operand into D
	Manual Page: 65
*/
void Z80::ld_d_n(uint8_t operand)
{
	registers.d = operand;
}

/*
	OpCode:      0x17
	Description: Rotate A left thru carry
	Manual Page: 99
*/
void Z80::rla(void)
{
	int carry;
	if (FLAGS_ISSET(FLAGS_CARRY))
	{
		carry = 1;
	}
	else
	{
		carry = 0;
	}
	
	if (registers.a & 0x80)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	registers.a <<= 1;
	registers.a += carry;
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

/*
	OpCode:      0x18
	Description: Add operand to program counter and jump
	Manual Page: 112
*/
void Z80::jr_n(uint8_t operand)
{
	registers.pc += (int8_t) (operand);
}

/*
	OpCode:      0x19
	Description: Add HL and DE, store in HL
	Manual Page: 90
*/
void Z80::add_hl_de(void)
{
	add_short(&registers.hl, registers.de);
}

/*
	OpCode:      0x1A
	Description: Load value (at address DE) into A
	Manual Page: 69
*/
void Z80::ld_a_dep(void)
{
	registers.a = memory->read_byte(registers.de);
}

/*
	OpCode:      0x1B
	Description: Decrement DE
	Manual Page: 93
*/
void Z80::dec_de(void)
{
	registers.de--;
}

/*
	OpCode:      0x1C
	Description: Increment E
	Manual Page: 88
*/
void Z80::inc_e(void)
{
	registers.e = increment(registers.e);
}

/*
	OpCode:      0x1D
	Description: Decrement E
	Manual Page: 89
*/
void Z80::dec_e(void)
{
	registers.e = decrement(registers.e);
}

/*
	OpCode:      0x1E
	Description: Load 8-bit operand into E
	Manual Page: 65
*/
void Z80::ld_e_n(uint8_t operand)
{
	registers.e = operand;
}

/*
	OpCode:      0x1F
	Description: Rotate A right thru carry
	Manual Page: 100
*/
void Z80::rra(void)
{
	uint8_t carry;
	if (FLAGS_ISCARRY)
	{
		carry = 1;
	}
	else
	{
		carry = 0;
	}
	carry <<= 7;
	
	if (registers.a & 0x01)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	registers.a >>= 1;
	registers.a += carry;
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}

/*
	OpCode:      0x20
	Description: If zero flag is 0, add operand to program counter to jump
	Manual Page: 113
*/
void Z80::jr_nz_n(uint8_t operand)
{
	if (FLAGS_ISZERO)
	{
		cycles_run -= 4;
	}
	else
	{
		registers.pc += (int8_t) operand;
	}
}

/*
	OpCode:      0x21
	Description: Load 16-bit operand into HL
	Manual Page: 76
*/
void Z80::ld_hl_nn(uint16_t operand)
{
	registers.hl = operand;
}

/*
	OpCode:      0x22
	Description: Load A into address (at HL), increment HL
	Manual Page: 73
*/
void Z80::ldi_hlp_a(void)
{
	memory->write_byte(registers.hl, registers.a);
	registers.hl++;
}

/*
	OpCode:      0x23
	Description: Increment HL
	Manual Page: 92
*/
void Z80::inc_hl(void)
{
	registers.hl++;
}

/*
	OpCode:      0x24
	Description: Increment H
	Manual Page: 88
*/
void Z80::inc_h(void)
{
	registers.h = increment(registers.h);
}

/*
	OpCode:      0x25
	Description: Decrement H
	Manual Page: 89
*/
void Z80::dec_h(void)
{
	registers.h = decrement(registers.h);
}

/*
	OpCode:      0x26
	Description: Load 8-bit operand into H
	Manual Page: 65
*/
void Z80::ld_h_n(uint8_t operand)
{
	registers.h = operand;
}

/*
	OpCode:      0x27
	Description: Decimal adjust A
	Manual Page: 95
*/
void Z80::daa(void)
{
	std::cout << "DAA: You forgot to implement this..." << std::endl;
	/* not implemented yet :( */
}

/*
	OpCode:      0x28
	Description: If zero flag is 1, add operand to program counter to jump
	Manual Page: 113
*/
void Z80::jr_z_n(uint8_t operand)
{
	if (FLAGS_ISZERO)
	{
		registers.pc += (int8_t) operand;
	}
	else
	{
		cycles_run -= 4;
	}
}

/*
	OpCode:      0x29
	Description: Add HL and HL, store in HL
	Manual Page: 90
*/
void Z80::add_hl_hl(void)
{
	add_short(&registers.hl, registers.hl);
}

/*
	OpCode:      0x2A
	Description: Load value (at address HL) into A, increment HL
	Manual Page: 73
*/
void Z80::ldi_a_hlp(void)
{
	registers.a = memory->read_byte(registers.hl);
	registers.hl++;
}

/*
	OpCode:      0x2B
	Description: Decrement HL
	Manual Page: 93
*/
void Z80::dec_hl(void)
{
	registers.hl--;
}

/*
	OpCode:      0x2C
	Description: Increment L
	Manual Page: 88
*/
void Z80::inc_l(void)
{
	registers.l = increment(registers.l);
}

/*
	OpCode:      0x2D
	Description: Decrement L
	Manual Page: 89
*/
void Z80::dec_l(void)
{
	registers.l = decrement(registers.l);
}

/*
	OpCode:      0x2E
	Description: Load 8-bit operand into L
	Manual Page: 65
*/
void Z80::ld_l_n(uint8_t operand)
{
	registers.l = operand;
}

/*
	OpCode:      0x2F
	Description: Complement A (Invert bits of A)
	Manual Page: 95
*/
void Z80::cpl(void)
{
	registers.a = ~registers.a;
	FLAGS_SET(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

/*
	OpCode:      0x30
	Description: If carry flag is 0, add operand to program counter to jump
	Manual Page: 113
*/
void Z80::jr_nc_n(uint8_t operand)
{
	if (FLAGS_ISCARRY)
	{
		cycles_run -= 4;
	}
	else
	{
		registers.pc += operand;
	}
}

/*
	OpCode:      0x31
	Description: Load 16-bit operand into Stack Pointer
	Manual Page: 31
*/
void Z80::ld_sp_nn(uint16_t operand)
{
	registers.sp = operand;
}

/*
	OpCode:      0x32
	Description: Load A into address (at HL), decrement HL
	Manual Page: 32
*/
void Z80::ldd_hlp_a(void)
{
	memory->write_byte(registers.hl, registers.a);
	registers.hl--;
}

/*
	OpCode:      0x33
	Description: Increment Stack Pointer
	Manual Page: 92
*/
void Z80::inc_sp(void)
{
	registers.sp++;
}

/*
	OpCode:      0x34
	Description: Increment value (at address HL)
	Manual Page: 88
*/
void Z80::inc_hlp(void)
{
	memory->write_byte(registers.hl, increment(memory->read_byte(registers.hl)));
}

/*
	OpCode:      0x35
	Description: Decrement value (at address HL)
	Manual Page: 89
*/
void Z80::dec_hlp(void)
{
	memory->write_byte(registers.hl, decrement(memory->read_byte(registers.hl)));
}

/*
	OpCode:      0x36
	Description: Load 8-bit operand into address (at HL)
	Manual Page: 66-67
*/
void Z80::ld_hlp_n(uint8_t operand)
{
	memory->write_byte(registers.hl, operand);
}

/*
	OpCode:      0x37
	Description: Set carry flag
	Manual Page: 96
*/
void Z80::scf(void)
{
	FLAGS_SET(FLAGS_CARRY);
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

/*
	OpCode:      0x38
	Description: If carry flag is 1, add operand to program counter to jump
	Manual Page: 113
*/
void Z80::jr_c_n(uint8_t operand)
{
	if (FLAGS_ISCARRY)
	{
		registers.pc += operand;
	}
	else
	{
		cycles_run -= 4;
	}
}

/*
	OpCode:      0x39
	Description: Add HL and SP, store in HL
	Manual Page: 
*/
void Z80::add_hl_sp(void)
{
	add_short(&registers.hl, registers.sp);
}

/*
	OpCode:      0x3A
	Description: Load value (at address HL) into A, decrement HL
	Manual Page: 
*/
void Z80::ldd_a_hlp(void)
{
	registers.a = memory->read_byte(registers.hl);
	registers.hl--;
}

/*
	OpCode:      0x3B
	Description: Decrement Stack Pointer
	Manual Page: 93
*/
void Z80::dec_sp(void)
{
	registers.sp--;
}

/*
	OpCode:      0x3C
	Description: Increment A
	Manual Page: 88
*/
void Z80::inc_a(void)
{
	registers.a = increment(registers.a);
}

/*
	OpCode:      0x3D
	Description: Decrement A
	Manual Page: 89
*/
void Z80::dec_a(void)
{
	registers.a = decrement(registers.a);
}

/*
	OpCode:      0x3E
	Description: Load 8-bit operand into A
	Manual Page: 
*/
void Z80::ld_a_n(uint8_t operand)
{
	registers.a = operand;
}

/*
	OpCode:      0x3F
	Description: Complement carry flag (invert it)
	Manual Page: 96
*/
void Z80::ccf(void)
{
	if (FLAGS_ISCARRY)
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	else
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

/*
	OpCode:      0x40
	Description: Load B into B
	Manual Page: 66-67
*/
// nop()

/*
	OpCode:      0x41
	Description: Load C into B
	Manual Page: 66-67
*/
void Z80::ld_b_c(void)
{
	registers.b = registers.c;
}

/*
	OpCode:      0x42
	Description: Load D into B
	Manual Page: 66-67
*/
void Z80::ld_b_d(void)
{
	registers.b = registers.d;
}

/*
	OpCode:      0x43
	Description: Load E into B
	Manual Page: 66-67
*/
void Z80::ld_b_e(void)
{
	registers.b = registers.e;
}

/*
	OpCode:      0x44
	Description: Load H into B
	Manual Page: 66-67
*/
void Z80::ld_b_h(void)
{
	registers.b = registers.h;
}

/*
	OpCode:      0x45
	Description: Load L into B
	Manual Page: 66-67
*/
void Z80::ld_b_l(void)
{
	registers.b = registers.l;
}

/*
	OpCode:      0x46
	Description: Load value (at address HL) into B
	Manual Page: 66-67
*/
void Z80::ld_b_hlp(void)
{
	registers.b = memory->read_byte(registers.hl);
}

/*
	OpCode:      0x47
	Description: Load A into B
	Manual Page: 66-67
*/
void Z80::ld_b_a(void)
{
	registers.b = registers.a;
}

/*
	OpCode:      0x48
	Description: Load B into C
	Manual Page: 66-67
*/
void Z80::ld_c_b(void)
{
	registers.c = registers.b;
}

/*
	OpCode:      0x49
	Description: Load C into C
	Manual Page: 66-67
*/
// nop()

/*
	OpCode:      0x4A
	Description: Load D into C
	Manual Page: 66-67
*/
void Z80::ld_c_d(void)
{
	registers.c = registers.d;
}

/*
	OpCode:      0x4B
	Description: Load E into C
	Manual Page: 66-67
*/
void Z80::ld_c_e(void)
{
	registers.c = registers.e;
}

/*
	OpCode:      0x4C
	Description: Load H into C
	Manual Page: 66-67
*/
void Z80::ld_c_h(void)
{
	registers.c = registers.h;
}

/*
	OpCode:      0x4D
	Description: Load L into C
	Manual Page: 66-67
*/
void Z80::ld_c_l(void)
{
	registers.c = registers.l;
}

/*
	OpCode:      0x4E
	Description: Load value (at addres HL) into C
	Manual Page: 66-67
*/
void Z80::ld_c_hlp(void)
{
	registers.c = memory->read_byte(registers.hl);
}

/*
	OpCode:      0x4F
	Description: Load A into C
	Manual Page: 66-67
*/
void Z80::ld_c_a(void)
{
	registers.c = registers.a;
}

/*
	OpCode:      0x50
	Description: Load B into D
	Manual Page: 66-67
*/
void Z80::ld_d_b(void)
{
	registers.d = registers.b;
}

/*
	OpCode:      0x51
	Description: Load C into D
	Manual Page: 66-67
*/
void Z80::ld_d_c(void)
{
	registers.d = registers.c;
}

/*
	OpCode:      0x52
	Description: Load D into D
	Manual Page: 66-67
*/
// nop()

/*
	OpCode:      0x53
	Description: Load E into D
	Manual Page: 66-67
*/
void Z80::ld_d_e(void)
{
	registers.d = registers.e;
}

/*
	OpCode:      0x54
	Description: Load H into D
	Manual Page: 66-67
*/
void Z80::ld_d_h(void)
{
	registers.d = registers.h;
}

/*
	OpCode:      0x55
	Description: Load L into D
	Manual Page: 66-67
*/
void Z80::ld_d_l(void)
{
	registers.d = registers.l;
}

/*
	OpCode:      0x56
	Description: Load value (at addres HL) into D
	Manual Page: 66-67
*/
void Z80::ld_d_hlp(void)
{
	registers.d = memory->read_byte(registers.hl);
}

/*
	OpCode:      0x57
	Description: Load A into D
	Manual Page: 66-67
*/
void Z80::ld_d_a(void)
{
	registers.d = registers.a;
}

/*
	OpCode:      0x58
	Description: Load B into E
	Manual Page: 66-67
*/
void Z80::ld_e_b(void)
{
	registers.e = registers.b;
}

/*
	OpCode:      0x59
	Description: Load C into E
	Manual Page: 66-67
*/
void Z80::ld_e_c(void)
{
	registers.e = registers.c;
}

/*
	OpCode:      0x5A
	Description: Load D into E
	Manual Page: 66-67
*/
void Z80::ld_e_d(void)
{
	registers.e = registers.d;
}

/*
	OpCode:      0x5B
	Description: Load E into E
	Manual Page: 66-67
*/
// nop()

/*
	OpCode:      0x5C
	Description: Load H into E
	Manual Page: 66-67
*/
void Z80::ld_e_h(void)
{
	registers.e = registers.h;
}

/*
	OpCode:      0x5D
	Description: Load L into E
	Manual Page: 66-67
*/
void Z80::ld_e_l(void)
{
	registers.e = registers.l;
}

/*
	OpCode:      0x5E
	Description: Load value (at addres HL) into E
	Manual Page: 66-67
*/
void Z80::ld_e_hlp(void)
{
	registers.e = memory->read_byte(registers.hl);
}

/*
	OpCode:      0x5F
	Description: Load A into E
	Manual Page: 66-67
*/
void Z80::ld_e_a(void)
{
	registers.e = registers.a;
}

/*
	OpCode:      0x60
	Description: Load B into H
	Manual Page: 66-67
*/
void Z80::ld_h_b(void)
{
	registers.h = registers.b;
}

/*
	OpCode:      0x61
	Description: Load C into H
	Manual Page: 66-67
*/
void Z80::ld_h_c(void)
{
	registers.h = registers.c;
}

/*
	OpCode:      0x62
	Description: Load D into H
	Manual Page: 66-67
*/
void Z80::ld_h_d(void)
{
	registers.h = registers.d;
}

/*
	OpCode:      0x63
	Description: Load E into H
	Manual Page: 66-67
*/
void Z80::ld_h_e(void)
{
	registers.h = registers.e;
}

/*
	OpCode:      0x64
	Description: Load H into H
	Manual Page: 66-67
*/
// nop()

/*
	OpCode:      0x65
	Description: Load L into H
	Manual Page: 66-67
*/
void Z80::ld_h_l(void)
{
	registers.h = registers.l;
}

/*
	OpCode:      0x66
	Description: Load value (at address HL) into H
	Manual Page: 66-67
*/
void Z80::ld_h_hlp(void)
{
	registers.h = memory->read_byte(registers.hl);
}

/*
	OpCode:      0x67
	Description: Load A into H
	Manual Page: 66-67
*/
void Z80::ld_h_a(void)
{
	registers.h = registers.a;
}

/*
	OpCode:      0x68
	Description: Load B into L
	Manual Page: 66-67
*/
void Z80::ld_l_b(void)
{
	registers.l = registers.b;
}

/*
	OpCode:      0x69
	Description: Load C into L
	Manual Page: 66-67
*/
void Z80::ld_l_c(void)
{
	registers.l = registers.c;
}

/*
	OpCode:      0x6A
	Description: Load D into L
	Manual Page: 66-67
*/
void Z80::ld_l_d(void)
{
	registers.l = registers.d;
}

/*
	OpCode:      0x6B
	Description: Load E into L
	Manual Page: 66-67
*/
void Z80::ld_l_e(void)
{
	registers.l = registers.e;
}

/*
	OpCode:      0x6C
	Description: Load H into L
	Manual Page: 66-67
*/
void Z80::ld_l_h(void)
{
	registers.l = registers.h;
}

/*
	OpCode:      0x6D
	Description: Load L into L
	Manual Page: 66-67
*/
// nop()

/*
	OpCode:      0x6E
	Description: Load value (at address HL) into L
	Manual Page: 66-67
*/
void Z80::ld_l_hlp(void)
{
	registers.l = memory->read_byte(registers.hl);
}

/*
	OpCode:      0x6F
	Description: Load A into L
	Manual Page: 66-67
*/
void Z80::ld_l_a(void)
{
	registers.l = registers.a;
}

/*
	OpCode:      0x70
	Description: Load B into address (at HL)
	Manual Page: 66-67
*/
void Z80::ld_hlp_b(void)
{
	memory->write_byte(registers.hl, registers.b);
}

/*
	OpCode:      0x71
	Description: Load C into address (at HL)
	Manual Page: 66-67
*/
void Z80::ld_hlp_c(void)
{
	memory->write_byte(registers.hl, registers.c);
}

/*
	OpCode:      0x72
	Description: Load D into address (at HL)
	Manual Page: 66-67
*/
void Z80::ld_hlp_d(void)
{
	memory->write_byte(registers.hl, registers.d);
}

/*
	OpCode:      0x73
	Description: Load E into address (at HL)
	Manual Page: 66-67
*/
void Z80::ld_hlp_e(void)
{
	memory->write_byte(registers.hl, registers.e);
}

/*
	OpCode:      0x74
	Description: Load H into address (at HL)
	Manual Page: 66-67
*/
void Z80::ld_hlp_h(void)
{
	memory->write_byte(registers.hl, registers.h);
}

/*
	OpCode:      0x75
	Description: Load L into address (at HL)
	Manual Page: 66-67
*/
void Z80::ld_hlp_l(void)
{
	memory->write_byte(registers.hl, registers.l);
}

/*
	OpCode:      0x76
	Description: Set halt flag
	Manual Page: 97
*/
void Z80::halt(void)
{
	halt_flag = 0x01;
}

/*
	OpCode:      0x77
	Description: Load A into address (at HL)
	Manual Page: 66-67
*/
void Z80::ld_hlp_a(void)
{
	memory->write_byte(registers.hl, registers.a);
}

/*
	OpCode:      0x78
	Description: Load B into A
	Manual Page: 66-67
*/
void Z80::ld_a_b(void)
{
	registers.a = registers.b;
}

/*
	OpCode:      0x79
	Description: Load C into A
	Manual Page: 66-67
*/
void Z80::ld_a_c(void)
{
	registers.a = registers.c;
}

/*
	OpCode:      0x7A
	Description: Load D into A
	Manual Page: 66-67
*/
void Z80::ld_a_d(void)
{
	registers.a = registers.d;
}

/*
	OpCode:      0x7B
	Description: Load E into A
	Manual Page: 66-67
*/
void Z80::ld_a_e(void)
{
	registers.a = registers.e;
}

/*
	OpCode:      0x7C
	Description: Load H into A
	Manual Page: 66-67
*/
void Z80::ld_a_h(void)
{
	registers.a = registers.h;
}

/*
	OpCode:      0x7D
	Description: Load L into A
	Manual Page: 66-67
*/
void Z80::ld_a_l(void)
{
	registers.a = registers.l;
}

/*
	OpCode:      0x7E
	Description: Load value (at address HL) into A
	Manual Page: 66-67
*/
void Z80::ld_a_hlp(void)
{
	registers.a = memory->read_byte(registers.hl);
}

/*
	OpCode:      0x7F
	Description: Load A into A
	Manual Page: 66-67
*/
// nop()

/*
	OpCode:      0x80
	Description: Add A and B, store in A
	Manual Page: 80
*/
void Z80::add_a_b(void)
{
	add_byte(&registers.a, registers.b);
}

/*
	OpCode:      0x81
	Description: Add A and C, store in A
	Manual Page: 80
*/
void Z80::add_a_c(void)
{
	add_byte(&registers.a, registers.c);
}

/*
	OpCode:      0x82
	Description: Add A and D, store in A
	Manual Page: 80
*/
void Z80::add_a_d(void)
{
	add_byte(&registers.a, registers.d);
}

/*
	OpCode:      0x83
	Description: Add A and E, store in A
	Manual Page: 80
*/
void Z80::add_a_e(void)
{
	add_byte(&registers.a, registers.e);
}

/*
	OpCode:      0x84
	Description: Add A and H, store in A
	Manual Page: 80
*/
void Z80::add_a_h(void)
{
	add_byte(&registers.a, registers.h);
}

/*
	OpCode:      0x85
	Description: Add A and L, store in A
	Manual Page: 80
*/
void Z80::add_a_l(void)
{
	add_byte(&registers.a, registers.l);
}

/*
	OpCode:      0x86
	Description: Add A and value (at address HL), store in A
	Manual Page: 80
*/
void Z80::add_a_hlp(void)
{
	add_byte(&registers.a, memory->read_byte(registers.hl));
}

/*
	OpCode:      0x87
	Description: Add A and A, store in A
	Manual Page: 80
*/
void Z80::add_a_a(void)
{
	add_byte(&registers.a, registers.a);
}

/*
	OpCode:      0x88
	Description: Add A, carry, and B, store in A
	Manual Page: 81
*/
void Z80::adc_b(void)
{
	add_carry(registers.b);
}

/*
	OpCode:      0x89
	Description: Add A, carry, and C, store in A
	Manual Page: 81
*/
void Z80::adc_c(void)
{
	add_carry(registers.c);
}

/*
	OpCode:      0x8A
	Description: Add A, carry, and D, store in A
	Manual Page: 81
*/
void Z80::adc_d(void)
{
	add_carry(registers.d);
}

/*
	OpCode:      0x8B
	Description: Add A, carry, and E, store in A
	Manual Page: 81
*/
void Z80::adc_e(void)
{
	add_carry(registers.e);
}

/*
	OpCode:      0x8C
	Description: Add A, carry, and H, store in A
	Manual Page: 81
*/
void Z80::adc_h(void)
{
	add_carry(registers.h);
}

/*
	OpCode:      0x8D
	Description: Add A, carry, and L, store in A
	Manual Page: 81
*/
void Z80::adc_l(void)
{
	add_carry(registers.l);
}

/*
	OpCode:      0x8E
	Description: Add A, carry, and value (at address HL), store in A
	Manual Page: 81
*/
void Z80::adc_hlp(void)
{
	add_carry(memory->read_byte(registers.hl));
}

/*
	OpCode:      0x8F
	Description: Add A, carry, and A, store in A
	Manual Page: 81
*/
void Z80::adc_a(void)
{
	add_carry(registers.a);
}

/*
	OpCode:      0x90
	Description: Subtract B from A, store in A
	Manual Page: 82
*/
void Z80::sub_b(void)
{
	subtract_byte(registers.b);
}

/*
	OpCode:      0x91
	Description: Subtract C from A, store in A
	Manual Page: 82
*/
void Z80::sub_c(void)
{
	subtract_byte(registers.c);
}

/*
	OpCode:      0x92
	Description: Subtract D from A, store in A
	Manual Page: 82
*/
void Z80::sub_d(void)
{
	subtract_byte(registers.d);
}

/*
	OpCode:      0x93
	Description: Subtract E from A, store in A
	Manual Page: 82
*/
void Z80::sub_e(void)
{
	subtract_byte(registers.e);
}

/*
	OpCode:      0x94
	Description: Subtract H from A, store in A
	Manual Page: 82
*/
void Z80::sub_h(void)
{
	subtract_byte(registers.h);
}

/*
	OpCode:      0x95
	Description: Subtract L from A, store in A
	Manual Page: 82
*/
void Z80::sub_l(void)
{
	subtract_byte(registers.l);
}

/*
	OpCode:      0x96
	Description: Subtract value (at address HL) from A, store in A
	Manual Page: 82
*/
void Z80::sub_hlp(void)
{
	subtract_byte(memory->read_byte(registers.hl));
}

/*
	OpCode:      0x97
	Description: Subtract A from A, store in A
	Manual Page: 82
*/
void Z80::sub_a(void)
{
	subtract_byte(registers.a);
}

/*
	OpCode:      0x98
	Description: Subtract B and carry from A, store in A
	Manual Page: 83
*/
void Z80::sbc_b(void)
{
	subtract_carry(registers.b);
}

/*
	OpCode:      0x99
	Description: Subtract C and carry from A, store in A
	Manual Page: 83
*/
void Z80::sbc_c(void)
{
	subtract_carry(registers.c);
}

/*
	OpCode:      0x9A
	Description: Subtract D and carry from A, store in A
	Manual Page: 83
*/
void Z80::sbc_d(void)
{
	subtract_carry(registers.d);
}

/*
	OpCode:      0x9B
	Description: Subtract E and carry from A, store in A
	Manual Page: 83
*/
void Z80::sbc_e(void)
{
	subtract_carry(registers.e);
}

/*
	OpCode:      0x9C
	Description: Subtract H and carry from A, store in A
	Manual Page: 83
*/
void Z80::sbc_h(void)
{
	subtract_carry(registers.h);
}

/*
	OpCode:      0x9D
	Description: Subtract L and carry from A, store in A
	Manual Page: 83
*/
void Z80::sbc_l(void)
{
	subtract_carry(registers.l);
}

/*
	OpCode:      0x9E
	Description: Subtract value (at address HL) and carry from A, store in A
	Manual Page: 83
*/
void Z80::sbc_hlp(void)
{
	subtract_carry(memory->read_byte(registers.hl));
}

/*
	OpCode:      0x9F
	Description: Subtract A and carry from A, store in A
	Manual Page: 83
*/
void Z80::sbc_a(void)
{
	subtract_carry(registers.a);
}

/*
	OpCode:      0xA0
	Description: Bitwise AND A and B, store in A
	Manual Page: 84
*/
void Z80::and_b(void)
{
	bitwise_and(registers.b);
}

/*
	OpCode:      0xA1
	Description: Bitwise AND A and C, store in A
	Manual Page: 84
*/
void Z80::and_c(void)
{
	bitwise_and(registers.c);
}

/*
	OpCode:      0xA2
	Description: Bitwise AND A and D, store in A
	Manual Page: 84
*/
void Z80::and_d(void)
{
	bitwise_and(registers.d);
}

/*
	OpCode:      0xA3
	Description: Bitwise AND A and E, store in A
	Manual Page: 84
*/
void Z80::and_e(void)
{
	bitwise_and(registers.e);
}

/*
	OpCode:      0xA4
	Description: Bitwise AND A and H, store in A
	Manual Page: 84
*/
void Z80::and_h(void)
{
	bitwise_and(registers.h);
}

/*
	OpCode:      0xA5
	Description: Bitwise AND A and L, store in A
	Manual Page: 84
*/
void Z80::and_l(void)
{
	bitwise_and(registers.l);
}

/*
	OpCode:      0xA6
	Description: Bitwise AND A and value (at address HL), store in A
	Manual Page: 84
*/
void Z80::and_hlp(void)
{
	bitwise_and(memory->read_byte(registers.hl));
}

/*
	OpCode:      0xA7
	Description: Bitwise AND A and A, store in A
	Manual Page: 84
*/
void Z80::and_a(void)
{
	bitwise_and(registers.a);
}

/*
	OpCode:      0xA8
	Description: Bitwise XOR A and B, store in A
	Manual Page: 86
*/
void Z80::xor_b(void)
{
	bitwise_xor(registers.b);
}

/*
	OpCode:      0xA9
	Description: Bitwise XOR A and C, store in A
	Manual Page: 86
*/
void Z80::xor_c(void)
{
	bitwise_xor(registers.c);
}

/*
	OpCode:      0xAA
	Description: Bitwise XOR A and D, store in A
	Manual Page: 86
*/
void Z80::xor_d(void)
{
	bitwise_xor(registers.d);
}

/*
	OpCode:      0xAB
	Description: Bitwise XOR A and E, store in A
	Manual Page: 86
*/
void Z80::xor_e(void)
{
	bitwise_xor(registers.e);
}

/*
	OpCode:      0xAC
	Description: Bitwise XOR A and H, store in A
	Manual Page: 86
*/
void Z80::xor_h(void)
{
	bitwise_xor(registers.h);
}

/*
	OpCode:      0xAD
	Description: Bitwise XOR A and L, store in A
	Manual Page: 86
*/
void Z80::xor_l(void)
{
	bitwise_xor(registers.l);
}

/*
	OpCode:      0xAE
	Description: Bitwise XOR A and value (at address HL), store in A
	Manual Page: 86
*/
void Z80::xor_hlp(void)
{
	bitwise_xor(memory->read_byte(registers.hl));
}

/*
	OpCode:      0xAF
	Description: Bitwise XOR A and A, store in A
	Manual Page: 86
*/
void Z80::xor_a(void)
{
	bitwise_xor(registers.a);
}

/*
	OpCode:      0xB0
	Description: Bitwise OR A and B, store in A
	Manual Page: 85
*/
void Z80::or_b(void)
{
	bitwise_or(registers.b);
}

/*
	OpCode:      0xB1
	Description: Bitwise OR A and C, store in A
	Manual Page: 85
*/
void Z80::or_c(void)
{
	bitwise_or(registers.c);
}

/*
	OpCode:      0xB2
	Description: Bitwise OR A and D, store in A
	Manual Page: 85
*/
void Z80::or_d(void)
{
	bitwise_or(registers.d);
}

/*
	OpCode:      0xB3
	Description: Bitwise OR A and E, store in A
	Manual Page: 85
*/
void Z80::or_e(void)
{
	bitwise_or(registers.e);
}

/*
	OpCode:      0xB4
	Description: Bitwise OR A and H, store in A
	Manual Page: 85
*/
void Z80::or_h(void)
{
	bitwise_or(registers.h);
}

/*
	OpCode:      0xB5
	Description: Bitwise OR A and L, store in A
	Manual Page: 85
*/
void Z80::or_l(void)
{
	bitwise_or(registers.l);
}

/*
	OpCode:      0xB6
	Description: Bitwise OR A and value (at address HL), store in A
	Manual Page: 85
*/
void Z80::or_hlp(void)
{
	bitwise_or(memory->read_byte(registers.hl));
}

/*
	OpCode:      0xB7
	Description: Bitwise OR A and A, store in A
	Manual Page: 85
*/
void Z80::or_a(void)
{
	bitwise_or(registers.a);
}

/*
	OpCode:      0xB8
	Description: Compare A and B, store in A
	Manual Page: 87
*/
void Z80::cp_b(void)
{
	compare(registers.b);
}

/*
	OpCode:      0xB9
	Description: Compare A and C, store in A
	Manual Page: 87
*/
void Z80::cp_c(void)
{
	compare(registers.c);
}

/*
	OpCode:      0xBA
	Description: Compare A and D, store in A
	Manual Page: 87
*/
void Z80::cp_d(void)
{
	compare(registers.d);
}

/*
	OpCode:      0xBB
	Description: Compare A and E, store in A
	Manual Page: 87
*/
void Z80::cp_e(void)
{
	compare(registers.e);
}

/*
	OpCode:      0xBC
	Description: Compare A and H, store in A
	Manual Page: 87
*/
void Z80::cp_h(void)
{
	compare(registers.h);
}

/*
	OpCode:      0xBD
	Description: Compare A and L, store in A
	Manual Page: 87
*/
void Z80::cp_l(void)
{
	compare(registers.l);
}

/*
	OpCode:      0xBE
	Description: Compare A and value (at address HL), store in A
	Manual Page: 87
*/
void Z80::cp_hlp(void)
{
	compare(memory->read_byte(registers.hl));
}

/*
	OpCode:      0xBF
	Description: Compare A and A, store in A
	Manual Page: 87
*/
void Z80::cp_a(void)
{
	compare(registers.a);
}

/*
	OpCode:      0xC0
	Description: If zero flag is 0, pop two bytes from stack, jump to that address
	Manual Page: 117
*/
void Z80::ret_nz(void)
{
	if (FLAGS_ISZERO)
	{
		cycles_run -= 12;
	}
	else
	{
		registers.pc = memory->read_short(registers.sp);
		registers.sp += 2;
	}
}

/*
	OpCode:      0xC1
	Description: Pop two bytes from stack, store in BC
	Manual Page: 79
*/
void Z80::pop_bc(void)
{
	registers.bc = memory->read_short(registers.sp);
	registers.sp += 2;
}

/*
	OpCode:      0xC2
	Description: If zero flag is 0, jump to address (at operand)
	Manual Page: 111
*/
void Z80::jp_nz_nn(uint16_t operand)
{
	if (FLAGS_ISZERO)
	{
		cycles_run -= 4;
	}
	else
	{
		registers.pc = operand;
	}
}

/*
	OpCode:      0xC3
	Description: Jump to address (at operand)
	Manual Page: 111
*/
void Z80::jp_nn(uint16_t operand)
{
	registers.pc = operand;
}

/*
	OpCode:      0xC4
	Description: If zero flag is 0, push PC onto stack and jump to address (at operand)
	Manual Page: 115
*/
void Z80::call_nz_nn(uint16_t operand)
{
	if (FLAGS_ISZERO)
	{
		cycles_run -= 12;
	}
	else
	{
		registers.sp -= 2;
		memory->write_short(registers.sp, registers.pc);
		registers.pc = operand;
	}
}

/*
	OpCode:      0xC5
	Description: Push BC to Stack
	Manual Page: 78
*/
void Z80::push_bc(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.bc);
}

/*
	OpCode:      0xC6
	Description: Add A and operand, store in A
	Manual Page: 80
*/
void Z80::add_a_n(uint8_t operand)
{
	add_byte(&registers.a, operand);
}

/*
	OpCode:      0xC7
	Description: Push PC onto stack, jump to 0x0000
	Manual Page: 116
*/
void Z80::rst_0(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.pc);
	registers.pc = 0x0000;
}

/*
	OpCode:      0xC8
	Description: If zero flag is 1, pop two bytes and jump to that address
	Manual Page: 117
*/
void Z80::ret_z(void)
{
	if (FLAGS_ISZERO)
	{
		registers.pc = memory->read_short(registers.sp);
		registers.sp += 2;
	}
	else
	{
		cycles_run -= 12;
	}
}

/*
	OpCode:      0xC9
	Description: Pop two bytes, jump to that address
	Manual Page: 117
*/
void Z80::ret(void)
{
	registers.pc = memory->read_short(registers.sp);
	registers.sp += 2;
}

/*
	OpCode:      0xCA
	Description: If zero flag is 1, jump to address (at operand)
	Manual Page: 111
*/
void Z80::jp_z_nn(uint16_t operand)
{
	if (FLAGS_ISZERO)
	{
		registers.pc = operand;
	}
	else
	{
		cycles_run -= 4;
	}
}

/*
	OpCode:      0xCB
	Description: Extended commands
	Manual Page: ----
*/
void Z80::cb_n(uint8_t operand)
{
	((void (*)(void))cb_instructions[operand].execute)();
	cycles_run += cb_cycles[operand];
}

/*
	OpCode:      0xCC
	Description: If zero flag is 1, push PC onto stack and jump to address (at operand)
	Manual Page: 
*/
void Z80::call_z_nn(uint16_t operand)
{
	if (FLAGS_ISZERO)
	{
		registers.sp -= 2;
		memory->write_short(registers.sp, registers.pc);
		registers.pc = operand;
	}
	else
	{
		cycles_run -= 12;
	}
}

/*
	OpCode:      0xCD
	Description: Push PC onto stack and jump to address (at operand)
	Manual Page: 114
*/
void Z80::call_nn(uint16_t operand)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.pc);
	registers.pc = operand;
}

/*
	OpCode:      0xCE
	Description: Add A, carry, and operand, store in A
	Manual Page: 81
*/
void Z80::adc_n(uint8_t operand)
{
	add_carry(operand);
}

/*
	OpCode:      0xCF
	Description: Push PC onto stack, jump to 0x0008
	Manual Page: 116
*/
void Z80::rst_08(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.pc);
	registers.pc = 0x0008;
}

/*
	OpCode:      0xD0
	Description: If carry flag is 0, pop two bytes and jump to that address
	Manual Page: 117
*/
void Z80::ret_nc(void)
{
	if (FLAGS_ISCARRY)
	{
		cycles_run -= 12;
	}
	else
	{
		registers.pc = memory->read_short(registers.sp);
		registers.sp += 2;
	}
}

/*
	OpCode:      0xD1
	Description: Pop two bytes from stack, store in DE
	Manual Page: 79
*/
void Z80::pop_de(void)
{
	registers.de = memory->read_short(registers.sp);
	registers.sp += 2;
}

/*
	OpCode:      0xD2
	Description: If carry flag is 0, jump to address (at operand)
	Manual Page: 111
*/
void Z80::jp_nc_nn(uint16_t operand)
{
	if (FLAGS_CARRY)
	{
		cycles_run -= 4;
	}
	else
	{
		registers.pc = operand;
	}
}

/*
	OpCode:      0xD4
	Description: If carry flag is 0, push PC onto stack and jump to address (at operand)
	Manual Page: 115
*/
void Z80::call_nc_nn(uint16_t operand)
{
	if (FLAGS_ISCARRY)
	{
		cycles_run -= 12;
	}
	else
	{
		registers.sp -= 2;
		memory->write_short(registers.sp, registers.pc);
		registers.pc = operand;
	}
}

/*
	OpCode:      0xD5
	Description: Push DE to Stack
	Manual Page: 78
*/
void Z80::push_de(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.de);
}

/*
	OpCode:      0xD6
	Description: Subtract operand from A
	Manual Page: 82
*/
void Z80::sub_n(uint8_t operand)
{
	subtract_byte(operand);
}

/*
	OpCode:      0xD7
	Description: Push PC onto stack, jump to 0x0010
	Manual Page: 116
*/
void Z80::rst_10(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.pc);
	registers.pc = 0x0010;
}

/*
	OpCode:      0xD8
	Description: If carry flag is 1, pop two bytes and jump to that address
	Manual Page: 117
*/
void Z80::ret_c(void)
{
	if (FLAGS_ISCARRY)
	{
		registers.pc = memory->read_short(registers.sp);
		registers.sp += 2;
	}
	else
	{
		cycles_run -= 12;
	}
}

/*
	OpCode:      0xD9
	Description: Return from interrupt
	Manual Page: 118
*/
void Z80::reti(void)
{
	registers.pc = memory->read_short(registers.sp);
	registers.sp += 2;
	set_ime_flag = 0x01;
}

/*
	OpCode:      0xDA
	Description: If carry flag is 1, jump to address (at operand)
	Manual Page: 111
*/
void Z80::jp_c_nn(uint16_t operand)
{
	if (FLAGS_CARRY)
	{
		registers.pc = operand;
	}
	else
	{
		cycles_run -= 4;
	}
}

/*
	OpCode:      0xDB
	Description: Unknown Operation
	Manual Page: ----
*/

/*
	OpCode:      0xDC
	Description: If carry flag is 1, push PC onto stack and jump to address (at operand)
	Manual Page: 115
*/
void Z80::call_c_nn(uint16_t operand)
{
	if (FLAGS_ISCARRY)
	{
		registers.sp -= 2;
		memory->write_short(registers.sp, registers.pc);
		registers.pc = operand;
	}
	else
	{
		cycles_run -= 12;
	}
}

/*
	OpCode:      0xDD
	Description: Unknown Operation
	Manual Page: ----
*/

/*
	OpCode:      0xDE
	Description: Subtract operand and carry from A, store in A
	Manual Page: 83
*/
void Z80::sbc_n(uint8_t operand)
{
	subtract_carry(operand);
}

/*
	OpCode:      0xDF
	Description: Push PC onto stack, jump to 0x0018
	Manual Page: 116
*/
void Z80::rst_18(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.pc);
	registers.pc = 0x0018;
}


/*
	OpCode:      0xE0
	Description: Store A in 0xFF00 + operand
	Manual Page: 70
*/
void Z80::ld_ff_n_ap(uint8_t operand)
{
	memory->write_byte(0xFF00 + operand, registers.a);
}

/*
	OpCode:      0xE1
	Description: 
	Manual Page: 
*/
void Z80::pop_hl(void)
{
	registers.hl = memory->read_short(registers.sp);
	registers.sp += 2;
}

/*
	OpCode:      0xE2
	Description: Store A in 0xFF00 + C
	Manual Page: 70
*/
void Z80::ld_ff_c_a(void)
{
	memory->write_byte(0xFF00 + registers.c, registers.a);
}

/*
	OpCode:      0xE3
	Description: Unknown Operation
	Manual Page: ----
*/

/*
	OpCode:      0xE4
	Description: Unknown Operation
	Manual Page: ----
*/

/*
	OpCode:      0xE5
	Description: Push HL to Stack
	Manual Page: 78
*/
void Z80::push_hl(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.hl);
}

/*
	OpCode:      0xE6
	Description: Bitwise AND A and operand, store in A
	Manual Page: 84
*/
void Z80::and_n(uint8_t operand)
{
	bitwise_and(operand);
}

/*
	OpCode:      0xE7
	Description: Push PC onto stack, jump to 0x0020
	Manual Page: 116
*/
void Z80::rst_20(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.pc);
	registers.pc = 0x0020;
}

/*
	OpCode:      0xE8
	Description: Add operand to stack pointer
	Manual Page: 91
*/
void Z80::add_sp_n(uint8_t operand)
{
	uint32_t result = registers.sp + operand;
	
	if(result & 0xFFFF0000)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	registers.sp = result & 0xFFFF;
	
	if(((registers.sp & 0x0F) + (operand & 0x0F)) > 0x0F)
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	
	// _does_ clear the zero flag
	FLAGS_CLEAR(FLAGS_ZERO | FLAGS_NEGATIVE);
}

/*
	OpCode:      0xE9
	Description: Jump to HL
	Manual Page: 112
*/
void Z80::jp_hl(void)
{
	registers.pc = registers.hl;
}

/*
	OpCode:      0xEA
	Description: Store A at address (at operand)
	Manual Page: 75
*/
void Z80::ld_nnp_a(uint16_t operand)
{
	memory->write_byte(operand, registers.a);
}

/*
	OpCode:      0xEB
	Description: Unknown Operation
	Manual Page: ----
*/

/*
	OpCode:      0xEC
	Description: Unknown Operation
	Manual Page: ----
*/

/*
	OpCode:      0xED
	Description: Unknown Operation
	Manual Page: ----
*/

/*
	OpCode:      0xEE
	Description: Bitwise XOR A and operand, store in A
	Manual Page: 86
*/
void Z80::xor_n(uint8_t operand)
{
	bitwise_xor(operand);
}

/*
	OpCode:      0xEF
	Description: Push PC onto stack, jump to 0x0028
	Manual Page: 116
*/
void Z80::rst_28(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.pc);
	registers.pc = 0x0028;
}

/*
	OpCode:      0xF0
	Description: Load value at 0xFF00 + operand into A
	Manual Page: 75
*/
void Z80::ld_ff_ap_n(uint8_t operand)
{
	registers.a = memory->read_byte(0xFF00 + operand);
}

/*
	OpCode:      0xF1
	Description: Pop two bytes from stack, store in af
	Manual Page: 79
*/
void Z80::pop_af(void)
{
	registers.af = memory->read_short(registers.sp);
	registers.sp += 2;
}

/*
	OpCode:      0xF2
	Description: Load value at 0xFF00 + C into A
	Manual Page: 70
*/
void Z80::ld_a_ff_c(void)
{
	registers.a = memory->read_byte(0xFF00 + registers.c);
}

/*
	OpCode:      0xF3
	Description: Disable interrupts
	Manual Page: 98
*/
void Z80::di_inst(void)
{
	disable_ime_flag = 0x01;
}

/*
	OpCode:      0xF4
	Description: Unknown Operation
	Manual Page: ----
*/

/*
	OpCode:      0xF5
	Description: Push AF to Stack
	Manual Page: 78
*/
void Z80::push_af(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.af);
}

/*
	OpCode:      0xF6
	Description: Bitwise OR A and operand, store in A
	Manual Page: 86
*/
void Z80::or_n(uint8_t operand)
{
	bitwise_or(operand);
}

/*
	OpCode:      0xF7
	Description: Push PC onto stack, jump to 0x0030
	Manual Page: 116
*/
void Z80::rst_30(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.pc);
	registers.pc = 0x0030;
}

/*
	OpCode:      0xF8
	Description: Store SP + (signed) operand into HL
	Manual Page: 77
*/
void Z80::ld_hl_sp_n(uint8_t operand)
{
	uint32_t result = registers.sp + (int8_t) (operand);
	
	if (result & 0xFFFF0000)
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	
	if (((registers.sp & 0x0F) + (operand & 0x0F)) > 0x0F)
	{
		FLAGS_SET(FLAGS_HALFCARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_HALFCARRY);
	}
	
	FLAGS_CLEAR(FLAGS_ZERO | FLAGS_NEGATIVE);
	
	registers.hl = (uint16_t) (result & 0xFFFF);
}

/*
	OpCode:      0xF9
	Description: Load HL into SP
	Manual Page: 76
*/
void Z80::ld_sp_hl(void)
{
	registers.sp = registers.hl;
}

/*
	OpCode:      0xFA
	Description: Load value (at address operand) into A
	Manual Page: 68
*/
void Z80::ld_a_nnp(uint16_t operand)
{
	registers.a = memory->read_byte(operand);
}

/*
	OpCode:      0xFB
	Description: Enable interrupts
	Manual Page: 98
*/
void Z80::ei(void)
{
	set_ime_flag = 0x01;
}

/*
	OpCode:      0xFC
	Description: Unknown Operation
	Manual Page: ----
*/

/*
	OpCode:      0xFD
	Description: Unknown Operation
	Manual Page: ----
*/

/*
	OpCode:      0xFE
	Description: Compare A with operand. Basically subtraction, but result is tossed
	Manual Page: 87
*/
void Z80::cp_n(uint8_t operand)
{
	compare(operand);
}

/*
	OpCode:      0xFF
	Description: Push PC onto stack, jump to 0x0038
	Manual Page: 116
*/
void Z80::rst_38(void)
{
	registers.sp -= 2;
	memory->write_short(registers.sp, registers.pc);
	registers.pc = 0x0038;
}

/* START EXTENDED OP CODES */

uint8_t Z80::rlc(uint8_t value)
{
	uint8_t carry = (value & 0x80) >> 7;
	
	if (value & 0x80)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	value <<= 1;
	value += carry;
	
	if (value)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

uint8_t Z80::rrc(uint8_t value)
{
	uint8_t carry = value & 0x01;
	
	value >>= 1;
	
	if (carry)
	{
		FLAGS_SET(FLAGS_CARRY);
		value |= 0x80;
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	if (value)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

uint8_t Z80::rl(uint8_t value)
{
	if (value & 0x80)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	value <<= 1;
	if (FLAGS_ISCARRY)
	{
		value++;
	}
	
	if (value)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

uint8_t Z80::rr(uint8_t value)
{
	value >>= 1;
	if (FLAGS_ISCARRY)
	{
		value |= 0x80;
	}
	
	if (value & 0x01)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	if (value)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

uint8_t Z80::sla(uint8_t value)
{
	if (value & 0x80)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	value <<= 1;
	
	if (value)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

uint8_t Z80::sra(uint8_t value)
{
	if (value & 0x01)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	value = (value & 0x80) | (value >> 1);
	
	if (value)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

uint8_t Z80::swap(uint8_t value)
{
	value = ((value &0x0F) << 4) | ((value & 0xF0) >> 4);
	
	if (value)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY | FLAGS_CARRY);
	
	return value;
}

uint8_t Z80::srl(uint8_t value)
{
	if (value & 0x01)
	{
		FLAGS_SET(FLAGS_CARRY);
	}
	else
	{
		FLAGS_CLEAR(FLAGS_CARRY);
	}
	
	value >>= 1;
	
	if (value)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
	
	return value;
}

void Z80::bit(uint8_t bit, uint8_t value)
{
	if (value & bit)
	{
		FLAGS_CLEAR(FLAGS_ZERO);
	}
	else
	{
		FLAGS_SET(FLAGS_ZERO);
	}
	
	FLAGS_CLEAR(FLAGS_NEGATIVE);
	FLAGS_SET(FLAGS_HALFCARRY);
}

uint8_t Z80::set(uint8_t bit, uint8_t value)
{
	return value |= bit;
}

void Z80::cb_rlc_b(void)
{
	registers.b = rlc(registers.b);
}

void Z80::cb_rlc_c(void)
{
	registers.c = rlc(registers.c);
}

void Z80::cb_rlc_d(void)
{
	registers.d = rlc(registers.d);
}

void Z80::cb_rlc_e(void)
{
	registers.e = rlc(registers.e);
}

void Z80::cb_rlc_h(void)
{
	registers.h = rlc(registers.h);
}

void Z80::cb_rlc_l(void)
{
	registers.l = rlc(registers.l);
}

void Z80::cb_rlc_hlp(void)
{
	memory->write_byte(registers.hl, rlc(memory->read_byte(registers.hl)));
}

void Z80::cb_rlc_a(void)
{
	registers.a = rlc(registers.a);
}

void Z80::cb_rrc_b(void)
{
	registers.b = rrc(registers.b);
}

void Z80::cb_rrc_c(void)
{
	registers.c = rrc(registers.c);
}

void Z80::cb_rrc_d(void)
{
	registers.d = rrc(registers.d);
}

void Z80::cb_rrc_e(void)
{
	registers.e = rrc(registers.e);
}

void Z80::cb_rrc_h(void)
{
	registers.h = rrc(registers.h);
}

void Z80::cb_rrc_l(void)
{
	registers.l = rrc(registers.l);
}

void Z80::cb_rrc_hlp(void)
{
	memory->write_byte(registers.hl, rrc(memory->read_byte(registers.hl)));
}

void Z80::cb_rrc_a(void)
{
	registers.a = rrc(registers.a);
}

void Z80::cb_rl_b(void)
{
	registers.b = rl(registers.b);
}

void Z80::cb_rl_c(void)
{
	registers.c = rl(registers.c);
}

void Z80::cb_rl_d(void)
{
	registers.d = rl(registers.d);
}

void Z80::cb_rl_e(void)
{
	registers.e = rl(registers.e);
}

void Z80::cb_rl_h(void)
{
	registers.h = rl(registers.h);
}

void Z80::cb_rl_l(void)
{
	registers.l = rl(registers.l);
}

void Z80::cb_rl_hlp(void)
{
	memory->write_byte(registers.hl, rl(memory->read_byte(registers.hl)));
}

void Z80::cb_rl_a(void)
{
	registers.a = rl(registers.a);
}

void Z80::cb_rr_b(void)
{
	registers.b = rr(registers.b);
}

void Z80::cb_rr_c(void)
{
	registers.c = rr(registers.c);
}

void Z80::cb_rr_d(void)
{
	registers.d = rr(registers.d);
}

void Z80::cb_rr_e(void)
{
	registers.e = rr(registers.e);
}

void Z80::cb_rr_h(void)
{
	registers.h = rr(registers.h);
}

void Z80::cb_rr_l(void)
{
	registers.l = rr(registers.l);
}

void Z80::cb_rr_hlp(void)
{
	memory->write_byte(registers.hl, rr(memory->read_byte(registers.hl)));
}

void Z80::cb_rr_a(void)
{
	registers.a = rr(registers.a);
}

void Z80::cb_sla_b(void)
{
	registers.b = sla(registers.b);
}

void Z80::cb_sla_c(void)
{
	registers.c = sla(registers.c);
}

void Z80::cb_sla_d(void)
{
	registers.d = sla(registers.d);
}

void Z80::cb_sla_e(void)
{
	registers.e = sla(registers.e);
}

void Z80::cb_sla_h(void)
{
	registers.h = sla(registers.h);
}

void Z80::cb_sla_l(void)
{
	registers.l = sla(registers.l);
}

void Z80::cb_sla_hlp(void)
{
	memory->write_byte(registers.hl, sla(memory->read_byte(registers.hl)));
}

void Z80::cb_sla_a(void)
{
	registers.a = sla(registers.a);
}

void Z80::cb_sra_b(void)
{
	registers.b = sra(registers.b);
}

void Z80::cb_sra_c(void)
{
	registers.c = sra(registers.c);
}

void Z80::cb_sra_d(void)
{
	registers.d = sra(registers.d);
}

void Z80::cb_sra_e(void)
{
	registers.e = sra(registers.e);
}

void Z80::cb_sra_h(void)
{
	registers.h = sra(registers.h);
}

void Z80::cb_sra_l(void)
{
	registers.l = sra(registers.l);
}

void Z80::cb_sra_hlp(void)
{
	memory->write_byte(registers.hl, sra(registers.hl));
}

void Z80::cb_sra_a(void)
{
	registers.a = sra(registers.a);
}

void Z80::cb_swap_b(void)
{
	registers.b = swap(registers.b);
}

void Z80::cb_swap_c(void)
{
	registers.c = swap(registers.c);
}

void Z80::cb_swap_d(void)
{
	registers.d = swap(registers.d);
}

void Z80::cb_swap_e(void)
{
	registers.e = swap(registers.e);
}

void Z80::cb_swap_h(void)
{
	registers.h = swap(registers.h);
}

void Z80::cb_swap_l(void)
{
	registers.l = swap(registers.l);
}

void Z80::cb_swap_hlp(void)
{
	memory->write_byte(registers.hl, swap(memory->read_byte(registers.hl)));
}

void Z80::cb_swap_a(void)
{
	registers.a = swap(registers.a);
}

void Z80::cb_srl_b(void)
{
	registers.b = srl(registers.b);
}

void Z80::cb_srl_c(void)
{
	registers.c = srl(registers.c);
}

void Z80::cb_srl_d(void)
{
	registers.d = srl(registers.d);
}

void Z80::cb_srl_e(void)
{
	registers.e = srl(registers.e);
}

void Z80::cb_srl_h(void)
{
	registers.h = srl(registers.h);
}

void Z80::cb_srl_l(void)
{
	registers.l = srl(registers.l);
}

void Z80::cb_srl_hlp(void)
{
	memory->write_byte(registers.hl, srl(memory->read_byte(registers.hl)));
}

void Z80::cb_srl_a(void)
{
	/*if(registers.a & 0x01) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);
	
	registers.a >>= 1;
	
	if(registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);
	
	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);*/
	registers.a = srl(registers.a);
}

void Z80::cb_bit_0_b(void)
{
	bit(1 << 0, registers.b);
}

void Z80::cb_bit_0_c(void)
{
	bit(1 << 0, registers.c);
}

void Z80::cb_bit_0_d(void)
{
	bit(1 << 0, registers.d);
}

void Z80::cb_bit_0_e(void)
{
	bit(1 << 0, registers.e);
}

void Z80::cb_bit_0_h(void)
{
	bit(1 << 0, registers.h);
}

void Z80::cb_bit_0_l(void)
{
	bit(1 << 0, registers.l);
}

void Z80::cb_bit_0_hlp(void)
{
	bit(1 << 0, memory->read_byte(registers.hl));
}

void Z80::cb_bit_0_a(void)
{
	bit(1 << 0, registers.a);
}

void Z80::cb_bit_1_b(void)
{
	bit(1 << 1, registers.b);
}

void Z80::cb_bit_1_c(void)
{
	bit(1 << 1, registers.c);
}

void Z80::cb_bit_1_d(void)
{
	bit(1 << 1, registers.d);
}

void Z80::cb_bit_1_e(void)
{
	bit(1 << 1, registers.e);
}

void Z80::cb_bit_1_h(void)
{
	bit(1 << 1, registers.h);
}

void Z80::cb_bit_1_l(void)
{
	bit(1 << 1, registers.l);
}

void Z80::cb_bit_1_hlp(void)
{
	bit(1 << 1, memory->read_byte(registers.hl));
}

void Z80::cb_bit_1_a(void)
{
	bit(1 << 1, registers.a);
}

void Z80::cb_bit_2_b(void)
{
	bit(1 << 2, registers.b);
}

void Z80::cb_bit_2_c(void)
{
	bit(1 << 2, registers.c);
}

void Z80::cb_bit_2_d(void)
{
	bit(1 << 2, registers.d);
}

void Z80::cb_bit_2_e(void)
{
	bit(1 << 2, registers.e);
}

void Z80::cb_bit_2_h(void)
{
	bit(1 << 2, registers.h);
}

void Z80::cb_bit_2_l(void)
{
	bit(1 << 2, registers.l);
}

void Z80::cb_bit_2_hlp(void)
{
	bit(1 << 2, memory->read_byte(registers.hl));
}

void Z80::cb_bit_2_a(void)
{
	bit(1 << 2, registers.a);
}

void Z80::cb_bit_3_b(void)
{
	bit(1 << 3, registers.b);
}

void Z80::cb_bit_3_c(void)
{
	bit(1 << 3, registers.c);
}

void Z80::cb_bit_3_d(void)
{
	bit(1 << 3, registers.d);
}

void Z80::cb_bit_3_e(void)
{
	bit(1 << 3, registers.e);
}

void Z80::cb_bit_3_h(void)
{
	bit(1 << 3, registers.h);
}

void Z80::cb_bit_3_l(void)
{
	bit(1 << 3, registers.l);
}

void Z80::cb_bit_3_hlp(void)
{
	bit(1 << 3, memory->read_byte(registers.hl));
}

void Z80::cb_bit_3_a(void)
{
	bit(1 << 3, registers.a);
}

void Z80::cb_bit_4_b(void)
{
	bit(1 << 4, registers.b);
}

void Z80::cb_bit_4_c(void)
{
	bit(1 << 4, registers.c);
}

void Z80::cb_bit_4_d(void)
{
	bit(1 << 4, registers.d);
}

void Z80::cb_bit_4_e(void)
{
	bit(1 << 4, registers.e);
}

void Z80::cb_bit_4_h(void)
{
	bit(1 << 4, registers.h);
}

void Z80::cb_bit_4_l(void)
{
	bit(1 << 4, registers.l);
}

void Z80::cb_bit_4_hlp(void)
{
	bit(1 << 4, memory->read_byte(registers.hl));
}

void Z80::cb_bit_4_a(void)
{
	bit(1 << 4, registers.a);
}

void Z80::cb_bit_5_b(void)
{
	bit(1 << 5, registers.b);
}

void Z80::cb_bit_5_c(void)
{
	bit(1 << 5, registers.c);
}

void Z80::cb_bit_5_d(void)
{
	bit(1 << 5, registers.d);
}

void Z80::cb_bit_5_e(void)
{
	bit(1 << 5, registers.e);
}

void Z80::cb_bit_5_h(void)
{
	bit(1 << 5, registers.h);
}

void Z80::cb_bit_5_l(void)
{
	bit(1 << 5, registers.l);
}

void Z80::cb_bit_5_hlp(void)
{
	bit(1 << 5, memory->read_byte(registers.hl));
}

void Z80::cb_bit_5_a(void)
{
	bit(1 << 5, registers.a);
}

void Z80::cb_bit_6_b(void)
{
	bit(1 << 6, registers.b);
}

void Z80::cb_bit_6_c(void)
{
	bit(1 << 6, registers.c);
}

void Z80::cb_bit_6_d(void)
{
	bit(1 << 6, registers.d);
}

void Z80::cb_bit_6_e(void)
{
	bit(1 << 6, registers.e);
}

void Z80::cb_bit_6_h(void)
{
	bit(1 << 6, registers.h);
}

void Z80::cb_bit_6_l(void)
{
	bit(1 << 6, registers.l);
}

void Z80::cb_bit_6_hlp(void)
{
	bit(1 << 6, memory->read_byte(registers.hl));
}

void Z80::cb_bit_6_a(void)
{
	bit(1 << 6, registers.a);
}

void Z80::cb_bit_7_b(void)
{
	bit(1 << 7, registers.b);
}

void Z80::cb_bit_7_c(void)
{
	bit(1 << 7, registers.c);
}

void Z80::cb_bit_7_d(void)
{
	bit(1 << 7, registers.d);
}

void Z80::cb_bit_7_e(void)
{
	bit(1 << 7, registers.e);
}

void Z80::cb_bit_7_h(void)
{
	bit(1 << 7, registers.h);
}

void Z80::cb_bit_7_l(void)
{
	bit(1 << 7, registers.l);
}

void Z80::cb_bit_7_hlp(void)
{
	bit(1 << 7, memory->read_byte(registers.hl));
}

void Z80::cb_bit_7_a(void)
{
	bit(1 << 7, registers.a);
}

void Z80::cb_res_0_b(void)
{
	registers.b &= ~(1 << 0);
}

void Z80::cb_res_0_c(void)
{
	registers.c &= ~(1 << 0);
}

void Z80::cb_res_0_d(void)
{
	registers.d &= ~(1 << 0);
}

void Z80::cb_res_0_e(void)
{
	registers.e &= ~(1 << 0);
}

void Z80::cb_res_0_h(void)
{
	registers.h &= ~(1 << 0);
}

void Z80::cb_res_0_l(void)
{
	registers.l &= ~(1 << 0);
}

void Z80::cb_res_0_hlp(void)
{
	memory->write_byte(registers.hl, memory->read_byte(registers.hl) & ~(1 << 0));
}

void Z80::cb_res_0_a(void)
{
	registers.a &= ~(1 << 0);
}

void Z80::cb_res_1_b(void)
{
	registers.b &= ~(1 << 1);
}

void Z80::cb_res_1_c(void)
{
	registers.c &= ~(1 << 1);
}

void Z80::cb_res_1_d(void)
{
	registers.d &= ~(1 << 1);
}

void Z80::cb_res_1_e(void)
{
	registers.e &= ~(1 << 1);
}

void Z80::cb_res_1_h(void)
{
	registers.h &= ~(1 << 1);
}
void Z80::cb_res_1_l(void)
{
	registers.l &= ~(1 << 1);
}

void Z80::cb_res_1_hlp(void)
{
	memory->write_byte(registers.hl, registers.hl & ~(1 << 1));
}

void Z80::cb_res_1_a(void)
{
	registers.a &= ~(1 << 1);
}

void Z80::cb_res_2_b(void)
{
	registers.b &= ~(1 << 2);
}

void Z80::cb_res_2_c(void)
{
	registers.c &= ~(1 << 2);
}

void Z80::cb_res_2_d(void)
{
	registers.d &= ~(1 << 2);
}

void Z80::cb_res_2_e(void)
{
	registers.e &= ~(1 << 2);
}

void Z80::cb_res_2_h(void)
{
	registers.h &= ~(1 << 2);
}

void Z80::cb_res_2_l(void)
{
	registers.l &= ~(1 << 2);
}

void Z80::cb_res_2_hlp(void)
{
	memory->write_byte(registers.hl, registers.hl & ~(1 << 2));
}

void Z80::cb_res_2_a(void)
{
	registers.a &= ~(1 << 2);
}

void Z80::cb_res_3_b(void)
{
	registers.b &= ~(1 << 3);
}

void Z80::cb_res_3_c(void)
{
	registers.c &= ~(1 << 3);
}

void Z80::cb_res_3_d(void)
{
	registers.d &= ~(1 << 3);
}

void Z80::cb_res_3_e(void)
{
	registers.e &= ~(1 << 3);
}

void Z80::cb_res_3_h(void)
{
	registers.h &= ~(1 << 3);
}

void Z80::cb_res_3_l(void)
{
	registers.l &= ~(1 << 3);
}

void Z80::cb_res_3_hlp(void)
{
	memory->write_byte(registers.hl, registers.hl & ~(1 << 3));
}

void Z80::cb_res_3_a(void)
{
	registers.a &= ~(1 << 3);
}

void Z80::cb_res_4_b(void)
{
	registers.b &= ~(1 << 4);
}

void Z80::cb_res_4_c(void)
{
	registers.c &= ~(1 << 4);
}

void Z80::cb_res_4_d(void)
{
	registers.d &= ~(1 << 4);
}

void Z80::cb_res_4_e(void)
{
	registers.e &= ~(1 << 4);
}

void Z80::cb_res_4_h(void)
{
	registers.h &= ~(1 << 4);
}

void Z80::cb_res_4_l(void)
{
	registers.l &= ~(1 << 4);
}

void Z80::cb_res_4_hlp(void)
{
	memory->write_byte(registers.hl, registers.hl & ~(1 << 4));
}

void Z80::cb_res_4_a(void)
{
	registers.a &= ~(1 << 4);
}

void Z80::cb_res_5_b(void)
{
	registers.b &= ~(1 << 5);
}

void Z80::cb_res_5_c(void)
{
	registers.c &= ~(1 << 5);
}

void Z80::cb_res_5_d(void)
{
	registers.d &= ~(1 << 5);
}

void Z80::cb_res_5_e(void)
{
	registers.e &= ~(1 << 5);
}

void Z80::cb_res_5_h(void)
{
	registers.h &= ~(1 << 5);
}

void Z80::cb_res_5_l(void)
{
	registers.l &= ~(1 << 5);
}

void Z80::cb_res_5_hlp(void)
{
	memory->write_byte(registers.hl, registers.hl & ~(1 << 5));
}

void Z80::cb_res_5_a(void)
{
	registers.a &= ~(1 << 5);
}

void Z80::cb_res_6_b(void)
{
	registers.b &= ~(1 << 6);
}

void Z80::cb_res_6_c(void)
{
	registers.c &= ~(1 << 6);
}

void Z80::cb_res_6_d(void)
{
	registers.d &= ~(1 << 6);
}

void Z80::cb_res_6_e(void)
{
	registers.e &= ~(1 << 6);
}

void Z80::cb_res_6_h(void)
{
	registers.h &= ~(1 << 6);
}

void Z80::cb_res_6_l(void)
{
	registers.l &= ~(1 << 6);
}

void Z80::cb_res_6_hlp(void)
{
	memory->write_byte(registers.hl, registers.hl & ~(1 << 6));
}

void Z80::cb_res_6_a(void)
{
	registers.a &= ~(1 << 6);
}

void Z80::cb_res_7_b(void)
{
	registers.b &= ~(1 << 7);
}

void Z80::cb_res_7_c(void)
{
	registers.c &= ~(1 << 7);
}

void Z80::cb_res_7_d(void)
{
	registers.d &= ~(1 << 7);
}

void Z80::cb_res_7_e(void)
{
	registers.e &= ~(1 << 7);
}

void Z80::cb_res_7_h(void)
{
	registers.h &= ~(1 << 7);
}

void Z80::cb_res_7_l(void)
{
	registers.l &= ~(1 << 7);
}

void Z80::cb_res_7_hlp(void)
{
	memory->write_byte(registers.hl, memory->read_byte(registers.hl) & ~(1 << 7));
}

void Z80::cb_res_7_a(void)
{
	registers.a &= ~(1 << 7);
}

void Z80::cb_set_0_b(void)
{
	registers.b = set(1 << 0, registers.b);
}

void Z80::cb_set_0_c(void)
{
	registers.c = set(1 << 0, registers.c);
}

void Z80::cb_set_0_d(void)
{
	registers.d = set(1 << 0, registers.d);
}

void Z80::cb_set_0_e(void)
{
	registers.e = set(1 << 0, registers.e);
}

void Z80::cb_set_0_h(void)
{
	registers.h = set(1 << 0, registers.h);
}

void Z80::cb_set_0_l(void)
{
	registers.l = set(1 << 0, registers.l);
}

void Z80::cb_set_0_hlp(void)
{
	memory->write_byte(registers.hl, set(1 << 0, memory->read_byte(registers.hl)));
}

void Z80::cb_set_0_a(void)
{
	registers.a = set(1 << 0, registers.a);
}

void Z80::cb_set_1_b(void)
{
	registers.b = set(1 << 1, registers.b);
}

void Z80::cb_set_1_c(void)
{
	registers.c = set(1 << 1, registers.c);
}

void Z80::cb_set_1_d(void)
{
	registers.d = set(1 << 1, registers.d);
}

void Z80::cb_set_1_e(void)
{
	registers.e = set(1 << 1, registers.e);
}

void Z80::cb_set_1_h(void)
{
	registers.h = set(1 << 1, registers.h);
}

void Z80::cb_set_1_l(void)
{
	registers.l = set(1 << 1, registers.l);
}

void Z80::cb_set_1_hlp(void)
{
	memory->write_byte(registers.hl, set(1 << 1, memory->read_byte(registers.hl)));
}

void Z80::cb_set_1_a(void)
{
	registers.a = set(1 << 1, registers.a);
}

void Z80::cb_set_2_b(void)
{
	registers.b = set(1 << 2, registers.b);
}

void Z80::cb_set_2_c(void)
{
	registers.c = set(1 << 2, registers.c);
}

void Z80::cb_set_2_d(void)
{
	registers.d = set(1 << 2, registers.d);
}

void Z80::cb_set_2_e(void)
{
	registers.e = set(1 << 2, registers.e);
}

void Z80::cb_set_2_h(void)
{
	registers.h = set(1 << 2, registers.h);
}

void Z80::cb_set_2_l(void)
{
	registers.l = set(1 << 2, registers.l);
}

void Z80::cb_set_2_hlp(void)
{
	memory->write_byte(registers.hl, set(1 << 2, memory->read_byte(registers.hl)));
}

void Z80::cb_set_2_a(void)
{
	registers.a = set(1 << 2, registers.a);
}

void Z80::cb_set_3_b(void)
{
	registers.b = set(1 << 3, registers.b);
}

void Z80::cb_set_3_c(void)
{
	registers.c = set(1 << 3, registers.c);
}

void Z80::cb_set_3_d(void)
{
	registers.d = set(1 << 3, registers.d);
}

void Z80::cb_set_3_e(void)
{
	registers.e = set(1 << 3, registers.e);
}

void Z80::cb_set_3_h(void)
{
	registers.h = set(1 << 3, registers.h);
}

void Z80::cb_set_3_l(void)
{
	registers.l = set(1 << 3, registers.l);
}

void Z80::cb_set_3_hlp(void)
{
	memory->write_byte(registers.hl, set(1 << 3, memory->read_byte(registers.hl)));
}

void Z80::cb_set_3_a(void)
{
	registers.a = set(1 << 3, registers.a);
}

void Z80::cb_set_4_b(void)
{
	registers.b = set(1 << 4, registers.b);
}

void Z80::cb_set_4_c(void)
{
	registers.c = set(1 << 4, registers.c);
}

void Z80::cb_set_4_d(void)
{
	registers.d = set(1 << 4, registers.d);
}

void Z80::cb_set_4_e(void)
{
	registers.e = set(1 << 4, registers.e);
}

void Z80::cb_set_4_h(void)
{
	registers.h = set(1 << 4, registers.h);
}

void Z80::cb_set_4_l(void)
{
	registers.l = set(1 << 4, registers.l);
}

void Z80::cb_set_4_hlp(void)
{
	memory->write_byte(registers.hl, set(1 << 4, memory->read_byte(registers.hl)));
}

void Z80::cb_set_4_a(void)
{
	registers.a = set(1 << 4, registers.a);
}

void Z80::cb_set_5_b(void)
{
	registers.b = set(1 << 5, registers.b);
}

void Z80::cb_set_5_c(void)
{
	registers.c = set(1 << 5, registers.c);
}

void Z80::cb_set_5_d(void)
{
	registers.d = set(1 << 5, registers.d);
}

void Z80::cb_set_5_e(void)
{
	registers.e = set(1 << 5, registers.e);
}

void Z80::cb_set_5_h(void)
{
	registers.h = set(1 << 5, registers.h);
}

void Z80::cb_set_5_l(void)
{
	registers.l = set(1 << 5, registers.l);
}

void Z80::cb_set_5_hlp(void)
{
	memory->write_byte(registers.hl, set(1 << 5, memory->read_byte(registers.hl)));
}

void Z80::cb_set_5_a(void)
{
	registers.a = set(1 << 5, registers.a);
}

void Z80::cb_set_6_b(void)
{
	registers.b = set(1 << 6, registers.b);
}

void Z80::cb_set_6_c(void)
{
	registers.c = set(1 << 6, registers.c);
}

void Z80::cb_set_6_d(void)
{
	registers.d = set(1 << 6, registers.d);
}

void Z80::cb_set_6_e(void)
{
	registers.e = set(1 << 6, registers.e);
}

void Z80::cb_set_6_h(void)
{
	registers.h = set(1 << 6, registers.h);
}

void Z80::cb_set_6_l(void)
{
	registers.l = set(1 << 6, registers.l);
}

void Z80::cb_set_6_hlp(void)
{
	memory->write_byte(registers.hl, set(1 << 6, memory->read_byte(registers.hl)));
}

void Z80::cb_set_6_a(void)
{
	registers.a = set(1 << 6, registers.a);
}

void Z80::cb_set_7_b(void)
{
	registers.b = set(1 << 7, registers.b);
}

void Z80::cb_set_7_c(void)
{
	registers.c = set(1 << 7, registers.c);
}

void Z80::cb_set_7_d(void)
{
	registers.d = set(1 << 7, registers.d);
}

void Z80::cb_set_7_e(void)
{
	registers.e = set(1 << 7, registers.e);
}

void Z80::cb_set_7_h(void)
{
	registers.h = set(1 << 7, registers.h);
}

void Z80::cb_set_7_l(void)
{
	registers.l = set(1 << 7, registers.l);
}

void Z80::cb_set_7_hlp(void)
{
	memory->write_byte(registers.hl, set(1 << 7, memory->read_byte(registers.hl)));
}

void Z80::cb_set_7_a(void)
{
	registers.a = set(1 << 7, registers.a);
}