/*
 * COZIBOY by Sofiane Djerbi (@Kugge)
 * Source code and license available on Github.
 * https://github.com/Kugge/Coziboy
 */
#pragma once
#include <stdint.h>

// Binary implementation of flags
#define FLAG_ZERO       (1 << 7) // 1000 0000 (128)
#define FLAG_NEGATIVE   (1 << 6) // 0100 0000 (64)
#define FLAG_HALFCARRY  (1 << 5) // 0010 0000 (32)
#define FLAG_CARRY      (1 << 4) // 0001 0000 (16)

#define FLAG_ISZERO         (registers.f & FLAGS_ZERO)      // Check for Z
#define FLAG_ISNEGATIVE     (registers.f & FLAGS_NEGATIVE)  // Check for N
#define FLAG_ISCARRY        (registers.f & FLAGS_CARRY)     // Check for C
#define FLAG_ISHALFCARRY    (registers.f & FLAGS_HALFCARRY) // Check for H

#define FLAG_SET(x)     (registers.f |= (x))    // Set a flag
#define FLAG_UNSET(x)   (registers.f &= ~(x))   // Unset a flag

// CPU registers implemented following:
// https://gbdev.io/pandocs/CPU_Registers_and_Flags.html
struct registers {
    uint8_t a;    // 8 Bit - 1Hi / Accumulator
    uint8_t f;    // 8 Bit - 1Lo / Flag (Z N H C)
    uint8_t b;    // 8 Bit - 2Hi
    uint8_t c;    // 8 Bit - 2Lo
    uint8_t d;    // 8 Bit - 3Hi
    uint8_t e;    // 8 Bit - 3Lo
    uint8_t h;    // 8 Bit - 4Hi
    uint8_t l;    // 8 Bit - 4Lo
    uint16_t sp;  // 16 Bit / Stack pointer
    uint16_t pc;  // 16 Bit / Program counter
};

// An abstract CPU instruction
// Might be extended : CPU cycles, parameters.
struct instruction {
    char assembly[16];      // Assembly equivalent (debugging/modding)
    uint8_t length;         // Operand length in bytes
    uint8_t cycles;         // Minimum CPU Cycles for this instruction
    void (*process)();      // Processing function (execute the opcode)
};

// CPU Vars
extern struct registers registers;                  // CPU registers
extern uint8_t cycles;                              // CPU cycles left
static const struct instruction instructions[256];  // CPU opcodes

// ASSIST/DEBUG FUNCS
void print_registers(void);
void reset(void);
void process(uint8_t []);

// OP FUNCS
void undefined(uint8_t);
void nop(void);
void ld_bc_d16(uint8_t, uint8_t);
void ld_bc_a(void);
void inc_bc(void);
void inc_b(void);
void dec_b(void);
void ld_b_d8(uint8_t);
void rlca(void);
