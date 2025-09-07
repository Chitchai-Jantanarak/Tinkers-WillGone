#ifndef MATH3D_H
#define MATH3D_H

typedef struct { float x, y, z; }    Vec3;
typedef struct { float x, y, z, w; } Vec4;
typedef struct { float m[16]; }      Mat4;

Vec3 vec3(float x, float y, float z);
Vec4 vec4(float x, float y, float z, float w);
Mat4 mat4_identify(void);

Mat4 mat4_rx(float a);
Mat4 mat4_ry(float a);
Mat4 mat4_rz(float a);

Mat4 mat4_mul(Mat4 a, Mat4 b);
Vec4 vec4_mul(Vec4 v, Mat4* restrict m);

Mat4 mat4_perspective(float fovy_rad, float aspect, float zn, float zf);

#endif