#define _POSIX_C_SOURCE 200809L
#include "io.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

static struct termios old_tio;

// FIXME: Maybe have a different thread polling and writing to memory ?
//
// credits: https://www.jmeiners.com/lc3-vm/#:input-buffering
uint16_t check_key(void)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(1, &readfds, NULL, NULL, &timeout) != 0;
}

static void sigint_handle(int signal __attribute__((unused)))
{
    io_restore();
    puts("\n");
    exit(130);
}

void io_setup(void)
{
    // Handle sigint
    struct sigaction sa;
    sa.sa_handler = sigint_handle;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Switch to non-cannonical
    tcgetattr(STDIN_FILENO, &old_tio);
    struct termios new_tio = old_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void io_restore(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}
