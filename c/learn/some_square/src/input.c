#include "input.h"

#if defined(_WIN32) || defined(_WIN64)

void runMouseTracker(void) {}; // Not implemented

#else

void runMouseTracker(void) {
    struct termios oldt, newt;
    struct winsize ws;

    tcgetattr(STDIN_FILENO, &oldt);
    // Enabling mouse trackable
    printf("\033[?1003h\033[?1006h]");
    fflush(stdout);
}

#endif