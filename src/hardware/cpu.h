/*
 * COZIBOY by Sofiane Djerbi (@Kugge)
 * Source code and licence available on Github.
 * https://github.com/Kugge/Coziboy
 */
#pragma once

// Binary flags
#define FLAG_ZERO       (1 << 7) // 0100 0000 (128)
#define FLAG_NEGATIVE   (1 << 6) // 0010 0000 (64)
#define FLAG_HALFCARRY  (1 << 5) // 0001 0000 (32)
#define FLAG_CARRY      (1 << 4) // 0000 1000 (16)

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
// Might be extended : CPU cycles, parameters.
struct instruction {
    char assembly[11]; // Assembly equivalent (debugging/modding)
    void *process;     // Processing function (execute the opcode)
} extern const instructions[256];

void nop(void);

