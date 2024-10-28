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

typedef void (*instruction_t)(uint16_t registers[], uint16_t instruction_line);

void op_add(uint16_t registers[], uint16_t instruction);
void op_and(uint16_t registers[], uint16_t instruction);
void op_br(uint16_t registers[], uint16_t instruction);

// NOTE: op_ret is included in the handling of op_jmp
void op_jmp(uint16_t registers[], uint16_t instruction);

void op_jsr(uint16_t registers[], uint16_t instruction);
void op_ld(uint16_t registers[], uint16_t instruction);
void op_ldi(uint16_t registers[], uint16_t instruction);
void op_ldr(uint16_t registers[], uint16_t instruction);
void op_lea(uint16_t registers[], uint16_t instruction);
void op_not(uint16_t registers[], uint16_t instruction);

// NOTE: Return from interrupt is not handled in this project
__attribute((noreturn)) void op_rti(uint16_t registers[], uint16_t instruction);

void op_st(uint16_t registers[], uint16_t instruction);
void op_sti(uint16_t registers[], uint16_t instruction);
void op_str(uint16_t registers[], uint16_t instruction);

#endif /* !OPCODE_H */
