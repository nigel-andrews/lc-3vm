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
    cr_expect(registers[RCOND] == FLAG_P, "Conditions flag is not positive");
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
    cr_expect(registers[RCOND] == FLAG_P, "Conditions flag is not positive");
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
    cr_expect(registers[RCOND] == FLAG_N, "Conditions flag is not negative");
}

TestSuite(op_and);

Test(op_and, zero)
{
    registers[R1] = 0;

    uint16_t instruction_line = (5 << 12) | (R0 << 9) | (R1 << 6) | (1 << 5);
    op_and(registers, instruction_line);

    cr_expect(registers[R0] == 0, "Expected: %d, Got: %d\n", 0, registers[R0]);
    cr_expect(registers[RCOND] == FLAG_Z, "Conditions flag is not zero");
}

Test(op_and, same)
{
    registers[R1] = 3;

    uint16_t instruction_line = (5 << 12) | (R0 << 9) | (R1 << 6) | R1;
    op_and(registers, instruction_line);

    cr_expect(registers[R0] == (3 & 3), "Expected: %d, Got: %d\n", (3 & 3),
              registers[R0]);
    cr_expect(registers[RCOND] == FLAG_P, "Conditions flag is not positive");
}

Test(op_and, diff)
{
    registers[R1] = 3;
    registers[R2] = 5;

    uint16_t instruction_line = (5 << 12) | (R0 << 9) | (R1 << 6) | R2;
    op_and(registers, instruction_line);

    cr_expect(registers[R0] == (3 & 5), "Expected: %d, Got: %d\n", (3 & 5),
              registers[R0]);
    cr_expect(registers[RCOND] == FLAG_P, "Conditions flag is not positive");
}

TestSuite(op_br);

// NOTE: The spec stipulates that the PC is offset and incremented. The
// incrementation is handled by the machine after fetching the instruction so no
// need to test this here

Test(op_br, none)
{
    // NOTE: 0000 0 0 0 000001111
    uint16_t instruction_line = 0xF;
    int16_t old_counter = registers[RPC];
    op_br(registers, instruction_line);
    cr_expect(registers[RPC] == old_counter,
              "Program branched when it should not have, old_counter was %d, "
              "current PC at %d\n",
              old_counter, registers[RPC]);
}

Test(op_br, empty_rcond)
{
    // NOTE: 0000 1 1 1 000001111
    uint16_t instruction_line = (0x7 << 9) | 0xF;
    int16_t old_counter = registers[RPC];
    op_br(registers, instruction_line);
    cr_expect(registers[RPC] == old_counter,
              "Program branched when it should not have, old_counter was %d, "
              "current PC at %d\n",
              old_counter, registers[RPC]);
}

Test(op_br, branch_all)
{
    // NOTE: 0000 1 1 1 000000100
    uint16_t offset = 4;
    uint16_t instruction_line = (0x6 << 9) | offset;
    registers[RCOND] = FLAG_Z;
    int16_t old_counter = registers[RPC];
    op_br(registers, instruction_line);
    cr_expect(registers[RPC] == old_counter + offset,
              "Program should have branched, expected location was %d, current "
              "PC at %d\n",
              old_counter + offset, registers[RPC]);
}

Test(op_br, branch_any)
{
    // NOTE: 0000 0 0 1 000001111
    uint16_t offset = 0xF;
    uint16_t instruction_line = (1 << 9) | offset;
    registers[RCOND] = FLAG_P;
    int16_t old_counter = registers[RPC];
    op_br(registers, instruction_line);
    cr_expect(registers[RPC] == old_counter + offset,
              "Program should have branched, expected location was %d, current "
              "PC at %d\n",
              old_counter + offset, registers[RPC]);
}

Test(op_br, branch_neg)
{
    // NOTE: 0000 0 0 1 111111100
    uint16_t offset = 0x1FC;
    uint16_t instruction_line = (1 << 9) | offset;
    registers[RCOND] = FLAG_P;
    registers[RPC] = 16;
    int16_t old_counter = registers[RPC];
    op_br(registers, instruction_line);
    cr_expect(registers[RPC] == old_counter - 4,
              "Program should have branched, expected location was %d, current "
              "PC at %d\n",
              old_counter - 4, registers[RPC]);
}

TestSuite(op_jmp);

Test(op_jmp, jmp_start)
{
    registers[R2] = 0x3000;
    registers[RPC] = 0x3500;

    // NOTE: 1100 000 010 000000
    uint16_t instruction_line = (0xC << 12) | (R2 << 6);

    op_jmp(registers, instruction_line);

    cr_expect(registers[RPC] == registers[R2],
              "Program counter is not at expected value %d, it is at %d\n",
              registers[R2], registers[RPC]);
}

Test(op_jmp, jmp_ret)
{
    registers[R7] = 0x3000;
    registers[RPC] = 0x3500;

    // NOTE: 1100 111 000 000000
    uint16_t instruction_line = (0xC << 12) | (7 << 6);

    op_jmp(registers, instruction_line);

    cr_expect(registers[RPC] == registers[R7],
              "Program counter is not at expected value %d, it is at %d\n",
              registers[R7], registers[RPC]);
}
