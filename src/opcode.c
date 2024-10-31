#include "opcode.h"

#include <stdio.h>

#include "bithelpers.h"
#include "error.h"
#include "memory.h"
#include "registers.h"

#define GET_SR1(instruction) ((instruction >> 6) & 0x7)
#define GET_SR2(instruction) (instruction & 0x7)
#define GET_IMM5(instruction) (instruction & 0x1F)
#define GET_DR(instruction) (instruction >> 9) & 0x7
#define GET_PCOFFSET9(instruction) (instruction & 0x1FF)
#define GET_PCOFFSET11(instruction) (instruction & 0x7FF)

#define STRING_REPR(OPCODE) [OP_##OPCODE] = "OP_" #OPCODE,

static const char *string_repr[] = { XOPCODE(STRING_REPR) };

#undef STRING_REPR

void log_op_string_repr(enum opcode_t op)
{
    fprintf(stderr, "%s\n", string_repr[op]);
}

void op_ADD(uint16_t instruction)
{
    int16_t rhs = -1;

    if (test_bit(instruction, 5) != 0)
        rhs = sext(GET_IMM5(instruction), 5);
    else
        rhs = register_get(GET_SR2(instruction));

    int16_t result = register_get(GET_SR1(instruction)) + rhs;

    enum reg_t dr = GET_DR(instruction);

    register_set(dr, result);
    update_condition_flags(dr);
}

void op_AND(uint16_t instruction)
{
    int16_t rhs = -1;

    if (test_bit(instruction, 5) != 0)
        rhs = sext(GET_IMM5(instruction), 5);
    else
        rhs = register_get(GET_SR2(instruction));

    int16_t result = register_get(GET_SR1(instruction)) & rhs;

    enum reg_t dr = GET_DR(instruction);

    register_set(dr, result);
    update_condition_flags(dr);
}

void op_BR(uint16_t instruction)
{
    int conditions = (instruction & 0x700) >> 9;

    if (conditions & register_get(RCOND))
        register_add(RPC, sext(GET_PCOFFSET9(instruction), 9));
}

void op_JMP(uint16_t instruction)
{
    int br = (instruction & 0x1C0) >> 6;
    register_set(RPC, register_get(R7 < br ? R7 : br));
}

void op_JSR(uint16_t instruction)
{
    register_set(R7, register_get(RPC));

    int16_t address = -1;
    if (test_bit(instruction, 11))
        address = register_get(RPC) + sext(GET_PCOFFSET11(instruction), 11);
    else
        address = register_get(GET_SR1(instruction));

    register_set(RPC, address);
}

void op_LD(uint16_t instruction)
{
    int dr = GET_DR(instruction);
    int16_t address = register_get(RPC) + sext(GET_PCOFFSET9(instruction), 9);
    register_set(dr, read_memory(address));
    update_condition_flags(dr);
}

void op_LDI(uint16_t instruction)
{
    int dr = GET_DR(instruction);
    int16_t address = register_get(RPC) + sext(GET_PCOFFSET9(instruction), 9);
    register_set(dr, read_memory(read_memory(address)));
    update_condition_flags(dr);
}

void op_LDR(uint16_t instruction)
{
    int dr = GET_DR(instruction);
    int br = GET_SR1(instruction);
    int16_t offset = sext(instruction & 0x3F, 6);

    register_set(dr, read_memory(register_get(br) + offset));
    update_condition_flags(dr);
}

void op_LEA(uint16_t instruction)
{
    int dr = GET_DR(instruction);
    register_set(dr, register_get(RPC) + sext(GET_PCOFFSET9(instruction), 9));
    update_condition_flags(dr);
}

void op_NOT(uint16_t instruction)
{
    assert(((instruction & 0x3F) ^ 0x3F) == 0);
    int dr = GET_DR(instruction);
    int sr = GET_SR1(instruction);

    register_set(dr, ~register_get(sr));
    update_condition_flags(dr);
}

__attribute((noreturn)) void op_RTI(uint16_t instruction __attribute((unused)))
{
    errx(INVALID_OPCODE,
         "Return from interrupt is not supported in this project");
}

void op_ST(uint16_t instruction)
{
    write_memory(register_get(RPC) + sext(GET_PCOFFSET9(instruction), 9),
                 register_get(GET_DR(instruction)));
}

void op_STI(uint16_t instruction)
{
    write_memory(
        read_memory(register_get(RPC) + sext(GET_PCOFFSET9(instruction), 9)),
        register_get(GET_DR(instruction)));
}

void op_STR(uint16_t instruction)
{
    write_memory(register_get(GET_SR1(instruction))
                     + sext(instruction & 0x3F, 6),
                 register_get(GET_DR(instruction)));
}

void op_TRAP(uint16_t instruction)
{
    assert((instruction & (0xF << 8)) == 0);

    uint8_t trapvect8 = instruction & 0xFF;
    register_set(R7, register_get(RPC));
    register_set(RPC, (int16_t)trapvect8);
}
