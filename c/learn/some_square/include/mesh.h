#pragma once

#include <stddef.h>

typedef struct Mesh Mesh;
typedef struct { float x, y, z; }  Vec3;
typedef struct { int a, b, c; }    Tri;
typedef struct { int a, b; }       Edge;

typedef struct MeshVTable {
    const Vec3* (*get_vertices)(const Mesh*, size_t* count);
    const Tri*  (*get_tris)(const Mesh*, size_t* count);
    const Edge* (*get_edges)(const Mesh*, size_t* count);
    const Vec3* (*get_normals)(const Mesh*, size_t* count);
    void (*destroy)(Mesh*);
} MeshVTable;

struct Mesh {
    const MeshVTable* vt;
};

// Factory func
Mesh* mesh_make_square(float s);
Mesh* mesh_make_cube(float s);
Mesh* mesh_make_pyramid(float s, float h);
Mesh* mesh_make_sphere(int rings, int sectors, float r);