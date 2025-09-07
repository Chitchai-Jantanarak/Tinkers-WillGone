#ifndef SHAPE_H
#define SHAPE_H

#include <stddef.h>

typedef struct Shape Shape;

typedef struct {
    void    (*destroy)(Shape*);
    double  (*area)(const Shape*);
}