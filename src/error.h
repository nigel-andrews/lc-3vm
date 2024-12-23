#ifndef ERROR_H
#define ERROR_H

#include <err.h>

enum err_t
{
    INVALID_ARG = 1,
    INVALID_OPCODE = 2,
    MEMORY_VIOLATION = 3,
};

#endif /* !ERROR_H */
