#include "platform.h"
#include "renderer.h"
#include "winsize_ctl.h"
#include <stdio.h>

int main(void){
    platform_enter_raw();
    platform_hide_cursor();
    platform_clear_screen();

    terminal_size_t ts; get_terminal_size(&ts);
    Renderer* R = renderer_create(ts.col, ts.row);

    renderer_clear(R, '.');
    int cx = ts.col/2, cy = ts.row/2;
    renderer_plot(R, cx, cy, 0.0f, '@');
    renderer_present(R);

    platform_sleep(1500);

    renderer_clear(R, ' ');
    renderer_present(R);

    renderer_destroy(R);
    platform_show_cursor();
    platform_leave_raw();
    return 0;
}
