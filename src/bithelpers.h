#ifndef BITHELPERS_H
#define BITHELPERS_H

#include <assert.h>
#include <stdint.h>

static inline int test_bit(uint16_t value, int n)
{
    assert(n < 16 && n >= 0);
    return (1 << n) & value;
}

static inline int16_t sext(int value)
{
    int sign = ((1u << 4) & value);
    return ((sign != 0) * ((~sign) ^ 0x1F)) | value;
}

#endif /* !BITHELPERS_H */
