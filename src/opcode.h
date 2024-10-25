#ifndef OPCODE_H
#define OPCODE_H

#include <stdint.h>

#include "registers.h"

enum opcode_t
{
    OP_BR = 0,
    OP_ADD,
    OP_LD,
    OP_ST,
    OP_JSR = 4,
    OP_JSRR = 4,
    OP_AND,
    OP_LDR,
    OP_STR,
    OP_RTI,
    OP_NOT,
    OP_LDI,
    OP_STI,
    OP_JMP,
    OP_RES,
    OP_LEA,
    OP_TRAP,
    OP_COUNT
};

typedef void (*instruction_t)(uint16_t registers[], enum reg_t operand_a,
                              enum reg_t operand_b);

void op_add(uint16_t registers[], enum reg_t a, enum reg_t b);

#endif /* !OPCODE_H */
