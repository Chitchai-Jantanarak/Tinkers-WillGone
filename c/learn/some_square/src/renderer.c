#include "renderer.h"
#include <stdlib.h>
#include <stdio.h>

struct Renderer {
    int cols;
    int rows;
    char* buffer;
};

Renderer* renderer_create(int rows, int cols) {
    Renderer* r = malloc(sizeof *r);
    r -> rows = rows;
    r -> cols = cols;
    return r;
}