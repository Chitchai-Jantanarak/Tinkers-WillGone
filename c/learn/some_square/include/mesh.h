#ifndef MESH_H
#define MESH_H

#include <stddef.h>
#include "math3d.h"  // Include math3d.h to get Vec3 definition

typedef struct Mesh Mesh;

// Use Vec3 from math3d.h, don't redefine it
typedef struct { int a, b, c; }    Tri;
typedef struct { int a, b; }       Edge;

// Factory functions
Mesh* mesh_make_square(float s);
Mesh* mesh_make_cube(float s);
Mesh* mesh_make_pyramid(float s, float h);
Mesh* mesh_make_sphere(int rings, int sectors, float r);

// Accessor functions
const Vec3* mesh_vertices(const Mesh*, size_t* count);
const Tri*  mesh_triangles(const Mesh*, size_t* count);  // Fixed function name
const Edge* mesh_edges(const Mesh*, size_t* count);
const Vec3* mesh_normals(const Mesh*, size_t* count);

// Cleanup function
void mesh_destroy(Mesh* m);

#endif