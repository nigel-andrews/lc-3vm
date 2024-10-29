#ifndef PROGRAM_H
#define PROGRAM_H

#include <stddef.h>
#include <stdint.h>

struct program
{
    int run;
    uint16_t starting_address;
    size_t program_size;
};

struct program *load_program(const char *path);
void halt_program(void);

#endif /* !PROGRAM_H */
