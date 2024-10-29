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
    FLAG_P = 1 << 0,
    FLAG_Z = 1 << 1,
    FLAG_N = 1 << 2,
};

void register_set(enum reg_t reg, uint16_t value);
uint16_t register_get(enum reg_t reg);
void register_incr(enum reg_t reg);
void register_decr(enum reg_t reg);
void register_add(enum reg_t reg, uint16_t value);
void update_condition_flags(enum reg_t modified_register);
void clear_registers(void);

#endif /* ! REGISTERS_H */
