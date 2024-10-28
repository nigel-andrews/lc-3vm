#ifndef MEM_H
#define MEM_H

#include <stdint.h>

// LC-3 has 16-bit addressable memory
#define MAX_MEM (1u << 16u)

uint16_t read_memory(unsigned int address);
void write_memory(unsigned int address, uint16_t value);
void reset_memory(void);

#endif /* !MEM_H*/
