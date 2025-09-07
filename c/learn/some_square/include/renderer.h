#ifndef RENDERER_H
#define RENDERER_H

typedef struct Renderer Renderer;

Renderer* renderer_create(int cols, int rows);
void      renderer_destroy(Renderer* r);
void      renderer_clear(Renderer* r, char fill);
void      renderer_plot(Renderer* r, int x, int y, float z, char c);
void      renderer_present(Renderer* r);
int       renderer_rows(const Renderer* r);
int       renderer_cols(const Renderer* r);

#endif