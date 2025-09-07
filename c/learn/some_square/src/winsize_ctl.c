#include "winsize_ctl.h"

#if defined(_WIN32) || defined(_WIN64)

int get_terminal_size(terminal_size_t *ts) {
    if (!ts) return -1;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE) return -1;

    if (!GetConsoleScreenBufferInfo(hStdout, &csbi)) return -1;
    
    // refers to _SMALL_RECT coords
    ts -> row = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    ts -> col = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return 0;
}

#else

int get_terminal_size(terminal_size_t *ts) {
    if (!ts) return -1;

    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl TIOCGWINSZ failed");
        return -1;
    } 

    ts -> row = ws.ws_row;
    ts -> col = ws.ws_col;
}

#endif


void print_terminal_size(const terminal_size_t *ts) {
    if (!ts) return;
    printf("SIZE %d : %d\n", ts->row, ts->col);
}