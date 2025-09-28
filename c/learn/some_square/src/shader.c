#include "shader.h"
#include <math.h>

static inline char ascii_from_brightness(float b, const char* ramp) {
    if (b < 0) b = 0; 
    if (b > 1) b = 1;

    size_t n = 0;
    while (ramp[n]) ++n;
    size_t i = (size_t)(b * (n - 1) + 0.5f);
    
    return ramp[i];
}

// Lambert shading data structure
typedef struct {
    float lx, ly, lz;
    const char* ramp;
} LambertData;

static char lambert_shade_char(float brightness, void* user) {
    LambertData* data = (LambertData*)user;
    return ascii_from_brightness(brightness, data->ramp);
}

static float lambert_brightness_face(Vec3 face_normal, void* user) {
    LambertData* data = (LambertData*)user;
    
    // Dot product with light direction
    float dot = face_normal.x * data->lx + 
                face_normal.y * data->ly + 
                face_normal.z * data->lz;
    
    // Clamp to [0,1] and add some ambient lighting
    float brightness = fmaxf(0.0f, dot) * 0.8f + 0.2f;
    return fminf(1.0f, brightness);
}

static float lambert_brightness_vertex(Vec3 vertex_normal, void* user) {
    return lambert_brightness_face(vertex_normal, user);
}

Shader shader_make_lambert(const ShaderLambertCfg* cfg) {
    static LambertData data;
    data.lx = cfg->lx;
    data.ly = cfg->ly;
    data.lz = cfg->lz;
    data.ramp = cfg->ramp;
    
    Shader shader = {
        .shade_char = lambert_shade_char,
        .brightness_face = lambert_brightness_face,
        .brightness_vertex = lambert_brightness_vertex,
        .user = &data
    };
    
    return shader;
}