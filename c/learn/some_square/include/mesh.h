#ifndef MESH_H
#define MESH_H

#include <stddef.h>

typedef struct Mesh Mesh;

typedef struct { float x, y, z; }  Vec3;
typedef struct { int a, b, c; }    Tri;
typedef struct { int a, b; }       Edge;

// Factory func
Mesh* mesh_make_square(float s);
Mesh* mesh_make_cube(float s);
Mesh* mesh_make_pyramid(float s, float h);
Mesh* mesh_make_sphere(int rings, int sectors, float r);

const Vec3* mesh_vertices(const Mesh*, size_t* count);
const Tri*  mesh_edges(const Mesh*, size_t* count);
const Edge* mesh_tris(const Mesh*, size_t* count);
const Vec3* mesh_normals(const Mesh*, size_t* count);

#endif