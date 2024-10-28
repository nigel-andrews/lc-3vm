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
    uint16_t sign = ((1u << (n - 1)) & value);
    // 10110 -> 1111 1111 1111 0110
    // 00110 -> 0000 0000 0000 0110

    // 0000 0000 0001 1111 ~= 1111 1111 1110 0000
    uint16_t mask = ~0x1F;

    return (sign != 0) * (mask) | value;
}

#endif /* !BITHELPERS_H */
