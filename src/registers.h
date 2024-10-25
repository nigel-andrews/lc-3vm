#ifndef REGISTERS_H
#define REGISTERS_H

#include <assert.h>
#include <stdint.h>

enum reg_t
{
    // General purpose registers
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,

    // Program counter
    RPC,

    // Condition flags
    RCOND,
    REGISTER_COUNT
};

#define PC_START 0x3000

enum condition_flags_t
{
    FLAG_N = 1 << 0,
    FLAG_Z = 1 << 1,
    FLAG_P = 1 << 2,
};

static inline void register_set(uint16_t registers[], enum reg_t reg,
                                uint16_t value)
{
    assert(reg < REGISTER_COUNT && reg >= 0);
    registers[reg] = value;
}

static inline void register_get(uint16_t registers[], enum reg_t reg,
                                uint16_t value)
{
    assert(reg < REGISTER_COUNT && reg >= 0);
    registers[reg] = value;
}

#endif /* ! REGISTERS_H */
