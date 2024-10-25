#include <err.h>
#include <stdio.h>

#include "error.h"
#include "machine.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        errx(INVALID_ARG, "Usage: src/vm <executable>\n");
    }

    return 0;
}
