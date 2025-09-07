#include "math3d.h"
#include <math.h>

// Initators
Vec3 vec3(float x, float y, float z) { return (Vec3){ x, y, z }; }
Vec4 vec4(float x, float y, float z, float w ) { return (Vec4){ x, y, z, w}; }
Mat4 mat4_identify(void) { 
    return (Mat4){ .m = {
        1, 0, 0, 0,
        0, 1, 0, 0, 
        0, 0, 1, 0, 
        0, 0, 0, 1
    }};
}


Mat4 mat4_mul(Mat4 A, Mat4 B) {
    Mat4 R = { 0 };
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            for (int k = 0; k < 4; k++) {
                R.m[r * 4 + c] += A.m[r * 4 + k] * B.m[k * 4 + c];
            }
        }
    }

    return R;
}

Vec4 vec4_mul(Vec4 v, Mat4* restrict M) {
    const float x = v.x;
    const float y = v.y;
    const float z = v.z;
    const float w = v.w;
    const float* restrict m = M -> m;

    Vec4 r;
    r.x = x*m[0] + y*m[1] + z*m[2]  + w*m[3];
    r.y = x*m[4] + y*m[5] + z*m[6]  + w*m[7];
    r.z = x*m[8] + y*m[9] + z*m[10] + w*m[11];
    r.w = x*m[12]+ y*m[13]+ z*m[14] + w*m[15];

    return r;
}


Mat4 mat4_rx(float a) {
    float c = cosf(a);
    float s = sinf(a);
    return (Mat4) { .m = {
        1,  0,  0,  0,
        0,  c , -s, 0,
        0,  s,  c,  0,
        0,  0,  0,  1
    }};
}

Mat4 mat4_ry(float a) {
    float c = cosf(a);
    float s = sinf(a);
    return (Mat4) { .m = {
        c,  0,  s,  0,
        0,  1,  0,  0,
        -s, 0,  c,  0,
        0,  0,  0,  1
    }};
}

Mat4 mat4_rz(float a) {
    float c = cosf(a);
    float s = sinf(a);
    return (Mat4) { .m = {
        c,  -s, 0,  0,
        s,  c,  0,  0,
        0,  0,  1,  0, 
        0,  0,  0,  1
    }};
}


Mat4 mat4_perspective(
    float fovy_rad, 
    float aspect, 
    float zn, 
    float zf
) {
    float f = 1.0f / tanf(fovy_rad * 0.5f);
    return (Mat4) { .m = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (zn + zf) / (zn - zf), (2 * zn * zf) / (zn - zf),
        0, 0, -1, 0
    }};
}