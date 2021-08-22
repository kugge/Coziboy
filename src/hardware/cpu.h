/*
 * COZIBOY by Sofiane Djerbi (@Kugge)
 * Source code and licence available on Github.
 * https://github.com/Kugge/Coziboy
 */
#pragma once

// Binary flags
#define FLAG_ZERO       (1 << 7) // 1000 0000 (128)
#define FLAG_NEGATIVE   (1 << 6) // 0100 0000 (64)
#define FLAG_HALFCARRY  (1 << 5) // 0010 0000 (32)
#define FLAG_CARRY      (1 << 4) // 0001 0000 (16)

// CPU registers implemented following:
// https://gbdev.io/pandocs/CPU_Registers_and_Flags.html
struct registers {
    unsigned char a;    // 8 Bit - 1Hi / Accumulator
    unsigned char f;    // 8 Bit - 1Lo / Flag (Z N H C)
    unsigned char b;    // 8 Bit - 2Hi
    unsigned char c;    // 8 Bit - 2Lo
    unsigned char d;    // 8 Bit - 3Hi
    unsigned char e;    // 8 Bit - 3Lo
    unsigned char h;    // 8 Bit - 4Hi
    unsigned char l;    // 8 Bit - 4Lo
    unsigned short sp;  // 16 Bit / Stack pointer
    unsigned short pc;  // 16 Bit / Program counter
} extern registers;

// An abstract CPU instruction
// Might be extended : CPU cycles, parameters.
struct instruction {
    char assembly[16];      // Assembly equivalent (debugging/modding)
    unsigned char length;   // Operand length in bytes
    unsigned char cycles;   // Minimum CPU Cycles for this instruction
    void *process;          // Processing function (execute the opcode)
} extern const instructions[256];

// ASSIST/DEBUG FUNCS
void print_registers(void);
void reset(void);

// OP FUNCS
void undefined(unsigned char);
void nop(void);

