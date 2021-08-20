/* 
 * COZIBOY by Sofiane Djerbi (@Kugge)
 * Source code and licence available on Github.
 * https://github.com/Kugge/Coziboy
 */
/* 
 * CPU and assembly implemented following gameboy Pan Docs 
 * https://gbdev.io/pandocs/
 * 
 * And Pastraiser opcodes table
 * https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
 */
#include <stddef.h>

// CPU registers implemented following:
// https://gbdev.io/pandocs/CPU_Registers_and_Flags.html
struct registers {
	unsigned char a;    // 8 Bit - Hi / Accumulator
    unsigned char f;    // 8 Bit - Lo / Flag (Z N H C)
	unsigned char b;    // 8 Bit - Hi 
	unsigned char c;    // 8 Bit - Lo
	unsigned char d;    // 8 Bit - Hi
	unsigned char e;    // 8 Bit - Lo
	unsigned char h;    // 8 Bit - Hi
	unsigned char l;    // 8 Bit - Lo
	unsigned short sp;  // 16 Bit / Stack pointer 
	unsigned short pc;  // 16 Bit / Program counter
} extern registers;

// An abstract CPU instruction
struct instruction {
    char assembly[11];      // Assembly equivalent (debugging/modding)
    void (*process)(void);  // Processing function (execute the opcode)
};

