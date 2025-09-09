#ifndef INPUT_H
#define INPUT_H

typedef void (*MouseListener)(int x, int y, int btn, void* user);
void input_mouse_enable(void);
void input_mouse_disable(void);
void input_set_mouse_listener(MouseListener f, void* user);
void input_poll_nonblocking(void);

#endif