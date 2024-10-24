#ifndef OPCODE_H
#define OPCODE_H

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
};

#endif /* !OPCODE_H */
