#ifndef SHADER_H
#define SHADER_H

#include "math3d.h"

// Using Lambert
typedef struct {
    float lx, ly, lz;
    const char* ramp;
} ShaderLambertCfg;

typedef struct {
    char (*shade_char)(float brightness, void* user);
    float (*brightness_face)(Vec3 face_normal, void* user);
    float (*brightness_vertex)(Vec3 vertex_normal, void *user);
    void* user;
} Shader;

Shader shader_make_lambert(const ShaderLambertCfg* cfg);

#endif