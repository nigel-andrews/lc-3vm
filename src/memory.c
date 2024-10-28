#include "memory.h"

#include <err.h>
#include <string.h>

#include "error.h"

static uint16_t memory[MAX_MEM];

uint16_t read_memory(unsigned int address)
{
    if (MAX_MEM <= address)
        errx(MEMORY_VIOLATION, "Address %u falls out of bounds", address);

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
