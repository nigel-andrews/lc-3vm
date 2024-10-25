#ifndef BITHELPERS_H
#define BITHELPERS_H

#include <stdbool.h>

static inline int test_a_bit(int value, int n)
{
    return (1 << n) & value;
}

#endif /* !BITHELPERS_H */
