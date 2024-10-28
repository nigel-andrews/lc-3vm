#include <criterion/criterion.h>

#include "memory.h"
#include "opcode.h"
#include "registers.h"

static uint16_t registers[REGISTER_COUNT];

void teardown(void)
{
    memset(registers, 0, sizeof(registers));
    reset_memory();
}

TestSuite(op_add);

Test(op_add, sr2, .fini = teardown)
{
    registers[R1] = 1;
    registers[R2] = 2;

    // NOTE: Should look like:
    // 0001 000 001 0 00 010
    uint16_t instruction_line = (OP_ADD << 12) | (R0 << 9) | (R1 << 6) | (R2);
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
        (OP_ADD << 12) | (R0 << 9) | (R1 << 6) | (1 << 5) | 6;
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
        (OP_ADD << 12) | (R0 << 9) | (R1 << 6) | (1 << 5) | (-6 & 0x1F);
    op_add(registers, instruction_line);

    cr_expect((int16_t)registers[R0] == -5, "Expected: %d, Got: %d\n", -5,
              registers[R0]);
    cr_expect(registers[RCOND] == FLAG_N, "Conditions flag is not negative");
}

TestSuite(op_and);

Test(op_and, zero)
{
    registers[R1] = 0;

    uint16_t instruction_line =
        (OP_AND << 12) | (R0 << 9) | (R1 << 6) | (1 << 5);
    op_and(registers, instruction_line);

    cr_expect(registers[R0] == 0, "Expected: %d, Got: %d\n", 0, registers[R0]);
    cr_expect(registers[RCOND] == FLAG_Z, "Conditions flag is not zero");
}

Test(op_and, same)
{
    registers[R1] = 3;

    uint16_t instruction_line = (OP_AND << 12) | (R0 << 9) | (R1 << 6) | R1;
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
    uint16_t instruction_line = (OP_JMP << 12) | (R2 << 6);

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
    uint16_t instruction_line = (OP_JMP << 12) | (7 << 6);

    op_jmp(registers, instruction_line);

    cr_expect(registers[RPC] == registers[R7],
              "Program counter is not at expected value %d, it is at %d\n",
              registers[R7], registers[RPC]);
}

TestSuite(op_jsr);

Test(op_jsr, jsr)
{
    registers[RPC] = 0xF;
    int sr_address = 0x3FE;
    uint16_t instruction_line = (OP_JSR << 12) | 1 << 11 | sr_address;
    op_jsr(registers, instruction_line);

    cr_expect(
        registers[R7] == 0xF,
        "Old program counter not saved correctly in R7, expected %d, got %d\n",
        0xF, registers[R7]);
    cr_expect(registers[RPC] == sr_address,
              "Program counter is not at expected value %d, it is at %d\n",
              sr_address, registers[RPC]);
}

Test(op_jsr, jsrr)
{
    registers[RPC] = 0xF;
    registers[R2] = 0x3000;
    uint16_t instruction_line = (OP_JSR << 12) | (R2 << 6);
    op_jsr(registers, instruction_line);

    cr_expect(
        registers[R7] == 0xF,
        "Old program counter not saved correctly in R7, expected %d, got %d\n",
        0xF, registers[R7]);
    cr_expect(registers[RPC] == 0x3000,
              "Program counter is not at expected value %d, it is at %d\n",
              registers[R2], registers[RPC]);
}

TestSuite(op_ld);

Test(op_ld, basic)
{
    registers[RPC] = 0x1;
    // NOTE: 0010 000 000001111
    uint16_t instruction_line = OP_LD << 12 | R0 << 9 | 0xF;
    write_memory(0x10, 42);
    op_ld(registers, instruction_line);

    cr_expect(registers[R0] == 42,
              "Loaded memory value is wrong, expected 42, found %d\n",
              registers[R0]);
    cr_expect(registers[RCOND] == FLAG_P,
              "Condition flags wrongly set, should be positive\n");
}

TestSuite(op_ldi);

Test(op_ldi, basic)
{
    registers[RPC] = 0x1;

    uint16_t instruction_line = OP_LDI << 12 | R0 << 9 | 0xF;
    write_memory(0x10, 0xA410);
    write_memory(0xA410, 42);

    op_ldi(registers, instruction_line);

    cr_expect(registers[R0] == 42,
              "Loaded memory value is wrong, expected 42, found %d\n",
              registers[R0]);
    cr_expect(registers[RCOND] == FLAG_P,
              "Condition flags wrongly set, should be positive\n");
}

TestSuite(op_ldr);

Test(op_ldr, basic)
{
    registers[RPC] = 0x1;
    registers[R1] = 12;

    uint16_t instruction_line = OP_LDR << 12 | R0 << 9 | R1 << 6 | 4;
    write_memory(0x10, 2024);

    op_ldr(registers, instruction_line);

    cr_expect(registers[R0] == 2024,
              "Loaded memory value is wrong, expected 2024, found %d\n",
              registers[R0]);
    cr_expect(registers[RCOND] == FLAG_P,
              "Condition flags wrongly set, should be positive\n");
}

TestSuite(op_lea);

Test(op_lea, basic)
{
    registers[RPC] = 1;
    uint16_t instruction_line = OP_LEA << 12 | R0 << 9 | 0xF;

    op_lea(registers, instruction_line);

    write_memory(0x10, '!');

    cr_expect(read_memory(registers[R0]) == '!',
              "Wrong address loaded, expected %d, got %d\n", 0x10,
              registers[R0]);

    cr_expect(registers[RCOND] == FLAG_P,
              "Condition flags wrongly set, should be positive\n");
}

TestSuite(op_not);

Test(op_not, zero)
{
    registers[R1] = 0;
    uint16_t instruction_line = OP_NOT << 12 | R0 << 9 | R1 << 6 | 0x3F;

    op_not(registers, instruction_line);
    cr_expect((int16_t)registers[R0] == ~0);
    cr_expect(registers[RCOND] == FLAG_N);
}

Test(op_not, one)
{
    registers[R1] = 1;
    uint16_t instruction_line = OP_NOT << 12 | R0 << 9 | R1 << 6 | 0x3F;

    op_not(registers, instruction_line);
    cr_expect((int16_t)registers[R0] == ~1);
    cr_expect(registers[RCOND] == FLAG_N);
}

Test(op_not, simple)
{
    registers[R1] = 42;
    uint16_t instruction_line = OP_NOT << 12 | R0 << 9 | R1 << 6 | 0x3F;

    op_not(registers, instruction_line);
    cr_expect((int16_t)registers[R0] == ~42);
    cr_expect(registers[RCOND] == FLAG_N);
}

Test(op_not, negative_one)
{
    registers[R1] = -1;
    uint16_t instruction_line = OP_NOT << 12 | R0 << 9 | R1 << 6 | 0x3F;

    op_not(registers, instruction_line);
    cr_expect((int16_t)registers[R0] == ~(-1));
    cr_expect(registers[RCOND] == FLAG_Z);
}
