#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif

typedef enum {
    TOP_LEFT,
    TOP_RIGHT,
    BOT_LEFT,
    BOT_RIGHT
} INPUT_MOUSE_POS;

void runMouseTracker(void);
INPUT_MOUSE_POS classifyQuadrant(int x, int y, int width, int height);  // Removed const
int getMouseX(void);
int getMouseY(void);
void setTerminalSize(int w, int h);

#endif