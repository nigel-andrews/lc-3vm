#include "opcode.h"

#include <stdio.h>

#include "bithelpers.h"
#include "registers.h"

void op_add(uint16_t registers[], uint16_t instruction_line)
{
    enum reg_t sr2 = -1;

    if (test_bit(instruction_line, 5) != 0)
    {
        // TODO: handle imm5
    }
    else
    {
        sr2 = instruction_line & 0x7;
    }

    printf("sr2 == %d\n", sr2);

    enum reg_t sr1 = (instruction_line >> 6) & 0x7;
    enum reg_t dr = (instruction_line >> 9) & 0x7;

    uint16_t result =
        register_get(registers, sr1) + register_get(registers, sr2);

    register_set(registers, dr, result);
}
