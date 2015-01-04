#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "vmath.h"
#include "shade.h"

struct mesh
{
	unsigned int id;
	GLuint shader_index;
	GLuint mvp_handle;//model view projection matrix
	GLuint tex_handle;//sampler2D
	GLuint tex_index;
	GLuint vao_index;//vertex array object
	GLuint vbo_index;//vertex buffer object
	GLfloat* vbo_data;
	unsigned int vbo_size;
	GLuint uvb_index;//uv buffer
	GLfloat* uvb_data;
	unsigned int uvb_size;
};

struct wfobj
{
	vec3* vert;
	vec3* norm;
	vec2* uv;
	unsigned int size;
};

struct mesh* forge_mesh(const char* obj_path, const char* tex_path, const char* vs_path, const char* fs_path);

void wreck_mesh(struct mesh*);

void draw_mesh(struct mesh*, mat4);

#endif
