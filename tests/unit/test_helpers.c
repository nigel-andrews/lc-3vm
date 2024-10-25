#include <criterion/criterion.h>

#include "bithelpers.h"

TestSuite(test_a_bit);
TestSuite(sext);

Test(test_a_bit, zero_value)
{
    int test = 0;
    for (int i = 0; i < 10; ++i)
    {
        cr_expect(test_bit(test, i) == 0, "Expected: %d, Got: %d\n", 0,
                  test_bit(test, i));
    }
}

Test(test_a_bit, zero_n)
{
    int test = 42;
    cr_expect(test_bit(1, 0) != 0, "Expected: %d, Got: %d", 1,
              test_bit(test, 0));
}

Test(test_a_bit, pow_of_two)
{
    for (int i = 0; i < 16; ++i)
    {
        cr_expect(test_bit((1 << i), i) != 0, "Expected: non-zero, Got: %d\n",
                  test_bit((1 << i), i));
    }
}

Test(test_a_bit, misc)
{
    int test = 4;
    for (int i = 0; i < 4; ++i)
    {
        cr_expect(test_bit(test, i) == 0 || i == 2,
                  "condition == %d and i == %d\n", test_bit(test, i), i);
    }
}

#define TEST_SEXT(value)                                                       \
    do                                                                         \
    {                                                                          \
        int8_t val = (value);                                                  \
        cr_expect(val == sext(val), "Expected: %hd, Got: %hd\n", val,          \
                  sext(val));                                                  \
    } while (0)

Test(sext, zero)
{
    TEST_SEXT(0);
}

Test(sext, one)
{
    TEST_SEXT(1);
}

Test(sext, neg_one)
{
    TEST_SEXT(-1);
}

Test(sext, simple)
{
    TEST_SEXT(8);
}

Test(sext, simple_neg)
{
    TEST_SEXT(-12);
}

Test(sext, max_val)
{
    TEST_SEXT(15);
}

Test(sext, neg_max)
{
    TEST_SEXT(~0 ^ 0xE);
}