// CPU instruction set in gameboy assembly (with functions).
// Those functions are available below.
// Generated with a python script:
// https://gist.github.com/Kugge/6bca056cbf221570c16f109ecfd72ae7
const struct instruction instructions[256] = {
    {"NOP", nop},                       // 0x00
    {"LD BC,d16", ld_bc_d16},           // 0x01
    {"LD (BC),A", ld_bc_a},             // 0x02
    {"INC BC", inc_bc},                 // 0x03
    {"INC B", inc_b},                   // 0x04
    {"DEC B", dec_b},                   // 0x05
    {"LD B,d8", ld_b_d8},               // 0x06
    {"RLCA", rlca},                     // 0x07
    {"LD (a16),SP", ld_a16_sp},         // 0x08
    {"ADD HL,BC", add_hl_bc},           // 0x09
    {"LD A,(BC)", ld_a_bc},             // 0x0a
    {"DEC BC", dec_bc},                 // 0x0b
    {"INC C", inc_c},                   // 0x0c
    {"DEC C", dec_c},                   // 0x0d
    {"LD C,d8", ld_c_d8},               // 0x0e
    {"RRCA", rrca},                     // 0x0f
    {"STOP 0", stop_0},                 // 0x10
    {"LD DE,d16", ld_de_d16},           // 0x11
    {"LD (DE),A", ld_de_a},             // 0x12
    {"INC DE", inc_de},                 // 0x13
    {"INC D", inc_d},                   // 0x14
    {"DEC D", dec_d},                   // 0x15
    {"LD D,d8", ld_d_d8},               // 0x16
    {"RLA", rla},                       // 0x17
    {"JR r8", jr_r8},                   // 0x18
    {"ADD HL,DE", add_hl_de},           // 0x19
    {"LD A,(DE)", ld_a_de},             // 0x1a
    {"DEC DE", dec_de},                 // 0x1b
    {"INC E", inc_e},                   // 0x1c
    {"DEC E", dec_e},                   // 0x1d
    {"LD E,d8", ld_e_d8},               // 0x1e
    {"RRA", rra},                       // 0x1f
    {"JR NZ,r8", jr_nz_r8},             // 0x20
    {"LD HL,d16", ld_hl_d16},           // 0x21
    {"LD (HL+),A", ld_hl+_a},           // 0x22
    {"INC HL", inc_hl},                 // 0x23
    {"INC H", inc_h},                   // 0x24
    {"DEC H", dec_h},                   // 0x25
    {"LD H,d8", ld_h_d8},               // 0x26
    {"DAA", daa},                       // 0x27
    {"JR Z,r8", jr_z_r8},               // 0x28
    {"ADD HL,HL", add_hl_hl},           // 0x29
    {"LD A,(HL+)", ld_a_hl+},           // 0x2a
    {"DEC HL", dec_hl},                 // 0x2b
    {"INC L", inc_l},                   // 0x2c
    {"DEC L", dec_l},                   // 0x2d
    {"LD L,d8", ld_l_d8},               // 0x2e
    {"CPL", cpl},                       // 0x2f
    {"JR NC,r8", jr_nc_r8},             // 0x30
    {"LD SP,d16", ld_sp_d16},           // 0x31
    {"LD (HL-),A", ld_hl-_a},           // 0x32
    {"INC SP", inc_sp},                 // 0x33
    {"INC (HL)", inc_hl},               // 0x34
    {"DEC (HL)", dec_hl},               // 0x35
    {"LD (HL),d8", ld_hl_d8},           // 0x36
    {"SCF", scf},                       // 0x37
    {"JR C,r8", jr_c_r8},               // 0x38
    {"ADD HL,SP", add_hl_sp},           // 0x39
    {"LD A,(HL-)", ld_a_hl-},           // 0x3a
    {"DEC SP", dec_sp},                 // 0x3b
    {"INC A", inc_a},                   // 0x3c
    {"DEC A", dec_a},                   // 0x3d
    {"LD A,d8", ld_a_d8},               // 0x3e
    {"CCF", ccf},                       // 0x3f
    {"LD B,B", ld_b_b},                 // 0x40
    {"LD B,C", ld_b_c},                 // 0x41
    {"LD B,D", ld_b_d},                 // 0x42
    {"LD B,E", ld_b_e},                 // 0x43
    {"LD B,H", ld_b_h},                 // 0x44
    {"LD B,L", ld_b_l},                 // 0x45
    {"LD B,(HL)", ld_b_hl},             // 0x46
    {"LD B,A", ld_b_a},                 // 0x47
    {"LD C,B", ld_c_b},                 // 0x48
    {"LD C,C", ld_c_c},                 // 0x49
    {"LD C,D", ld_c_d},                 // 0x4a
    {"LD C,E", ld_c_e},                 // 0x4b
    {"LD C,H", ld_c_h},                 // 0x4c
    {"LD C,L", ld_c_l},                 // 0x4d
    {"LD C,(HL)", ld_c_hl},             // 0x4e
    {"LD C,A", ld_c_a},                 // 0x4f
    {"LD D,B", ld_d_b},                 // 0x50
    {"LD D,C", ld_d_c},                 // 0x51
    {"LD D,D", ld_d_d},                 // 0x52
    {"LD D,E", ld_d_e},                 // 0x53
    {"LD D,H", ld_d_h},                 // 0x54
    {"LD D,L", ld_d_l},                // 0x55
    {"LD D,(HL)", ld_d_hl},             // 0x56
    {"LD D,A", ld_d_a},                 // 0x57
    {"LD E,B", ld_e_b},                 // 0x58
    {"LD E,C", ld_e_c},                 // 0x59
    {"LD E,D", ld_e_d},                 // 0x5a
    {"LD E,E", ld_e_e},                 // 0x5b
    {"LD E,H", ld_e_h},                 // 0x5c
    {"LD E,L", ld_e_l},                 // 0x5d
    {"LD E,(HL)", ld_e_hl},             // 0x5e
    {"LD E,A", ld_e_a},                 // 0x5f
    {"LD H,B", ld_h_b},                 // 0x60
    {"LD H,C", ld_h_c},                 // 0x61
    {"LD H,D", ld_h_d},                 // 0x62
    {"LD H,E", ld_h_e},                 // 0x63
    {"LD H,H", ld_h_h},                 // 0x64
    {"LD H,L", ld_h_l},                 // 0x65
    {"LD H,(HL)", ld_h_hl},             // 0x66
    {"LD H,A", ld_h_a},                 // 0x67
    {"LD L,B", ld_l_b},                 // 0x68
    {"LD L,C", ld_l_c},                 // 0x69
    {"LD L,D", ld_l_d},                 // 0x6a
    {"LD L,E", ld_l_e},                 // 0x6b
    {"LD L,H", ld_l_h},                 // 0x6c
    {"LD L,L", ld_l_l},                 // 0x6d
    {"LD L,(HL)", ld_l_hl},             // 0x6e
    {"LD L,A", ld_l_a},                 // 0x6f
    {"LD (HL),B", ld_hl_b},             // 0x70
    {"LD (HL),C", ld_hl_c},             // 0x71
    {"LD (HL),D", ld_hl_d},             // 0x72
    {"LD (HL),E", ld_hl_e},             // 0x73
    {"LD (HL),H", ld_hl_h},             // 0x74
    {"LD (HL),L", ld_hl_l},             // 0x75
    {"HALT", halt},                     // 0x76
    {"LD (HL),A", ld_hl_a},             // 0x77
    {"LD A,B", ld_a_b},                 // 0x78
    {"LD A,C", ld_a_c},                 // 0x79
    {"LD A,D", ld_a_d},                 // 0x7a
    {"LD A,E", ld_a_e},                 // 0x7b
    {"LD A,H", ld_a_h},                 // 0x7c
    {"LD A,L", ld_a_l},                 // 0x7d
    {"LD A,(HL)", ld_a_hl},             // 0x7e
    {"LD A,A", ld_a_a},                 // 0x7f
    {"ADD A,B", add_a_b},               // 0x80
    {"ADD A,C", add_a_c},               // 0x81
    {"ADD A,D", add_a_d},               // 0x82
    {"ADD A,E", add_a_e},               // 0x83
    {"ADD A,H", add_a_h},               // 0x84
    {"ADD A,L", add_a_l},               // 0x85
    {"ADD A,(HL)", add_a_hl},           // 0x86
    {"ADD A,A", add_a_a},               // 0x87
    {"ADC A,B", adc_a_b},               // 0x88
    {"ADC A,C", adc_a_c},               // 0x89
    {"ADC A,D", adc_a_d},               // 0x8a
    {"ADC A,E", adc_a_e},               // 0x8b
    {"ADC A,H", adc_a_h},               // 0x8c
    {"ADC A,L", adc_a_l},               // 0x8d
    {"ADC A,(HL)", adc_a_hl},           // 0x8e
    {"ADC A,A", adc_a_a},               // 0x8f
    {"SUB B", sub_b},                   // 0x90
    {"SUB C", sub_c},                   // 0x91
    {"SUB D", sub_d},                   // 0x92
    {"SUB E", sub_e},                   // 0x93
    {"SUB H", sub_h},                   // 0x94
    {"SUB L", sub_l},                   // 0x95
    {"SUB (HL)", sub_hl},               // 0x96
    {"SUB A", sub_a},                   // 0x97
    {"SBC A,B", sbc_a_b},               // 0x98
    {"SBC A,C", sbc_a_c},               // 0x99
    {"SBC A,D", sbc_a_d},               // 0x9a
    {"SBC A,E", sbc_a_e},               // 0x9b
    {"SBC A,H", sbc_a_h},               // 0x9c
    {"SBC A,L", sbc_a_l},               // 0x9d
    {"SBC A,(HL)", sbc_a_hl},           // 0x9e
    {"SBC A,A", sbc_a_a},               // 0x9f
    {"AND B", and_b},                   // 0xa0
    {"AND C", and_c},                   // 0xa1
    {"AND D", and_d},                   // 0xa2
    {"AND E", and_e},                   // 0xa3
    {"AND H", and_h},                   // 0xa4
    {"AND L", and_l},                   // 0xa5
    {"AND (HL)", and_hl},               // 0xa6
    {"AND A", and_a},                   // 0xa7
    {"XOR B", xor_b},                   // 0xa8
    {"XOR C", xor_c},                   // 0xa9
    {"XOR D", xor_d},                   // 0xaa
    {"XOR E", xor_e},                   // 0xab
    {"XOR H", xor_h},                   // 0xac
    {"XOR L", xor_l},                   // 0xad
    {"XOR (HL)", xor_hl},               // 0xae
    {"XOR A", xor_a},                   // 0xaf
    {"OR B", or_b},                     // 0xb0
    {"OR C", or_c},                     // 0xb1
    {"OR D", or_d},                     // 0xb2
    {"OR E", or_e},                     // 0xb3
    {"OR H", or_h},                     // 0xb4
    {"OR L", or_l},                     // 0xb5
    {"OR (HL)", or_hl},                 // 0xb6
    {"OR A", or_a},                     // 0xb7
    {"CP B", cp_b},                     // 0xb8
    {"CP C", cp_c},                     // 0xb9
    {"CP D", cp_d},                     // 0xba
    {"CP E", cp_e},                     // 0xbb
    {"CP H", cp_h},                     // 0xbc
    {"CP L", cp_l},                     // 0xbd
    {"CP (HL)", cp_hl},                 // 0xbe
    {"CP A", cp_a},                     // 0xbf
    {"RET NZ", ret_nz},                 // 0xc0
    {"POP BC", pop_bc},                 // 0xc1
    {"JP NZ,a16", jp_nz_a16},           // 0xc2
    {"JP a16", jp_a16},                 // 0xc3
    {"CALL NZ,a16", call_nz_a16},       // 0xc4
    {"PUSH BC", push_bc},               // 0xc5
    {"ADD A,d8", add_a_d8},             // 0xc6
    {"RST 00H", rst_00h},               // 0xc7
    {"RET Z", ret_z},                   // 0xc8
    {"RET", ret},                       // 0xc9
    {"JP Z,a16", jp_z_a16},             // 0xca
    {"PREFIX CB", prefix_cb},           // 0xcb
    {"CALL Z,a16", call_z_a16},         // 0xcc
    {"CALL a16", call_a16},             // 0xcd
    {"ADC A,d8", adc_a_d8},             // 0xce
    {"RST 08H", rst_08h},               // 0xcf
    {"RET NC", ret_nc},                 // 0xd0
    {"POP DE", pop_de},                 // 0xd1
    {"JP NC,a16", jp_nc_a16},           // 0xd2
    {NULL, NULL}                        // 0xd3
    {"CALL NC,a16", call_nc_a16},       // 0xd4
    {"PUSH DE", push_de},               // 0xd5
    {"SUB d8", sub_d8},                 // 0xd6
    {"RST 10H", rst_10h},               // 0xd7
    {"RET C", ret_c},                   // 0xd8
    {"RETI", reti},                     // 0xd9
    {"JP C,a16", jp_c_a16},             // 0xda
    {NULL, NULL}                        // 0xdb
    {"CALL C,a16", call_c_a16},         // 0xdc
    {NULL, NULL}                        // 0xdd
    {"SBC A,d8", sbc_a_d8},             // 0xde
    {"RST 18H", rst_18h},               // 0xdf
    {"LDH (a8),A", ldh_a8_a},           // 0xe0
    {"POP HL", pop_hl},                 // 0xe1
    {"LD (C),A", ld_c_a},               // 0xe2
    {NULL, NULL}                        // 0xe3
    {NULL, NULL}                        // 0xe4
    {"PUSH HL", push_hl},               // 0xe5
    {"AND d8", and_d8},                 // 0xe6
    {"RST 20H", rst_20h},               // 0xe7
    {"ADD SP,r8", add_sp_r8},           // 0xe8
    {"JP (HL)", jp_hl},                 // 0xe9
    {"LD (a16),A", ld_a16_a},           // 0xea
    {NULL, NULL}                        // 0xeb
    {NULL, NULL}                        // 0xec
    {NULL, NULL}                        // 0xed
    {"XOR d8", xor_d8},                 // 0xee
    {"RST 28H", rst_28h},               // 0xef
    {"LDH A,(a8)", ldh_a_a8},           // 0xf0
    {"POP AF", pop_af},                 // 0xf1
    {"LD A,(C)", ld_a_c},               // 0xf2
    {"DI", di},                         // 0xf3
    {NULL, NULL}                        // 0xf4
    {"PUSH AF", push_af},               // 0xf5
    {"OR d8", or_d8},                   // 0xf6
    {"RST 30H", rst_30h},               // 0xf7
    {"LD HL,SP+r8", ld_hl_sp+r8},       // 0xf8
    {"LD SP,HL", ld_sp_hl},             // 0xf9
    {"LD A,(a16)", ld_a_a16},           // 0xfa
    {"EI", ei},                         // 0xfb
    {NULL, NULL}                        // 0xfc
    {NULL, NULL}                        // 0xfd
    {"CP d8", cp_d8},                   // 0xfe
    {"RST 38H", rst_38h},               // 0xff 
};

// NOP instruction: Do nothing
void nop(){}
