#ifndef PLATFORM_H
#define PLATFORM_H

void platform_enter_raw(void);
void platform_leave_raw(void);
void platform_hide_cursor(void);
void platform_show_cursor(void);
void platform_clear_screen(void);

void platform_sleep(int ms);
unsigned long long platform_now(void);

#endif