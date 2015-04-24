#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include "vmath.h"

struct mesh
{
	unsigned int id;
	GLuint shader_index;
	GLuint mvp_handle;//model view projection matrix
	GLuint view_handle;
	GLuint model_handle;
	GLuint tex_handle;//sampler2D
	GLuint tex_index;
	GLuint vao_index;//vertex array object
	GLuint vbo_index;//vertex buffer object
	GLfloat* vbo_data;
	unsigned int vbo_size;
	GLuint uvb_index;//uv buffer
	GLfloat* uvb_data;
	unsigned int uvb_size;
	GLuint nbo_index;//normal buffer
	GLfloat* nbo_data;
	unsigned int nbo_size;
};

struct wfobj
{
	vec3* vert;
	vec2* uv;
	vec3* norm;
	unsigned int size;
};

struct mesh* forge_mesh(const char* obj_path, const char* tex_path, const char* vs_path, const char* fs_path);

void wreck_mesh(struct mesh*);

void draw_mesh(struct mesh*, mat4);

#endif
