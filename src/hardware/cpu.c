/*
 * COZIBOY by Sofiane Djerbi (@Kugge)
 * Source code and licence available on Github.
 * https://github.com/Kugge/Coziboy
 */
/*
 * CPU functions implemented following gameboy Pan Docs
 * https://gbdev.io/pandocs/
 *
 * CPU opcodes implemented following pastraiser opcodes table
 * https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
 */
#include <stdio.h>
#include "cpu.h"

struct registers registers;

void print_registers() {
    printf("A: 0x%02X  F: 0x%02X\n", registers.a, registers.f);
    printf("B: 0x%02X  C: 0x%02X\n", registers.b, registers.c);
    printf("D: 0x%02X  E: 0x%02X\n", registers.d, registers.e);
    printf("H: 0x%02X  L: 0x%02X\n", registers.h, registers.l);
    printf("SP: 0x%04X\nPC: 0x%04X\n", registers.sp, registers.pc);
}

void reset() {
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
}

/********* CPU INSTRUCTIONS **********/

// UNDEFINED instruction: Debug error
void undefined(unsigned char val){
    printf("!! UNDEFINED INSTRUCTION 0x%04X !!", val);
}

// NOP Instruction: Do nothing
void nop(){}

// CPU instruction set in gameboy assembly (with functions).
// Those functions are available below.
// Generated with a python script:
// https://gist.github.com/Kugge/6bca056cbf221570c16f109ecfd72ae7
/*
const struct instruction instructions[256] = {
    {"NOP", 0, 4, nop},                      // 0x0
    {"LD BC,0x%04X", 2, 12, ld_bc_d16},      // 0x1
    {"LD (BC),A", 0, 8, ld_bc_a},            // 0x2
    {"INC BC", 0, 8, inc_bc},                // 0x3
    {"INC B", 0, 4, inc_b},                  // 0x4
    {"DEC B", 0, 4, dec_b},                  // 0x5
    {"LD B,0x%02X", 1, 8, ld_b_d8},          // 0x6
    {"RLCA", 0, 4, rlca},                    // 0x7
    {"LD (0x%04X),SP", 2, 20, ld_a16_sp},    // 0x8
    {"ADD HL,BC", 0, 8, add_hl_bc},          // 0x9
    {"LD A,(BC)", 0, 8, ld_a_bc},            // 0xa
    {"DEC BC", 0, 8, dec_bc},                // 0xb
    {"INC C", 0, 4, inc_c},                  // 0xc
    {"DEC C", 0, 4, dec_c},                  // 0xd
    {"LD C,0x%02X", 1, 8, ld_c_d8},          // 0xe
    {"RRCA", 0, 4, rrca},                    // 0xf
    {"STOP 0", 0, 4, stop_0},                // 0x10
    {"LD DE,0x%04X", 2, 12, ld_de_d16},      // 0x11
    {"LD (DE),A", 0, 8, ld_de_a},            // 0x12
    {"INC DE", 0, 8, inc_de},                // 0x13
    {"INC D", 0, 4, inc_d},                  // 0x14
    {"DEC D", 0, 4, dec_d},                  // 0x15
    {"LD D,0x%02X", 1, 8, ld_d_d8},          // 0x16
    {"RLA", 0, 4, rla},                      // 0x17
    {"JR 0x%02X", 1, 12, jr_r8},             // 0x18
    {"ADD HL,DE", 0, 8, add_hl_de},          // 0x19
    {"LD A,(DE)", 0, 8, ld_a_de},            // 0x1a
    {"DEC DE", 0, 8, dec_de},                // 0x1b
    {"INC E", 0, 4, inc_e},                  // 0x1c
    {"DEC E", 0, 4, dec_e},                  // 0x1d
    {"LD E,0x%02X", 1, 8, ld_e_d8},          // 0x1e
    {"RRA", 0, 4, rra},                      // 0x1f
    {"JR NZ,0x%02X", 1, 8, jr_nz_r8},        // 0x20
    {"LD HL,0x%04X", 2, 12, ld_hl_d16},      // 0x21
    {"LD (HL+),A", 0, 8, ld_hlp_a},          // 0x22
    {"INC HL", 0, 8, inc_hl},                // 0x23
    {"INC H", 0, 4, inc_h},                  // 0x24
    {"DEC H", 0, 4, dec_h},                  // 0x25
    {"LD H,0x%02X", 1, 8, ld_h_d8},          // 0x26
    {"DAA", 0, 4, daa},                      // 0x27
    {"JR Z,0x%02X", 1, 8, jr_z_r8},          // 0x28
    {"ADD HL,HL", 0, 8, add_hl_hl},          // 0x29
    {"LD A,(HL+)", 0, 8, ld_a_hlp},          // 0x2a
    {"DEC HL", 0, 8, dec_hl},                // 0x2b
    {"INC L", 0, 4, inc_l},                  // 0x2c
    {"DEC L", 0, 4, dec_l},                  // 0x2d
    {"LD L,0x%02X", 1, 8, ld_l_d8},          // 0x2e
    {"CPL", 0, 4, cpl},                      // 0x2f
    {"JR NC,0x%02X", 1, 8, jr_nc_r8},        // 0x30
    {"LD SP,0x%04X", 2, 12, ld_sp_d16},      // 0x31
    {"LD (HL-),A", 0, 8, ld_hlm_a},          // 0x32
    {"INC SP", 0, 8, inc_sp},                // 0x33
    {"INC (HL)", 0, 12, inc_hl},             // 0x34
    {"DEC (HL)", 0, 12, dec_hl},             // 0x35
    {"LD (HL),0x%02X", 1, 12, ld_hl_d8},     // 0x36
    {"SCF", 0, 4, scf},                      // 0x37
    {"JR C,0x%02X", 1, 8, jr_c_r8},          // 0x38
    {"ADD HL,SP", 0, 8, add_hl_sp},          // 0x39
    {"LD A,(HL-)", 0, 8, ld_a_hlm},          // 0x3a
    {"DEC SP", 0, 8, dec_sp},                // 0x3b
    {"INC A", 0, 4, inc_a},                  // 0x3c
    {"DEC A", 0, 4, dec_a},                  // 0x3d
    {"LD A,0x%02X", 1, 8, ld_a_d8},          // 0x3e
    {"CCF", 0, 4, ccf},                      // 0x3f
    {"LD B,B", 0, 4, ld_b_b},                // 0x40
    {"LD B,C", 0, 4, ld_b_c},                // 0x41
    {"LD B,D", 0, 4, ld_b_d},                // 0x42
    {"LD B,E", 0, 4, ld_b_e},                // 0x43
    {"LD B,H", 0, 4, ld_b_h},                // 0x44
    {"LD B,L", 0, 4, ld_b_l},                // 0x45
    {"LD B,(HL)", 0, 8, ld_b_hl},            // 0x46
    {"LD B,A", 0, 4, ld_b_a},                // 0x47
    {"LD C,B", 0, 4, ld_c_b},                // 0x48
    {"LD C,C", 0, 4, ld_c_c},                // 0x49
    {"LD C,D", 0, 4, ld_c_d},                // 0x4a
    {"LD C,E", 0, 4, ld_c_e},                // 0x4b
    {"LD C,H", 0, 4, ld_c_h},                // 0x4c
    {"LD C,L", 0, 4, ld_c_l},                // 0x4d
    {"LD C,(HL)", 0, 8, ld_c_hl},            // 0x4e
    {"LD C,A", 0, 4, ld_c_a},                // 0x4f
    {"LD D,B", 0, 4, ld_d_b},                // 0x50
    {"LD D,C", 0, 4, ld_d_c},                // 0x51
    {"LD D,D", 0, 4, ld_d_d},                // 0x52
    {"LD D,E", 0, 4, ld_d_e},                // 0x53
    {"LD D,H", 0, 4, ld_d_h},                // 0x54
    {"LD D,L", 0, 4, ld_d_l},                // 0x55
    {"LD D,(HL)", 0, 8, ld_d_hl},            // 0x56
    {"LD D,A", 0, 4, ld_d_a},                // 0x57
    {"LD E,B", 0, 4, ld_e_b},                // 0x58
    {"LD E,C", 0, 4, ld_e_c},                // 0x59
    {"LD E,D", 0, 4, ld_e_d},                // 0x5a
    {"LD E,E", 0, 4, ld_e_e},                // 0x5b
    {"LD E,H", 0, 4, ld_e_h},                // 0x5c
    {"LD E,L", 0, 4, ld_e_l},                // 0x5d
    {"LD E,(HL)", 0, 8, ld_e_hl},            // 0x5e
    {"LD E,A", 0, 4, ld_e_a},                // 0x5f
    {"LD H,B", 0, 4, ld_h_b},                // 0x60
    {"LD H,C", 0, 4, ld_h_c},                // 0x61
    {"LD H,D", 0, 4, ld_h_d},                // 0x62
    {"LD H,E", 0, 4, ld_h_e},                // 0x63
    {"LD H,H", 0, 4, ld_h_h},                // 0x64
    {"LD H,L", 0, 4, ld_h_l},                // 0x65
    {"LD H,(HL)", 0, 8, ld_h_hl},            // 0x66
    {"LD H,A", 0, 4, ld_h_a},                // 0x67
    {"LD L,B", 0, 4, ld_l_b},                // 0x68
    {"LD L,C", 0, 4, ld_l_c},                // 0x69
    {"LD L,D", 0, 4, ld_l_d},                // 0x6a
    {"LD L,E", 0, 4, ld_l_e},                // 0x6b
    {"LD L,H", 0, 4, ld_l_h},                // 0x6c
    {"LD L,L", 0, 4, ld_l_l},                // 0x6d
    {"LD L,(HL)", 0, 8, ld_l_hl},            // 0x6e
    {"LD L,A", 0, 4, ld_l_a},                // 0x6f
    {"LD (HL),B", 0, 8, ld_hl_b},            // 0x70
    {"LD (HL),C", 0, 8, ld_hl_c},            // 0x71
    {"LD (HL),D", 0, 8, ld_hl_d},            // 0x72
    {"LD (HL),E", 0, 8, ld_hl_e},            // 0x73
    {"LD (HL),H", 0, 8, ld_hl_h},            // 0x74
    {"LD (HL),L", 0, 8, ld_hl_l},            // 0x75
    {"HALT", 0, 4, halt},                    // 0x76
    {"LD (HL),A", 0, 8, ld_hl_a},            // 0x77
    {"LD A,B", 0, 4, ld_a_b},                // 0x78
    {"LD A,C", 0, 4, ld_a_c},                // 0x79
    {"LD A,D", 0, 4, ld_a_d},                // 0x7a
    {"LD A,E", 0, 4, ld_a_e},                // 0x7b
    {"LD A,H", 0, 4, ld_a_h},                // 0x7c
    {"LD A,L", 0, 4, ld_a_l},                // 0x7d
    {"LD A,(HL)", 0, 8, ld_a_hl},            // 0x7e
    {"LD A,A", 0, 4, ld_a_a},                // 0x7f
    {"ADD A,B", 0, 4, add_a_b},              // 0x80
    {"ADD A,C", 0, 4, add_a_c},              // 0x81
    {"ADD A,D", 0, 4, add_a_d},              // 0x82
    {"ADD A,E", 0, 4, add_a_e},              // 0x83
    {"ADD A,H", 0, 4, add_a_h},              // 0x84
    {"ADD A,L", 0, 4, add_a_l},              // 0x85
    {"ADD A,(HL)", 0, 8, add_a_hl},          // 0x86
    {"ADD A,A", 0, 4, add_a_a},              // 0x87
    {"ADC A,B", 0, 4, adc_a_b},              // 0x88
    {"ADC A,C", 0, 4, adc_a_c},              // 0x89
    {"ADC A,D", 0, 4, adc_a_d},              // 0x8a
    {"ADC A,E", 0, 4, adc_a_e},              // 0x8b
    {"ADC A,H", 0, 4, adc_a_h},              // 0x8c
    {"ADC A,L", 0, 4, adc_a_l},              // 0x8d
    {"ADC A,(HL)", 0, 8, adc_a_hl},          // 0x8e
    {"ADC A,A", 0, 4, adc_a_a},              // 0x8f
    {"SUB B", 0, 4, sub_b},                  // 0x90
    {"SUB C", 0, 4, sub_c},                  // 0x91
    {"SUB D", 0, 4, sub_d},                  // 0x92
    {"SUB E", 0, 4, sub_e},                  // 0x93
    {"SUB H", 0, 4, sub_h},                  // 0x94
    {"SUB L", 0, 4, sub_l},                  // 0x95
    {"SUB (HL)", 0, 8, sub_hl},              // 0x96
    {"SUB A", 0, 4, sub_a},                  // 0x97
    {"SBC A,B", 0, 4, sbc_a_b},              // 0x98
    {"SBC A,C", 0, 4, sbc_a_c},              // 0x99
    {"SBC A,D", 0, 4, sbc_a_d},              // 0x9a
    {"SBC A,E", 0, 4, sbc_a_e},              // 0x9b
    {"SBC A,H", 0, 4, sbc_a_h},              // 0x9c
    {"SBC A,L", 0, 4, sbc_a_l},              // 0x9d
    {"SBC A,(HL)", 0, 8, sbc_a_hl},          // 0x9e
    {"SBC A,A", 0, 4, sbc_a_a},              // 0x9f
    {"AND B", 0, 4, and_b},                  // 0xa0
    {"AND C", 0, 4, and_c},                  // 0xa1
    {"AND D", 0, 4, and_d},                  // 0xa2
    {"AND E", 0, 4, and_e},                  // 0xa3
    {"AND H", 0, 4, and_h},                  // 0xa4
    {"AND L", 0, 4, and_l},                  // 0xa5
    {"AND (HL)", 0, 8, and_hl},              // 0xa6
    {"AND A", 0, 4, and_a},                  // 0xa7
    {"XOR B", 0, 4, xor_b},                  // 0xa8
    {"XOR C", 0, 4, xor_c},                  // 0xa9
    {"XOR D", 0, 4, xor_d},                  // 0xaa
    {"XOR E", 0, 4, xor_e},                  // 0xab
    {"XOR H", 0, 4, xor_h},                  // 0xac
    {"XOR L", 0, 4, xor_l},                  // 0xad
    {"XOR (HL)", 0, 8, xor_hl},              // 0xae
    {"XOR A", 0, 4, xor_a},                  // 0xaf
    {"OR B", 0, 4, or_b},                    // 0xb0
    {"OR C", 0, 4, or_c},                    // 0xb1
    {"OR D", 0, 4, or_d},                    // 0xb2
    {"OR E", 0, 4, or_e},                    // 0xb3
    {"OR H", 0, 4, or_h},                    // 0xb4
    {"OR L", 0, 4, or_l},                    // 0xb5
    {"OR (HL)", 0, 8, or_hl},                // 0xb6
    {"OR A", 0, 4, or_a},                    // 0xb7
    {"CP B", 0, 4, cp_b},                    // 0xb8
    {"CP C", 0, 4, cp_c},                    // 0xb9
    {"CP D", 0, 4, cp_d},                    // 0xba
    {"CP E", 0, 4, cp_e},                    // 0xbb
    {"CP H", 0, 4, cp_h},                    // 0xbc
    {"CP L", 0, 4, cp_l},                    // 0xbd
    {"CP (HL)", 0, 8, cp_hl},                // 0xbe
    {"CP A", 0, 4, cp_a},                    // 0xbf
    {"RET NZ", 0, 8, ret_nz},                // 0xc0
    {"POP BC", 0, 12, pop_bc},               // 0xc1
    {"JP NZ,0x%04X", 2, 12, jp_nz_a16},      // 0xc2
    {"JP 0x%04X", 2, 16, jp_a16},            // 0xc3
    {"CALL NZ,0x%04X", 2, 12, call_nz_a16},  // 0xc4
    {"PUSH BC", 0, 16, push_bc},             // 0xc5
    {"ADD A,0x%02X", 1, 8, add_a_d8},        // 0xc6
    {"RST 00H", 0, 16, rst_00h},             // 0xc7
    {"RET Z", 0, 8, ret_z},                  // 0xc8
    {"RET", 0, 16, ret},                     // 0xc9
    {"JP Z,0x%04X", 2, 12, jp_z_a16},        // 0xca
    {"PREFIX CB", 0, 4, prefix_cb},          // 0xcb
    {"CALL Z,0x%04X", 2, 12, call_z_a16},    // 0xcc
    {"CALL 0x%04X", 2, 24, call_a16},        // 0xcd
    {"ADC A,0x%02X", 1, 8, adc_a_d8},        // 0xce
    {"RST 08H", 0, 16, rst_08h},             // 0xcf
    {"RET NC", 0, 8, ret_nc},                // 0xd0
    {"POP DE", 0, 12, pop_de},               // 0xd1
    {"JP NC,0x%04X", 2, 12, jp_nc_a16},      // 0xd2
    {"UNDEFINED", 0, 0, undefined}           // 0xd3
    {"CALL NC,0x%04X", 2, 12, call_nc_a16},  // 0xd4
    {"PUSH DE", 0, 16, push_de},             // 0xd5
    {"SUB 0x%02X", 1, 8, sub_d8},            // 0xd6
    {"RST 10H", 0, 16, rst_10h},             // 0xd7
    {"RET C", 0, 8, ret_c},                  // 0xd8
    {"RETI", 0, 16, reti},                   // 0xd9
    {"JP C,0x%04X", 2, 12, jp_c_a16},        // 0xda
    {"UNDEFINED", 0, 0, undefined}           // 0xdb
    {"CALL C,0x%04X", 2, 12, call_c_a16},    // 0xdc
    {"UNDEFINED", 0, 0, undefined}           // 0xdd
    {"SBC A,0x%02X", 1, 8, sbc_a_d8},        // 0xde
    {"RST 18H", 0, 16, rst_18h},             // 0xdf
    {"LDH (0x%02X),A", 1, 12, ldh_a8_a},     // 0xe0
    {"POP HL", 0, 12, pop_hl},               // 0xe1
    {"LD (C),A", 0, 8, ld_c_a},              // 0xe2
    {"UNDEFINED", 0, 0, undefined}           // 0xe3
    {"UNDEFINED", 0, 0, undefined}           // 0xe4
    {"PUSH HL", 0, 16, push_hl},             // 0xe5
    {"AND 0x%02X", 1, 8, and_d8},            // 0xe6
    {"RST 20H", 0, 16, rst_20h},             // 0xe7
    {"ADD SP,0x%02X", 1, 16, add_sp_r8},     // 0xe8
    {"JP (HL)", 0, 4, jp_hl},                // 0xe9
    {"LD (0x%04X),A", 2, 16, ld_a16_a},      // 0xea
    {"UNDEFINED", 0, 0, undefined}           // 0xeb
    {"UNDEFINED", 0, 0, undefined}           // 0xec
    {"UNDEFINED", 0, 0, undefined}           // 0xed
    {"XOR 0x%02X", 1, 8, xor_d8},            // 0xee
    {"RST 28H", 0, 16, rst_28h},             // 0xef
    {"LDH A,(0x%02X)", 1, 12, ldh_a_a8},     // 0xf0
    {"POP AF", 0, 12, pop_af},               // 0xf1
    {"LD A,(C)", 0, 8, ld_a_c},              // 0xf2
    {"DI", 0, 4, di},                        // 0xf3
    {"UNDEFINED", 0, 0, undefined}           // 0xf4
    {"PUSH AF", 0, 16, push_af},             // 0xf5
    {"OR 0x%02X", 1, 8, or_d8},              // 0xf6
    {"RST 30H", 0, 16, rst_30h},             // 0xf7
    {"LD HL,SP+0x%02X", 1, 12, ld_hl_sppr8}, // 0xf8
    {"LD SP,HL", 0, 8, ld_sp_hl},            // 0xf9
    {"LD A,(0x%04X)", 2, 16, ld_a_a16},      // 0xfa
    {"EI", 0, 4, ei},                        // 0xfb
    {"UNDEFINED", 0, 0, undefined}           // 0xfc
    {"UNDEFINED", 0, 0, undefined}           // 0xfd
    {"CP 0x%02X", 1, 8, cp_d8},              // 0xfe
    {"RST 38H", 0, 16, rst_38h},             // 0xff
};*/
