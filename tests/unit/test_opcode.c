#include <criterion/criterion.h>

#include "opcode.h"
#include "registers.h"

static int16_t registers[REGISTER_COUNT];

void teardown(void)
{
    memset(registers, 0, sizeof(registers));
}

TestSuite(op_add);

Test(op_add, sr2, .fini = teardown)
{
    registers[R1] = 1;
    registers[R2] = 2;

    // NOTE: Should look like:
    // 0001 000 001 0 00 010
    uint16_t instruction_line = (1 << 12) | (R0 << 9) | (R1 << 6) | (R2);
    op_add(registers, instruction_line);

    cr_expect(registers[R0] == 3, "Expected: %d, Got: %d\n", 3, registers[R0]);
}

Test(op_add, imm5, .fini = teardown)
{
    registers[R1] = 1;

    // NOTE: Should look like:
    // 0001 000 001 1 00110
    uint16_t instruction_line =
        (1 << 12) | (R0 << 9) | (R1 << 6) | (1 << 5) | 6;
    op_add(registers, instruction_line);

    cr_expect(registers[R0] == 7, "Expected: %d, Got: %d\n", 7, registers[R0]);
}

Test(op_add, imm5_negative, .fini = teardown)
{
    registers[R1] = 1;

    // NOTE: Should look like:
    // 0001 000 001 1 10110
    uint16_t instruction_line =
        (1 << 12) | (R0 << 9) | (R1 << 6) | (1 << 5) | (-6 & 0x1F);
    op_add(registers, instruction_line);

    cr_expect(registers[R0] == -5, "Expected: %d, Got: %d\n", -5,
              registers[R0]);
}

TestSuite(op_and);

Test(op_and, zero)
{
    registers[R1] = 0;

    uint16_t instruction_line = (5 << 12) | (R0 << 9) | (R1 << 6) | (1 << 5);
    op_and(registers, instruction_line);

    cr_expect(registers[R0] == (0 & 0), "Expected: %d, Got: %d\n", -5,
              registers[R0]);
}

Test(op_and, same)
{
    registers[R1] = 3;

    uint16_t instruction_line = (5 << 12) | (R0 << 9) | (R1 << 6) | R1;
    op_and(registers, instruction_line);

    cr_expect(registers[R0] == (3 & 3), "Expected: %d, Got: %d\n", -5,
              registers[R0]);
}

Test(op_and, diff)
{
    registers[R1] = 3;
    registers[R2] = 5;

    uint16_t instruction_line = (5 << 12) | (R0 << 9) | (R1 << 6) | R2;
    op_and(registers, instruction_line);

    cr_expect(registers[R0] == (3 & 5), "Expected: %d, Got: %d\n", -5,
              registers[R0]);
}
