#include "machine.h"

#include <err.h>
#include <stddef.h>
#include <stdint.h>

#include "error.h"
#include "memory.h"
#include "opcode.h"
#include "program.h"
#include "registers.h"

static instruction_t operations[OP_COUNT] = {
    [OP_ADD] = op_add,
    [OP_AND] = op_and,
    [OP_BR] = op_br,
};

static inline int get_op_code(uint16_t instruction)
{
    return instruction >> 12;
}

static inline uint16_t fetch(struct program *program)
{
    uint16_t address = register_get(RPC);

    if (program->program_size <= address)
    {
        errx(MEMORY_VIOLATION, "Program counter is out of bounds\n");
    }

    register_incr(RPC);

    return read_memory(address);
}

void execute(struct program *program)
{
    register_set(RCOND, 0);
    register_set(RPC, program->starting_address);

    static uint16_t instruction;

    program->run = 1;

    while (program->run)
    {
        // Address points to a system call
        // if (register_get(RPC) < 0xFF)
        // {
        //     call();
        //     continue;
        // }
        // else
        instruction = fetch(program);

        unsigned int opcode = get_op_code(instruction);

        if (opcode >= OP_COUNT)
            errx(INVALID_OPCODE,
                 "Invalid instruction encountered, code is %d\n", opcode);

        operations[opcode](instruction);

        break;
    }
}
