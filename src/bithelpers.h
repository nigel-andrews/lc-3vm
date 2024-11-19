#ifndef BITHELPERS_H
#define BITHELPERS_H

#include <assert.h>
#include <stdint.h>

static inline int test_bit(uint16_t value, int n)
{
    assert(n < 16 && n >= 0);
    return (1 << n) & value;
}

static inline int16_t sext(int value, int n)
{
    assert(n < 16 && n >= 0);
    uint16_t sign = test_bit(value, n - 1);
    uint16_t mask = 0xFFFF << n;

    return ((sign != 0) * (mask)) | value;
}

#endif /* !BITHELPERS_H */
