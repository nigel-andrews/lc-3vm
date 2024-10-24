#ifndef REGISTERS_H
#define REGISTERS_H

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
};

#define REGISTER_COUNT 10
#define PC_START 0x3000

enum condition_flags_t
{
    FLAG_N = 1 << 0,
    FLAG_Z = 1 << 1,
    FLAG_P = 1 << 2,
};

#endif /* ! REGISTERS_H */
