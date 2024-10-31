#include "registers.h"

#include <string.h>

#include "bithelpers.h"

static uint16_t registers[REGISTER_COUNT];

void register_set(enum reg_t reg, uint16_t value)
{
    assert(reg < REGISTER_COUNT && reg >= 0);
    registers[reg] = value;
}

uint16_t register_get(enum reg_t reg)
{
    assert(reg < REGISTER_COUNT && reg >= 0);
    return registers[reg];
}

void update_condition_flags(enum reg_t modified_register)
{
    uint16_t result = registers[modified_register];

    if (test_bit(result, 15))
        register_set(RCOND, FLAG_N);
    else if (result == 0)
        register_set(RCOND, FLAG_Z);
    else
        register_set(RCOND, FLAG_P);
}

void register_incr(enum reg_t reg)
{
    assert(reg < REGISTER_COUNT && reg >= 0);
    ++registers[reg];
}

void register_decr(enum reg_t reg)
{
    assert(reg < REGISTER_COUNT && reg >= 0);
    --registers[reg];
}

void register_add(enum reg_t reg, uint16_t value)
{
    assert(reg < REGISTER_COUNT && reg >= 0);
    registers[reg] += value;
}

void clear_registers(void)
{
    memset(registers, 0, sizeof(registers));
}
