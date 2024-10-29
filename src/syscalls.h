#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

// NOTE: 0x0000-0x00FF span is reserved for system calls
enum sysaddress_t
{
    SYS_GETC = 0x20,
    SYS_OUT,
    SYS_PUTS,
    SYS_IN,
    SYS_PUTSP,
    SYS_HALT,
    SYS_COUNT
};

typedef void (*syscall_t)(void);

void call(void);

#endif /* !SYSCALLS_H */
