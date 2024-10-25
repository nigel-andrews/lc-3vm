#include <criterion/criterion.h>

#include "opcode.h"
#include "registers.h"

static uint16_t registers[REGISTER_COUNT];

void teardown(void)
{
    memset(registers, 0, sizeof(registers));
}

TestSuite(opcodes);

Test(opcodes, op_add_sr2, .fini = teardown)
{
    registers[R1] = 1;
    registers[R2] = 2;

    // NOTE: Should look like:
    // 0001 000 001 0 00 010
    uint16_t instruction_line = (1 << 12) | (R0 << 9) | (R1 << 6) | (R2);
    op_add(registers, instruction_line);

    cr_expect(registers[R0] == 3, "Expected: %d, Got: %d\n", 3, registers[R0]);
}

Test(opcodes, op_add_imm5, .fini = teardown)
{
    // uint16_t instruction_line = (1 << 12) | (R0 << 9) | (R1 << 6) | (1 << 5)
    // | (R2);
    cr_expect(false, "Not implemented yet");
}