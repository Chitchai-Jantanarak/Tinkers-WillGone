#include "renderer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Renderer {
    int rows;
    int cols;
    char* buf;
    float* zbuf;
};

Renderer* renderer_create(int rows, int cols) {
    Renderer* r = malloc(sizeof *r);
    r -> rows = rows;
    r -> cols = cols;
    r -> buf  = (char*)malloc((size_t) rows * cols);
    r -> zbuf = (float*)malloc((size_t) rows * cols * sizeof(float));
    return r;
}

void renderer_destroy(Renderer* r) {
    if (!r) return;
    free(r -> buf);
    free(r -> zbuf);
    free(r);
}

int renderer_rows(const Renderer* r) { return r -> rows; }
int renderer_cols(const Renderer* r) { return r -> cols; }

void renderer_clear(Renderer* r, char fill) {
    memset(r -> buf, fill, ((size_t) r->rows * r->cols));
    for (int i = 0; i < (r->rows * r->cols); ++i) {
        r -> zbuf[i] = 1e9f;
    }
}

void renderer_plot(Renderer* r, int x, int y, float z, char c) {
    if (
           x < 0
        || y < 0
        || x >= r -> cols
        || y >= r -> rows
    ) return;

    size_t idx = (size_t) y * r->cols + (size_t) x;
    // existed buffer reached
    if (z < r->zbuf[idx]) {
        r -> buf[idx]  = c;
        r -> zbuf[idx] = z;
    }
}

void renderer_present(Renderer* r) {
    fputs("\x1b[H", stdout);
    for (int y = 0; y < r->rows; ++y) {
        fwrite( &r -> buf[y * r->cols], 1, r -> cols, stdout);
        fputc('\n', stdout);
    }
    fflush(stdout);
}