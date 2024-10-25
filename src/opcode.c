#include "opcode.h"

#include "bithelpers.h"
#include "registers.h"

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
    {
        rhs = sext(instruction_line & 0x1F);
    }
    else
    {
        rhs = register_get(registers, instruction_line & 0x7);
    }

    int16_t result =
        register_get(registers, (instruction_line >> 6) & 0x7) + rhs;

    enum reg_t dr = (instruction_line >> 9) & 0x7;
    register_set(registers, dr, result);
    update_condition_flags(registers, dr);
}
