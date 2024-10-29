#ifndef MACHINE_H
#define MACHINE_H

#include "program.h"

struct machine *get_machine(void);
void execute(struct program *program);

#endif /* !MACHINE_H */
