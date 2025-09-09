#ifndef SHAPE_H
#define SHAPE_H

#include <stddef.h>

typedef struct Shape Shape;

struct Shape {
    void    (*draw)(Shape*, char** buffer, int rows, int cols);
    void    (*rotate)(Shape*, float angle_x, float angle_y);
    void    (*destroy)(Shape*);
    double  (*area)(const Shape*);
};

Shape* shape_create_square(float size);


#endif