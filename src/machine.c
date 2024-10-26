#include "machine.h"

#include <err.h>
#include <stddef.h>
#include <stdint.h>

#include "error.h"
#include "opcode.h"
#include "registers.h"

static int16_t registers[REGISTER_COUNT];

static instruction_t operations[OP_COUNT] = {
    [OP_ADD] = op_add,
    [OP_AND] = op_and,
};

static inline int get_op_code(uint16_t instruction)
{
    return instruction >> 12;
}

static inline int fetch(void *program, int pcounter)
{
    // Fetch instruction
    return 0;
}

void run(void *program)
{
    int run = 1;

    registers[RCOND] = 0;
    registers[RPC] = PC_START;

    static uint16_t instruction;

    while (run)
    {
        instruction = fetch(program, registers[RPC]++);

        unsigned int opcode = get_op_code(instruction);

        if (opcode >= OP_COUNT)
            errx(INVALID_OPCODE,
                 "Invalid instruction encountered, code is %d\n", opcode);

        operations[opcode](registers, instruction);

        break;
    }
}
