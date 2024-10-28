#include <err.h>
#include <stdio.h>

#include "opcode.h"
#include "registers.h"

// #include "error.h"

static int16_t registers[REGISTER_COUNT];

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
    // if (argc != 2)
    // {
    //     errx(INVALID_ARG, "Usage: src/vm <executable>\n");
    // }

    uint16_t instruction_line = (OP_JSR << 12) | 1 << 11 | 0x3000;
    registers[RPC] = 0xF;

    op_jsr(registers, instruction_line);

    return 0;
}
