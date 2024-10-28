#include "opcode.h"

#include "bithelpers.h"
#include "registers.h"

#define GET_SR1(instruction_line) ((instruction_line >> 6) & 0x7)
#define GET_SR2(instruction_line) (instruction_line & 0x7)
#define GET_IMM5(instruction_line) (instruction_line & 0x1F)
#define GET_DR(instruction_line) (instruction_line >> 9) & 0x7

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

void op_add(int16_t registers[], uint16_t instruction_line)
{
    int16_t rhs = -1;

    if (test_bit(instruction_line, 5) != 0)
        rhs = sext(GET_IMM5(instruction_line), 5);
    else
        rhs = register_get(registers, GET_SR2(instruction_line));

    int16_t result = register_get(registers, GET_SR1(instruction_line)) + rhs;

    enum reg_t dr = GET_DR(instruction_line);

    register_set(registers, dr, result);
    update_condition_flags(registers, dr);
}

void op_and(int16_t registers[], uint16_t instruction_line)
{
    int16_t rhs = -1;

    if (test_bit(instruction_line, 5) != 0)
        rhs = sext(GET_IMM5(instruction_line), 5);
    else
        rhs = register_get(registers, GET_SR2(instruction_line));

    int16_t result = register_get(registers, GET_SR1(instruction_line)) & rhs;

    enum reg_t dr = GET_DR(instruction_line);

    register_set(registers, dr, result);
    update_condition_flags(registers, dr);
}

void op_br(int16_t registers[], uint16_t instruction_line)
{
    int conditions = (instruction_line & 0x700) >> 9;

    if (conditions & registers[RCOND])
    {
        registers[RPC] += sext(instruction_line & 0x1FF, 9);
    }
}
