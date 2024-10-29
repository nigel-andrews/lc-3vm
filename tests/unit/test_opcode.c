#include <criterion/criterion.h>

#include "memory.h"
#include "opcode.h"
#include "registers.h"

void teardown(void)
{
    clear_registers();
    reset_memory();
}

TestSuite(op_add);

Test(op_add, sr2, .fini = teardown)
{
    register_set(R1, 1);
    register_set(R2, 2);

    // NOTE: Should look like:
    // 0001 000 001 0 00 010
    uint16_t instruction_line = (OP_ADD << 12) | (R0 << 9) | (R1 << 6) | (R2);
    op_add(instruction_line);

    cr_expect(register_get(R0) == 3, "Expected: %d, Got: %d\n", 3,
              register_get(R0));
    cr_expect(register_get(RCOND) == FLAG_P, "Conditions flag is not positive");
}

Test(op_add, imm5, .fini = teardown)
{
    register_set(R1, 1);

    // NOTE: Should look like:
    // 0001 000 001 1 00110
    uint16_t instruction_line =
        (OP_ADD << 12) | (R0 << 9) | (R1 << 6) | (1 << 5) | 6;
    op_add(instruction_line);

    cr_expect(register_get(R0) == 7, "Expected: %d, Got: %d\n", 7,
              register_get(R0));
    cr_expect(register_get(RCOND) == FLAG_P, "Conditions flag is not positive");
}

Test(op_add, imm5_negative, .fini = teardown)
{
    register_set(R1, 1);

    // NOTE: Should look like:
    // 0001 000 001 1 10110
    uint16_t instruction_line =
        (OP_ADD << 12) | (R0 << 9) | (R1 << 6) | (1 << 5) | (-6 & 0x1F);
    op_add(instruction_line);

    cr_expect((int16_t)register_get(R0) == -5, "Expected: %d, Got: %d\n", -5,
              register_get(R0));
    cr_expect(register_get(RCOND) == FLAG_N, "Conditions flag is not negative");
}

TestSuite(op_and);

Test(op_and, zero)
{
    register_set(R1, 0);

    uint16_t instruction_line =
        (OP_AND << 12) | (R0 << 9) | (R1 << 6) | (1 << 5);
    op_and(instruction_line);

    cr_expect(register_get(R0) == 0, "Expected: %d, Got: %d\n", 0,
              register_get(R0));
    cr_expect(register_get(RCOND) == FLAG_Z, "Conditions flag is not zero");
}

Test(op_and, same)
{
    register_set(R1, 3);

    uint16_t instruction_line = (OP_AND << 12) | (R0 << 9) | (R1 << 6) | R1;
    op_and(instruction_line);

    cr_expect(register_get(R0) == (3 & 3), "Expected: %d, Got: %d\n", (3 & 3),
              register_get(R0));
    cr_expect(register_get(RCOND) == FLAG_P, "Conditions flag is not positive");
}

Test(op_and, diff)
{
    register_set(R1, 3);
    register_set(R2, 5);

    uint16_t instruction_line = (5 << 12) | (R0 << 9) | (R1 << 6) | R2;
    op_and(instruction_line);

    cr_expect(register_get(R0) == (3 & 5), "Expected: %d, Got: %d\n", (3 & 5),
              register_get(R0));
    cr_expect(register_get(RCOND) == FLAG_P, "Conditions flag is not positive");
}

TestSuite(op_br);

// NOTE: The spec stipulates that the PC is offset and incremented. The
// incrementation is handled by the machine after fetching the instruction so no
// need to test this here

Test(op_br, none)
{
    // NOTE: 0000 0 0 0 000001111
    uint16_t instruction_line = 0xF;
    int16_t old_counter = register_get(RPC);
    op_br(instruction_line);
    cr_expect(register_get(RPC) == old_counter,
              "Program branched when it should not have, old_counter was %d, "
              "current PC at %d\n",
              old_counter, register_get(RPC));
}

Test(op_br, empty_rcond)
{
    // NOTE: 0000 1 1 1 000001111
    uint16_t instruction_line = (0x7 << 9) | 0xF;
    int16_t old_counter = register_get(RPC);
    op_br(instruction_line);
    cr_expect(register_get(RPC) == old_counter,
              "Program branched when it should not have, old_counter was %d, "
              "current PC at %d\n",
              old_counter, register_get(RPC));
}

