#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "vmath.h"
/*
typedef struct _vec3
{
	float x;
	float y;
	float z;
}vec3;

typedef struct _vec4
{
	float x;
	float y;
	float z;
	float w;
}vec4;

typedef struct _mat4
{
	vec4 a;//|ax bx cx dx|
	vec4 b;//|ay by cy dy|
	vec4 c;//|az bz cz dz|
	vec4 d;//|aw bw cw dw|
}mat4;
*/
struct mesh
{
	unsigned int id;
	GLuint vao_index;
	GLuint vbo_index;
	GLfloat* vbo_data;
	unsigned int vbo_size;
};

struct mesh* forge_mesh(const char*);

void wreck_mesh(struct mesh*);

void draw_mesh(struct mesh*, GLuint, GLuint);

#endif
