#ifndef BITHELPERS_H
#define BITHELPERS_H

#include <assert.h>
#include <stdint.h>

static inline int test_bit(uint16_t value, int n)
{
    assert(n < 16 && n >= 0);
    return (1 << n) & value;
}

#endif /* !BITHELPERS_H */
