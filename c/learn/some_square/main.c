#include "winsize_ctl.h"

int main(int argc, char const *argv[])
{
    terminal_size_t ts;
    
    if (get_terminal_size(&ts) == 0) {
        print_terminal_size(&ts);
    } else {
        fprintf(stderr, "Failed to get");
    }

    return 0;
}
