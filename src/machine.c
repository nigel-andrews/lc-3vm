#include "machine.h"

#include <err.h>
#include <stdint.h>

#include "opcode.h"
#include "registers.h"

static uint16_t registers[REGISTER_COUNT];

static inline int get_op_code(uint16_t instruction)
{
    return instruction >> 12;
}

static int fetch(void *program, int pcounter)
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

        int opcode = get_op_code(instruction);

        switch (opcode)
        {
        case OP_BR:
            break;
        case OP_ADD:
            break;
        case OP_LD:
            break;
        case OP_ST:
            break;
        case OP_JSR:
            break;
        case OP_AND:
            break;
        case OP_LDR:
            break;
        case OP_STR:
            break;
        case OP_RTI:
            break;
        case OP_NOT:
            break;
        case OP_LDI:
            break;
        case OP_STI:
            break;
        case OP_JMP:
            break;
        case OP_RES:
            break;
        case OP_LEA:
            break;
        case OP_TRAP:
            break;
        default:
            errx(1, "Invalid instruction encountered, code is %d\n", opcode);
        }

        break;
    }
}