Test(op_br, branch_all)
{
    // NOTE: 0000 1 1 1 000000100
    uint16_t offset = 4;
    uint16_t instruction_line = (0x6 << 9) | offset;
    register_set(RCOND, FLAG_Z);
    int16_t old_counter = register_get(RPC);
    op_br(instruction_line);
    cr_expect(register_get(RPC) == old_counter + offset,
              "Program should have branched, expected location was %d, current "
              "PC at %d\n",
              old_counter + offset, register_get(RPC));
}

Test(op_br, branch_any)
{
    // NOTE: 0000 0 0 1 000001111
    uint16_t offset = 0xF;
    uint16_t instruction_line = (1 << 9) | offset;
    register_set(RCOND, FLAG_P);
    int16_t old_counter = register_get(RPC);
    op_br(instruction_line);
    cr_expect(register_get(RPC) == old_counter + offset,
              "Program should have branched, expected location was %d, current "
              "PC at %d\n",
              old_counter + offset, register_get(RPC));
}

Test(op_br, branch_neg)
{
    // NOTE: 0000 0 0 1 111111100
    uint16_t offset = 0x1FC;
    uint16_t instruction_line = (1 << 9) | offset;
    register_set(RCOND, FLAG_P);
    register_set(RPC, 16);
    int16_t old_counter = register_get(RPC);
    op_br(instruction_line);
    cr_expect(register_get(RPC) == old_counter - 4,
              "Program should have branched, expected location was %d, current "
              "PC at %d\n",
              old_counter - 4, register_get(RPC));
}

TestSuite(op_jmp);

Test(op_jmp, jmp_start)
{
    register_set(R2, 0x3000);
    register_set(RPC, 0x3500);

    // NOTE: 1100 000 010 000000
    uint16_t instruction_line = (OP_JMP << 12) | (R2 << 6);

    op_jmp(instruction_line);

    cr_expect(register_get(RPC) == register_get(R2),
              "Program counter is not at expected value %d, it is at %d\n",
              register_get(R2), register_get(RPC));
}

Test(op_jmp, jmp_ret)
{
    register_set(R7, 0x3000);
    register_set(RPC, 0x3500);

    // NOTE: 1100 111 000 000000
    uint16_t instruction_line = (OP_JMP << 12) | (7 << 6);

    op_jmp(instruction_line);

    cr_expect(register_get(RPC) == register_get(R7),
              "Program counter is not at expected value %d, it is at %d\n",
              register_get(R7), register_get(RPC));
}

TestSuite(op_jsr);

Test(op_jsr, jsr)
{
    register_set(RPC, 0xF);
    int sr_address = 0x3FE;
    uint16_t instruction_line = (OP_JSR << 12) | 1 << 11 | sr_address;
    op_jsr(instruction_line);

    cr_expect(
        register_get(R7) == 0xF,
        "Old program counter not saved correctly in R7, expected %d, got %d\n",
        0xF, register_get(R7));
    cr_expect(register_get(RPC) == sr_address,
              "Program counter is not at expected value %d, it is at %d\n",
              sr_address, register_get(RPC));
}

Test(op_jsr, jsrr)
{
    register_set(RPC, 0xF);
    register_set(R2, 0x3000);
    uint16_t instruction_line = (OP_JSR << 12) | (R2 << 6);
    op_jsr(instruction_line);

    cr_expect(
        register_get(R7) == 0xF,
        "Old program counter not saved correctly in R7, expected %d, got %d\n",
        0xF, register_get(R7));
    cr_expect(register_get(RPC) == 0x3000,
              "Program counter is not at expected value %d, it is at %d\n",
              register_get(R2), register_get(RPC));
}

TestSuite(op_ld);

Test(op_ld, basic)
{
    register_set(RPC, 0x1);
    // NOTE: 0010 000 000001111
    uint16_t instruction_line = OP_LD << 12 | R0 << 9 | 0xF;
    write_memory(0x10, 42);
    op_ld(instruction_line);

    cr_expect(register_get(R0) == 42,
              "Loaded memory value is wrong, expected 42, found %d\n",
              register_get(R0));
    cr_expect(register_get(RCOND) == FLAG_P,
              "Condition flags wrongly set, should be positive\n");
}

TestSuite(op_ldi);

Test(op_ldi, basic)
{
    register_set(RPC, 0x1);

    uint16_t instruction_line = OP_LDI << 12 | R0 << 9 | 0xF;
    write_memory(0x10, 0xA410);
    write_memory(0xA410, 42);

    op_ldi(instruction_line);

    cr_expect(register_get(R0) == 42,
              "Loaded memory value is wrong, expected 42, found %d\n",
              register_get(R0));
    cr_expect(register_get(RCOND) == FLAG_P,
              "Condition flags wrongly set, should be positive\n");
}

