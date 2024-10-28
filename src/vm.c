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

    registers[R1] = 1;

    // NOTE: Should look like:
    // 0001 000 001 1 10110
    uint16_t instruction_line =
        (1 << 12) | (R0 << 9) | (R1 << 6) | (1 << 5) | (-6 & 0x1F);
    op_add(registers, instruction_line);

    printf("%d\n", registers[R0]);

    return 0;
}
