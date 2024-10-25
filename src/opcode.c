#include "opcode.h"

#include "bithelpers.h"
#include "registers.h"

void op_add(int16_t registers[], uint16_t instruction_line)
{
    int16_t rhs = -1;

    if (test_bit(instruction_line, 5) != 0)
    {
        rhs = sext(instruction_line & 0x1F);
    }
    else
    {
        rhs = registers[instruction_line & 0x7];
    }

    int16_t result =
        register_get(registers, (instruction_line >> 6) & 0x7) + rhs;

    register_set(registers, (instruction_line >> 9) & 0x7, result);
}
