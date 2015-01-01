#include "mesh.h"

static GLfloat* load_mesh(const char* path, unsigned int* size)
{
	GLfloat* buffer;

	//(buffer)[9] = malloc(sizeof(buffer));??
	buffer = (GLfloat*) malloc(sizeof(GLfloat)*216);//42

	GLfloat data[] = {
		0.0f, 0.0f, -2.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		0.0f, -2.0f, 0.0f,

		0.0f, -2.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, -1.0f, 1.0f,
		0.0f, -2.0f, 0.0f,
		1.0f, -1.0f, 1.0f,

		0.0f, -2.0f, 0.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		2.0f, 0.0f, 0.0f,

		2.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, -1.0f,
		0.0f, 0.0f, -2.0f,
		1.0f, 1.0f, -1.0f,

		2.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		2.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 2.0f,

		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		0.0f, 0.0f, 2.0f,

		0.0f, 0.0f, 2.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		0.0f, 0.0f, 2.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-2.0f, 0.0f, 0.0f,

		-1.0f, -1.0f, 1.0f,
		-2.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,

		-2.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-2.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		0.0f, 0.0f, -2.0f,

		0.0f, 0.0f, -2.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		0.0f, 2.0f, 0.0f,

		0.0f, 2.0f, 0.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		0.0f, 2.0f, 0.0f,

		0.0f, 2.0f, 0.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f};


	//GLfloat data[] = {-1.0f, -1.0f, 0.0f,
	//		1.0f, -1.0f, 0.0f,
	//		0.0f,  1.0f, 0.0f};

	memcpy(buffer, data, sizeof(GLfloat)*216);
	*size = sizeof(GLfloat)*216;
	return buffer;
}

struct mesh* forge_mesh(const char* path)
{
	static unsigned id = 0;
	struct mesh* mesh = (struct mesh*) malloc(sizeof(struct mesh));

	mesh->id = id++;
	mesh->vbo_data = load_mesh(path, &mesh->vbo_size);

	glGenBuffers(1, &mesh->vbo_index);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_index);
	glBufferData(GL_ARRAY_BUFFER, mesh->vbo_size, mesh->vbo_data, GL_STATIC_DRAW);

	glGenVertexArrays(1, &mesh->vao_index);
	glBindVertexArray(mesh->vao_index);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, mesh->vbo_index);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	vlog("forge_mesh: vbo=%i, vao=%i\n", mesh->vbo_index, mesh->vao_index);
	return mesh;
}

void wreck_mesh(struct mesh* mesh)
{
	glDeleteVertexArrays(1, &mesh->vao_index);
	glDeleteBuffers(1, &mesh->vbo_index);
	free(mesh->vbo_data);
	free(mesh);
}

void draw_mesh(struct mesh* mesh, GLuint program, GLuint MatrixID)
{
	glUseProgram(program);

	mat4 MVP = IDMAT4;
	mat4 model = IDMAT4;
	mat4 proj = IDMAT4;
	mat4 rot = IDMAT4;
	mat4 trans = IDMAT4;

	perspective(proj, 45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	vec3 axis = {0,1,0};

	static float angle = 0; angle += 0.02;



	translate(trans, 0.2, 0.5, -6.0);

	//vec3 axis = {0,1,0};
	rotate(rot, axis, angle);
	//zrotmat(rot, angle);

	//mlt4_mm(model, rot, trans);
	//mlt4_mm(model, trans, rot);

	mlt4_mm(MVP, trans);
	mlt4_mm(MVP, rot);
	mlt4_mm(MVP, proj);

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, (GLfloat*)&MVP[0][0]);


	glBindVertexArray(mesh->vao_index);
	//glDrawArrays(GL_TRIANGLES, 0, 72);

	int i;
	for(i = 0; i < 72; i += 3)
		glDrawArrays(GL_LINE_LOOP, i, 3);
	//glBindVertexArray(0);
}
