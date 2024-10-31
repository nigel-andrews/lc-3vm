#include "syscalls.h"

#include <err.h>
#include <stdio.h>

#include "error.h"
#include "memory.h"
#include "program.h"
#include "registers.h"

// I/O will be tested functionnally

static inline void sys_getc(void)
{
    register_set(R0, (uint16_t)getchar());
}

static inline void sys_out(void)
{
    putchar((char)register_get(R0));
    fflush(stdout);
}

static inline void sys_puts(void)
{
    uint16_t *str = get_memory_pointer(register_get(R0));

    while (*str)
        putchar((char)*(str++));

    fflush(stdout);
}

static inline void sys_in(void)
{
    printf("Enter one character > ");
    sys_getc();
    sys_out();
}

static inline void sys_putsp(void)
{
    uint16_t *str = get_memory_pointer(register_get(R0));

    while (*str)
    {
        putchar(*str & 0xFF);

        char byte = (*(str++) & 0xFF00) >> 8;
        if (byte & 0xFF00)
            putchar(byte);
    }

    fflush(stdout);
}

static inline void sys_halt(void)
{
    halt_program();
    puts("\nHALT triggered");
}

static const syscall_t syscall_table[] = {
    [SYS_GETC] = sys_getc, [SYS_OUT] = sys_out,     [SYS_PUTS] = sys_puts,
    [SYS_IN] = sys_in,     [SYS_PUTSP] = sys_putsp, [SYS_HALT] = sys_halt,
};

void call(void)
{
    // Syscalls reside in memory but for simplicity's sake it will reside
    // in the program's memory instead of the emulator's
    uint16_t program_counter = register_get(RPC);
    if (program_counter < SYS_GETC || program_counter >= SYS_COUNT)
        errx(INVALID_OPCODE,
             "Demanded syscall is out of bounds of the syscall_table");

    syscall_table[program_counter]();
    register_set(RPC, register_get(R7));
}
