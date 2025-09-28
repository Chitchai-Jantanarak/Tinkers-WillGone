#include "input.h"
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)

static int mouse_x = 0, mouse_y = 0;
static int term_width = 80, term_height = 25;

void runMouseTracker(void) {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inputRecord;
    DWORD events;
    
    // Enable mouse input
    DWORD mode;
    GetConsoleMode(hInput, &mode);
    SetConsoleMode(hInput, mode | ENABLE_MOUSE_INPUT);
    
    // Check for mouse events (non-blocking)
    if (PeekConsoleInput(hInput, &inputRecord, 1, &events) && events > 0) {
        if (ReadConsoleInput(hInput, &inputRecord, 1, &events)) {
            if (inputRecord.EventType == MOUSE_EVENT) {
                MOUSE_EVENT_RECORD mouseEvent = inputRecord.Event.MouseEvent;
                mouse_x = mouseEvent.dwMousePosition.X;
                mouse_y = mouseEvent.dwMousePosition.Y;
                
                // Clamp to terminal bounds
                if (mouse_x < 0) mouse_x = 0;
                if (mouse_y < 0) mouse_y = 0;
                if (mouse_x >= term_width) mouse_x = term_width - 1;
                if (mouse_y >= term_height) mouse_y = term_height - 1;
            }
        }
    }
}

int getMouseX(void) { return mouse_x; }
int getMouseY(void) { return mouse_y; }
void setTerminalSize(int w, int h) { term_width = w; term_height = h; }

#else
#include <sys/select.h>
#include <unistd.h>

static int mouse_x = 0, mouse_y = 0;
static int term_width = 80, term_height = 25;

void runMouseTracker(void) {
    // Enable mouse tracking with SGR extended format
    printf("\033[?1003h\033[?1006h");
    fflush(stdout);
    
    // Check if input is available (non-blocking)
    fd_set readfds;
    struct timeval timeout = {0, 0};
    
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    
    if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
        char buffer[64];
        int n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            
            // Parse SGR mouse format: \033[<b;x;yM or \033[<b;x;ym
            if (n >= 6 && buffer[0] == '\033' && buffer[1] == '[' && buffer[2] == '<') {
                int btn, x, y;
                char end_char;
                if (sscanf(buffer + 3, "%d;%d;%d%c", &btn, &x, &y, &end_char) == 4) {
                    // Convert from 1-based to 0-based coordinates
                    mouse_x = x - 1;
                    mouse_y = y - 1;
                    
                    // Clamp to terminal bounds
                    if (mouse_x < 0) mouse_x = 0;
                    if (mouse_y < 0) mouse_y = 0;
                    if (mouse_x >= term_width) mouse_x = term_width - 1;
                    if (mouse_y >= term_height) mouse_y = term_height - 1;
                }
            }
        }
    }
}

int getMouseX(void) { return mouse_x; }
int getMouseY(void) { return mouse_y; }
void setTerminalSize(int w, int h) { term_width = w; term_height = h; }

#endif

INPUT_MOUSE_POS classifyQuadrant(int x, int y, int width, int height) {
    int mid_x = width / 2;
    int mid_y = height / 2;
    
    if (y < mid_y) {
        return (x < mid_x) ? TOP_LEFT : TOP_RIGHT;
    } else {
        return (x < mid_x) ? BOT_LEFT : BOT_RIGHT;
    }
}