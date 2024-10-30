#include "machine.h"

#include <err.h>
#include <stddef.h>
#include <stdint.h>

#include "error.h"
#include "io.h"
#include "memory.h"
#include "opcode.h"
#include "program.h"
#include "registers.h"
#include "syscalls.h"

#define GEN_TABLE(OPCODE) [OP_##OPCODE] = op_##OPCODE,

static instruction_t operations[OP_COUNT] = { XOPCODE(GEN_TABLE) };

#undef GEN_TABLE

static inline int get_op_code(uint16_t instruction)
{
    return instruction >> 12;
}

static inline uint16_t fetch(void)
{
    uint16_t address = register_get(RPC);
    register_incr(RPC);

    return read_memory(address);
}

void execute(struct program *program)
{
    io_setup();
    register_set(RCOND, 0);
    register_set(RPC, program->starting_address);

    static uint16_t instruction;

    program->run = 1;

    while (program->run)
    {
        // Address points to a system call
        if (register_get(RPC) < 0xFF)
        {
            call();
            continue;
        }
        else
            instruction = fetch();

        unsigned int opcode = get_op_code(instruction);

        if (opcode >= OP_COUNT)
            errx(INVALID_OPCODE, "Invalid instruction encountered, code is %d",
                 opcode);

        operations[opcode](instruction);
    }

    io_restore();
}
