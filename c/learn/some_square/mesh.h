#ifdef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>

typedef struct { float x, y, z; } Vec3;
static inline Vec3 v3(float x, float y, float z) { return ( Vec3 ){ x, y, z }; }

typedef struct {
    Vec3 verts[4];
    int e
}