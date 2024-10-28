#include "opcode.h"

#include "bithelpers.h"
#include "registers.h"

#define GET_SR1(instruction) ((instruction >> 6) & 0x7)
#define GET_SR2(instruction) (instruction & 0x7)
#define GET_IMM5(instruction) (instruction & 0x1F)
#define GET_DR(instruction) (instruction >> 9) & 0x7
#define GET_PCOFFSET9(instruction) (instruction & 0x1FF)
#define GET_PCOFFSET11(instruction) (instruction & 0x7FF)

static void update_condition_flags(int16_t registers[],
                                   enum reg_t modified_register)
{
    int16_t result = registers[modified_register];

    if (result < 0)
        register_set(registers, RCOND, FLAG_N);
    else if (result > 0)
        register_set(registers, RCOND, FLAG_P);
    else
        register_set(registers, RCOND, FLAG_Z);
}

void op_add(int16_t registers[], uint16_t instruction)
{
    int16_t rhs = -1;

    if (test_bit(instruction, 5) != 0)
        rhs = sext(GET_IMM5(instruction), 5);
    else
        rhs = register_get(registers, GET_SR2(instruction));

    int16_t result = register_get(registers, GET_SR1(instruction)) + rhs;

    enum reg_t dr = GET_DR(instruction);

    register_set(registers, dr, result);
    update_condition_flags(registers, dr);
}

void op_and(int16_t registers[], uint16_t instruction)
{
    int16_t rhs = -1;

    if (test_bit(instruction, 5) != 0)
        rhs = sext(GET_IMM5(instruction), 5);
    else
        rhs = register_get(registers, GET_SR2(instruction));

    int16_t result = register_get(registers, GET_SR1(instruction)) & rhs;

    enum reg_t dr = GET_DR(instruction);

    register_set(registers, dr, result);
    update_condition_flags(registers, dr);
}

void op_br(int16_t registers[], uint16_t instruction)
{
    int conditions = (instruction & 0x700) >> 9;

    if (conditions & registers[RCOND])
    {
        registers[RPC] += sext(GET_PCOFFSET9(instruction), 9);
    }
}

void op_jmp(int16_t registers[], uint16_t instruction)
{
    int br = (instruction & 0x1C0) >> 6;
    registers[RPC] = registers[R7 < br ? R7 : br];
}

void op_jsr(int16_t registers[], uint16_t instruction)
{
    registers[R7] = registers[RPC];

    int16_t address = -1;
    if (test_bit(instruction, 11))
        address = sext(GET_PCOFFSET11(instruction), 11);
    else
        address = registers[GET_SR1(instruction)];

    registers[RPC] = address;
}
