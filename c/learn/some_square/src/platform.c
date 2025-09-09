#include "platform.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

void platform_enter_raw(void) {
    // !not_implemented
}

void platform_leave_raw(void) {
    // !not_implemented
}

void platform_sleep_ms(int ms) {
    Sleep(ms);
}

unsigned long long platform_now_ms(void) {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (counter.QuadPart * 1000ULL) / freq.QuadPart;
}

#else
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

static struct termios oldt;
static int raw_active = 0;

void platform_enter_raw(void) {
    if (raw_active) return;
    struct termios t;

    tcgetattr(STDIN_FILENO, &oldt);
    t = oldt;
    t.c_lflag     &= ~( ICANON | ECHO );
    t.c_cc[VMIN]  = 0
    t.c_cc[VTIME] = 0
    tcgetattr(STDIN_FILENO, TCSANOW, &t);
    raw_active = 1;
}

void platform_leave_raw(void) {
    if (!raw_active) return;

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    raw_active = 0;
}

void platform_hide_cursor(void)  { fputs("\x1b[?25l", stdout); fflush(stdout); }
void platform_show_cursor(void)  { fputs("\x1b[?25h", stdout); fflush(stdout); }
void platform_clear_screen(void) { fputs("\x1b[2J\x1b[H", stdout); fflush(stdout); }


void platform_sleep_ms(int ms) {
    struct timespec ts = {
        ms / 1000,                  // Second
        (ms % 1000) * 1000000L      // Nanosecond
    };
    nanosleep(&ts, NULL);
}

unsigned long long platform_now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long long) ts.tv_sec*1000ULL + ts.tv_nsec / 1000000ULL;
}

#endif