#ifndef OPCODE_H
#define OPCODE_H

#include <stdint.h>

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

#define XOPCODE(F)                                                             \
    F(BR)                                                                      \
    F(ADD)                                                                     \
    F(LD)                                                                      \
    F(ST)                                                                      \
    F(JSR)                                                                     \
    F(AND)                                                                     \
    F(LDR)                                                                     \
    F(STR)                                                                     \
    F(NOT)                                                                     \
    F(LDI)                                                                     \
    F(STI)                                                                     \
    F(JMP)                                                                     \
    F(LEA)                                                                     \
    F(TRAP)

void log_op_string_repr(enum opcode_t op);

typedef void (*instruction_t)(uint16_t instruction_line);

void op_ADD(uint16_t instruction);
void op_AND(uint16_t instruction);
void op_BR(uint16_t instruction);

// NOTE: op_RET is included in the handling of op_JMP
void op_JMP(uint16_t instruction);

void op_JSR(uint16_t instruction);
void op_LD(uint16_t instruction);
void op_LDI(uint16_t instruction);
void op_LDR(uint16_t instruction);
void op_LEA(uint16_t instruction);
void op_NOT(uint16_t instruction);

// NOTE: Return from interrupt is not handled in this project
__attribute((noreturn)) void op_rti(uint16_t instruction);

void op_ST(uint16_t instruction);
void op_STI(uint16_t instruction);
void op_STR(uint16_t instruction);
void op_TRAP(uint16_t instruction);

#endif /* !OPCODE_H */
