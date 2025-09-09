#ifndef WINSIZE_CTL_H
#define WINSIZE_CTL_H

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define HAS_WINSIZE 1
#else
#include <sys/ioctl.h>
#include <unistd.h>
#define HAS_WINSIZE 1
#endif


typedef struct {
    unsigned short row;
    unsigned short col;
} terminal_size_t;

extern int get_terminal_size(terminal_size_t *ts);
void print_terminal_size(const terminal_size_t *ts);


#endif