#ifndef IO_H
#define IO_H

#include <stdint.h>

// Device register locations
enum devreg_t
{
    KBSR = 0xFE00,
    KBDR = 0xFE02,
    DSR = 0xFE04,
    DDR = 0xFE06,
    MCR = 0xFFFE,
};

void io_setup(void);
void io_restore(void);
uint16_t check_key(void);

#endif /* !IO_H */
