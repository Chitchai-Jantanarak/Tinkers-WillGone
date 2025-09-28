#include "mesh.h"
#include <stdlib.h>
#include <math.h>

struct Mesh {
    Vec3* vertices;
    size_t vertex_count;
    Tri* triangles;
    size_t triangle_count;
    Vec3* normals;
    size_t normal_count;
};

Mesh* mesh_make_cube(float s) {
    Mesh* m = malloc(sizeof(Mesh));
    if (!m) return NULL;
    
    // 8 vertices for a cube
    m->vertex_count = 8;
    m->vertices = malloc(sizeof(Vec3) * 8);
    
    float h = s / 2.0f;
    m->vertices[0] = (Vec3){-h, -h, -h}; // 0: left-bottom-back
    m->vertices[1] = (Vec3){ h, -h, -h}; // 1: right-bottom-back
    m->vertices[2] = (Vec3){ h,  h, -h}; // 2: right-top-back
    m->vertices[3] = (Vec3){-h,  h, -h}; // 3: left-top-back
    m->vertices[4] = (Vec3){-h, -h,  h}; // 4: left-bottom-front
    m->vertices[5] = (Vec3){ h, -h,  h}; // 5: right-bottom-front
    m->vertices[6] = (Vec3){ h,  h,  h}; // 6: right-top-front
    m->vertices[7] = (Vec3){-h,  h,  h}; // 7: left-top-front
    
    // 12 triangles (2 per face, 6 faces)
    m->triangle_count = 12;
    m->triangles = malloc(sizeof(Tri) * 12);
    
    // Front face
    m->triangles[0]  = (Tri){4, 5, 6};
    m->triangles[1]  = (Tri){4, 6, 7};
    // Back face
    m->triangles[2]  = (Tri){1, 0, 3};
    m->triangles[3]  = (Tri){1, 3, 2};
    // Left face
    m->triangles[4]  = (Tri){0, 4, 7};
    m->triangles[5]  = (Tri){0, 7, 3};
    // Right face
    m->triangles[6]  = (Tri){5, 1, 2};
    m->triangles[7]  = (Tri){5, 2, 6};
    // Top face
    m->triangles[8]  = (Tri){3, 7, 6};
    m->triangles[9]  = (Tri){3, 6, 2};
    // Bottom face
    m->triangles[10] = (Tri){0, 1, 5};
    m->triangles[11] = (Tri){0, 5, 4};
    
    // Calculate face normals
    m->normal_count = 12;
    m->normals = malloc(sizeof(Vec3) * 12);
    
    for (size_t i = 0; i < m->triangle_count; i++) {
        Tri t = m->triangles[i];
        Vec3 v0 = m->vertices[t.a];
        Vec3 v1 = m->vertices[t.b];
        Vec3 v2 = m->vertices[t.c];
        
        // Calculate normal using cross product
        Vec3 u = {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
        Vec3 v = {v2.x - v0.x, v2.y - v0.y, v2.z - v0.z};
        
        Vec3 normal = {
            u.y * v.z - u.z * v.y,
            u.z * v.x - u.x * v.z,
            u.x * v.y - u.y * v.x
        };
        
        // Normalize
        float len = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        if (len > 0.0001f) {
            normal.x /= len;
            normal.y /= len;
            normal.z /= len;
        }
        
        m->normals[i] = normal;
    }
    
    return m;
}

void mesh_destroy(Mesh* m) {
    if (!m) return;
    free(m->vertices);
    free(m->triangles);
    free(m->normals);
    free(m);
}

const Vec3* mesh_vertices(const Mesh* m, size_t* count) {
    if (count) *count = m->vertex_count;
    return m->vertices;
}

const Tri* mesh_triangles(const Mesh* m, size_t* count) {
    if (count) *count = m->triangle_count;
    return m->triangles;
}

const Vec3* mesh_normals(const Mesh* m, size_t* count) {
    if (count) *count = m->normal_count;
    return m->normals;
}