#include <err.h>

#include "error.h"

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
    if (argc != 2)
    {
        errx(INVALID_ARG, "Usage: src/vm <executable>\n");
    }

    return 0;
}
