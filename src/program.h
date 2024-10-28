#ifndef PROGRAM_H
#define PROGRAM_H

#include <stddef.h>
#include <stdint.h>

struct program
{
    uint16_t starting_address;
    size_t program_size;
    uint16_t *memory;
};

struct program *load_program(const char *path);

#endif /* !PROGRAM_H */
