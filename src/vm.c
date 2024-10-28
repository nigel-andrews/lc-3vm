#include <err.h>

#include "error.h"
#include "program.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        errx(INVALID_ARG, "Usage: src/vm <executable>\n");
    }

    struct program *program = load_program(argv[1]);

    return 0;
}
