#include "program.h"

#include <stdio.h>

#include "error.h"
#include "memory.h"

static inline uint16_t swap_bytes(uint16_t word)
{
    return (word << 8) | (word >> 8);
}

static struct program program = { 0 };

struct program *load_program(const char *path)
{
    FILE *file = fopen(path, "r");

    if (!file)
        errx(INVALID_ARG, "Can't open file %s", path);

    fread(&program.starting_address, sizeof(uint16_t), 1, file);

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    program.starting_address = swap_bytes(program.starting_address);
#endif

    uint16_t *program_start = get_memory_pointer(program.starting_address);

    size_t address_space = MAX_MEM - program.starting_address;

    program.program_size =
        fread(program_start, sizeof(uint16_t), address_space, file);

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

    for (size_t i = 0; i < program.program_size; ++i)
        program_start[i] = swap_bytes(program_start[i]);

#endif

    fclose(file);

    return &program;
}

void halt_program(void)
{
    program.run = 0;
}
