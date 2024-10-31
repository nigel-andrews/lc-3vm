#include "memory.h"

#include <err.h>
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "io.h"

static uint16_t memory[MAX_MEM];

uint16_t read_memory(unsigned int address)
{
    if (MAX_MEM <= address)
        errx(MEMORY_VIOLATION, "Address %u falls out of bounds", address);

    if (address == KBSR)
    {
        // FIXME: This should be handled in another thread probably
        if (check_key())
        {
            memory[KBSR] = 1 << 15;
            memory[KBDR] = (uint16_t)getchar();
        }
        else
            memory[KBSR] = 0;
    }

    return memory[address];
}

void write_memory(unsigned int address, uint16_t value)
{
    if (MAX_MEM <= address)
        errx(MEMORY_VIOLATION, "Address %u falls out of bounds", address);

    memory[address] = value;
}

void reset_memory(void)
{
    memset(memory, 0, sizeof(memory));
}

uint16_t *get_memory_pointer(uint16_t offset)
{
    return memory + offset;
}
