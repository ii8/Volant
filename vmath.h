#ifndef VMATH_H
#define VMATH_H

#include <string.h>
#include <math.h>

typedef float vec3[3];
typedef float vec4[4];
typedef vec3 mat3[3];
typedef vec4 mat4[4];

//mat4 - ij
//|00 10 20 30|
//|01 11 21 31|
//|02 12 22 32|
//|03 13 23 33|

#define PI 3.14159265358979323846
#define IDMAT3 {{1.0,0.0,0.0},\
				{0.0,1.0,0.0},\
				{0.0,0.0,1.0}}

#define IDMAT4 {{1.0,0.0,0.0,0.0},\
				{0.0,1.0,0.0,0.0},\
				{0.0,0.0,1.0,0.0},\
				{0.0,0.0,0.0,1.0}}

void mlt4_ms(mat4 m, float s);

void mlt4_mv(vec3 v, mat4 m);

void mlt4_mm(mat4 m, mat4 n);

void add(vec3 v, vec3 u);

void sub(vec3 v, vec3 u);

void normalize(vec3 v);

double dot(vec3 v, vec3 u);

void cross(vec3 result, vec3 v, vec3 u);

void normal(vec3 normal, vec3 v, vec3 u, vec3 w);

void identity(mat4 result);

void translate(mat4 matrix, double x, double y, double z);

void scale(mat4 matrix, double x, double y, double z);

void zrotmat(mat4 matrix, double a);

void rotate(mat4 matrix, vec3 axis, double angle);

void perspective(mat4 matrix, float fov_angle, float aspect, float znear, float zfar);

#endif