TestSuite(op_ldr);

Test(op_ldr, basic)
{
    register_set(RPC, 0x1);
    register_set(R1, 12);

    uint16_t instruction_line = OP_LDR << 12 | R0 << 9 | R1 << 6 | 4;
    write_memory(0x10, 2024);

    op_ldr(instruction_line);

    cr_expect(register_get(R0) == 2024,
              "Loaded memory value is wrong, expected 2024, found %d\n",
              register_get(R0));
    cr_expect(register_get(RCOND) == FLAG_P,
              "Condition flags wrongly set, should be positive\n");
}

TestSuite(op_lea);

Test(op_lea, basic)
{
    register_set(RPC, 1);
    uint16_t instruction_line = OP_LEA << 12 | R0 << 9 | 0xF;

    op_lea(instruction_line);

    write_memory(0x10, '!');

    cr_expect(read_memory(register_get(R0)) == '!',
              "Wrong address loaded, expected %d, got %d\n", 0x10,
              register_get(R0));

    cr_expect(register_get(RCOND) == FLAG_P,
              "Condition flags wrongly set, should be positive\n");
}

TestSuite(op_not);

Test(op_not, zero)
{
    register_set(R1, 0);
    uint16_t instruction_line = OP_NOT << 12 | R0 << 9 | R1 << 6 | 0x3F;

    op_not(instruction_line);
    cr_expect((int16_t)register_get(R0) == ~0);
    cr_expect(register_get(RCOND) == FLAG_N);
}

Test(op_not, one)
{
    register_set(R1, 1);
    uint16_t instruction_line = OP_NOT << 12 | R0 << 9 | R1 << 6 | 0x3F;

    op_not(instruction_line);
    cr_expect((int16_t)register_get(R0) == ~1);
    cr_expect(register_get(RCOND) == FLAG_N);
}

Test(op_not, simple)
{
    register_set(R1, 42);
    uint16_t instruction_line = OP_NOT << 12 | R0 << 9 | R1 << 6 | 0x3F;

    op_not(instruction_line);
    cr_expect((int16_t)register_get(R0) == ~42);
    cr_expect(register_get(RCOND) == FLAG_N);
}

Test(op_not, negative_one)
{
    register_set(R1, -1);
    uint16_t instruction_line = OP_NOT << 12 | R0 << 9 | R1 << 6 | 0x3F;

    op_not(instruction_line);
    cr_expect((int16_t)register_get(R0) == ~(-1));
    cr_expect(register_get(RCOND) == FLAG_Z);
}

TestSuite(op_st);

Test(op_st, basic)
{
    register_set(R0, 42);
    uint16_t instruction_line = OP_ST << 12 | R0 << 9 | 0xF;

    op_st(instruction_line);

    cr_expect(read_memory(0xF) == 42,
              "Stored value is incorrect, expected 42, got %d\n",
              read_memory(0xF));
}

Test(op_st, increment_pc)
{
    register_set(RPC, 5);
    register_set(R0, 42);
    uint16_t instruction_line = OP_ST << 12 | R0 << 9 | 0xF;

    op_st(instruction_line);

    cr_expect(read_memory(0x14) == 42,
              "Stored value is incorrect, expected 42, got %d\n",
              read_memory(0x14));
}

TestSuite(op_sti);

Test(op_sti, basic)
{
    register_set(R0, 2024);
    write_memory(0xF, 0xA410);
    uint16_t instruction_line = OP_STI << 12 | R0 << 9 | 0xF;
    op_sti(instruction_line);
    cr_expect(read_memory(0xA410) == 2024);
}

Test(op_sti, increment_pc)
{
    register_set(RPC, 1);
    register_set(R0, 2024);
    write_memory(0xF, 0xA410);
    uint16_t instruction_line = OP_STI << 12 | R0 << 9 | 0xE;
    op_sti(instruction_line);
    cr_expect(read_memory(0xA410) == 2024);
}

TestSuite(op_str);

Test(op_str, no_offset)
{
    register_set(R0, 42);
    register_set(R1, 0xF);
    uint16_t instruction_line = OP_STI << 12 | R0 << 9 | R1 << 6;
    op_str(instruction_line);
    cr_expect(read_memory(0xF) == 42);
}

Test(op_str, base_offset)
{
    register_set(R0, 69);
    register_set(R1, 0xE);
    uint16_t instruction_line = OP_STI << 12 | R0 << 9 | R1 << 6 | 1;
    op_str(instruction_line);
    cr_expect(read_memory(0xF) == 69);
}
