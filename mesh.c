#include "mesh.h"

static struct wfobj* load_wavefront(const char * path)
{
	unsigned int vert_count = 0;
	unsigned int uv_count = 0;
	unsigned int norm_count = 0;
	unsigned int idx_count = 0;
	FILE * file = fopen(path, "r");
	if(file == NULL)
	{
		vlog("failed to load obj\n");
		return NULL;
	}
	while(1)
	{
		char ichiban[64];
		int res = fscanf(file, "%s", ichiban);
		if(res == EOF)
			break;
		if(strcmp(ichiban, "v") == 0)
			vert_count++;
		else if(strcmp(ichiban, "vt") == 0)
			uv_count++;
		else if(strcmp(ichiban, "vn") == 0)
			norm_count++;
		else if(strcmp(ichiban, "f") == 0)
			idx_count += 3;
		else
		{
			char meh[1000];
			fgets(meh, 1000, file);
		}
	}

	struct wfobj* object = (struct wfobj*)malloc(sizeof(struct wfobj));
	object->vert	= (vec3*)malloc(idx_count*sizeof(vec3));
	object->uv		= (vec2*)malloc(idx_count*sizeof(vec2));
	object->norm	= (vec3*)malloc(idx_count*sizeof(vec3));
	object->size	= idx_count;

	unsigned int* vert_idx_base	= (unsigned int*)malloc(idx_count*sizeof(unsigned int));
	unsigned int* uv_idx_base	= (unsigned int*)malloc(idx_count*sizeof(unsigned int));
	unsigned int* norm_idx_base	= (unsigned int*)malloc(idx_count*sizeof(unsigned int));

	unsigned int* vert_idx_top	= vert_idx_base;
	unsigned int* uv_idx_top	= uv_idx_base;
	unsigned int* norm_idx_top	= norm_idx_base;

	vec3* vert_base	= (vec3*)malloc(vert_count*sizeof(vec3));
	vec3* norm_base	= (vec3*)malloc(norm_count*sizeof(vec3));
	vec2* uv_base	= (vec2*)malloc(uv_count*sizeof(vec2));

	vec3* vert_top	= vert_base;
	vec3* norm_top	= norm_base;
	vec2* uv_top	= uv_base;

	rewind(file);
	while(1)
	{
		char ichiban[64];
		int res = fscanf(file, "%s", ichiban);
		if(res == EOF)
			break;

		if(strcmp(ichiban, "v") == 0)
		{
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
			memcpy(vert_top++, &vertex, sizeof(vec3));
		}
		else if(strcmp(ichiban, "vt") == 0)
		{
			vec2 uv;
			fscanf(file, "%f %f\n", &uv[0], &uv[1]);
			uv[1] = -uv[1];
			memcpy(uv_top++, &uv, sizeof(vec2));
		}
		else if(strcmp(ichiban, "vn") == 0)
		{
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			memcpy(norm_top++, &normal, sizeof(vec3));
		}
		else if(strcmp(ichiban,"f") == 0)
		{
			unsigned int vertex_index[3], uv_index[3], normal_index[3];
			fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertex_index[0], &uv_index[0], &normal_index[0],
				&vertex_index[1], &uv_index[1], &normal_index[1],
				&vertex_index[2], &uv_index[2], &normal_index[2]);

			*vert_idx_top++	= vertex_index[0];
			*vert_idx_top++	= vertex_index[1];
			*vert_idx_top++	= vertex_index[2];
			*uv_idx_top++	= uv_index[0];
			*uv_idx_top++	= uv_index[1];
			*uv_idx_top++	= uv_index[2];
			*norm_idx_top++	= normal_index[0];
			*norm_idx_top++	= normal_index[1];
			*norm_idx_top++	= normal_index[2];
		}
		else
		{
			char meh[1000];
			fgets(meh, 1000, file);
		}
	}
	unsigned i;
	for(i=0; i<idx_count; i++)
	{
		memcpy(object->vert[i], vert_base[vert_idx_base[i]-1], sizeof(vec3));
		memcpy(object->uv[i], uv_base[uv_idx_base[i]-1], sizeof(vec2));
		memcpy(object->norm[i], norm_base[norm_idx_base[i]-1], sizeof(vec3));
	}
	free(vert_idx_base);
	free(uv_idx_base);
	free(norm_idx_base);
	free(vert_base);
	free(norm_base);
	free(uv_base);

	return object;
}

static GLuint load_texture(const char* path)
{
	FILE* fp;
	unsigned char head[124];

	fp = fopen(path, "rb");
	if(fp == NULL)
	{
		vlog_err("Failed to open texture file %s\n", path);
		return 0;
	}

	int magic;
	fread(&magic, 4, 1, fp);
	if(magic != 0x20534444)
	{
		vlog_err("Not a dds texture file %s\n", path);
		fclose(fp);
		return 0;
	}

 	fread(&head, 124, 1, fp);

	unsigned int height		= *(unsigned int*)&(head[8]);
	unsigned int width		= *(unsigned int*)&(head[12]);
	unsigned int texsize	= *(unsigned int*)&(head[16]);
	unsigned int num_mipmap	= *(unsigned int*)&(head[24]);
	unsigned int four_cc	= *(unsigned int*)&(head[80]);

	unsigned char* buffer;
	unsigned int bufsize;

	bufsize = num_mipmap > 1 ? texsize * 2 : texsize;
	buffer = (unsigned char*)malloc(bufsize);
	fread(buffer, 1, bufsize, fp);
	fclose(fp);

	//unsigned int components = (four_cc == 0x31545844) ? 3 : 4;
	unsigned int format;
	switch(four_cc)
	{
		case 0x31545844:
			free(buffer);
			vlog_err("DXT1 is not supported, pls use 3 or 5 %s\n", path);
			return 0;
			//format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			//break;
		case 0x33545844:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case 0x35545844:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(buffer);
			vlog_err("Unsuported DDS compression format %s\n", path);
			return 0;
	}

	GLuint tex_index;
	glGenTextures(1, &tex_index);
	glBindTexture(GL_TEXTURE_2D, tex_index);

	//unsigned int block_size = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int block_size = 16;
	unsigned int offset = 0;
	unsigned int level;

	for(level = 0; level < num_mipmap && (width || height); ++level)
	{
		unsigned int size = ((width+3)/4)*((height+3)/4)*block_size;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

		offset += size;
		width  /= 2;
		height /= 2;
	}
	free(buffer);

	return tex_index;
}

void load_mesh(const char* path, GLfloat** vertex_data, GLfloat** uv_data, unsigned int* vertex_size, unsigned int* uv_size)
{
	struct wfobj* obj = load_wavefront(path);

	*vertex_size = obj->size * 3 * sizeof(GLfloat);
	*uv_size = obj->size * 2 * sizeof(GLfloat);

	*vertex_data = (GLfloat*)malloc(*vertex_size);
	*uv_data = (GLfloat*)malloc(*uv_size);

	unsigned i;
	for(i=0; i<obj->size; i++)
	{
		memcpy(*vertex_data+i*3, obj->vert+i, sizeof(vec3));
		memcpy(*uv_data+i*2, obj->uv+i, sizeof(vec2));
	}
}

struct mesh* forge_mesh(const char* obj_path, const char* tex_path, const char* vs_path, const char* fs_path)
{
	static unsigned id = 0;
	struct mesh* mesh = (struct mesh*) malloc(sizeof(struct mesh));
	memset(mesh, 0, sizeof(struct mesh));

	mesh->id = id++;
	load_mesh(obj_path, &mesh->vbo_data, &mesh->uvb_data, &mesh->vbo_size, &mesh->uvb_size);

	mesh->shader_index = load_program(vs_path, fs_path);
	mesh->mvp_handle = glGetUniformLocation(mesh->shader_index, "MVP");
	mesh->model_handle = glGetUniformLocation(mesh->shader_index, "model");
	mesh->view_handle = glGetUniformLocation(mesh->shader_index, "view");

	mesh->tex_index = load_texture(tex_path);
	mesh->tex_handle = glGetUniformLocation(mesh->shader_index, "sampler");

	glGenVertexArrays(1, &mesh->vao_index);
	glBindVertexArray(mesh->vao_index);

	glGenBuffers(1, &mesh->vbo_index);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_index);
	glBufferData(GL_ARRAY_BUFFER, mesh->vbo_size, mesh->vbo_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenBuffers(1, &mesh->uvb_index);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->uvb_index);
	glBufferData(GL_ARRAY_BUFFER, mesh->uvb_size, mesh->uvb_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenBuffers(1, &mesh->nbo_index);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->nbo_index);
	glBufferData(GL_ARRAY_BUFFER, mesh->nbo_size, mesh->nbo_data, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	return mesh;
}

void wreck_mesh(struct mesh* mesh)
{
	glDeleteVertexArrays(1, &mesh->vao_index);
	glDeleteBuffers(1, &mesh->vbo_index);
	glDeleteBuffers(1, &mesh->uvb_index);
	glDeleteBuffers(1, &mesh->nbo_index);
	glDeleteProgram(mesh->shader_index);
	glDeleteTextures(1, &mesh->tex_index);
	free(mesh->vbo_data);
	free(mesh->uvb_data);
	free(mesh->nbo_data);
	free(mesh);
}

void draw_mesh(struct mesh* mesh, mat4 view)//, mat4 proj)
{
	glUseProgram(mesh->shader_index);

	mat4 MVP = IDMAT4;
	mat4 model = IDMAT4;
	mat4 proj = IDMAT4;
	perspective(proj, 45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	//cam->getPerspective();

	static float angle = 0; angle += 0.02;
	//rotate(rot, axis, angle);
	//rotatey(model, angle);
	if(mesh->id == 1)
		translate(model, 10.0, 0.0, 0.0);
	else
		translate(model, 0.0, 2.0, -8.0);

	mlt4_mm(MVP, model);
	mlt4_mm(MVP, view);
	mlt4_mm(MVP, proj);

	glUniformMatrix4fv(mesh->mvp_handle, 1, GL_FALSE, (GLfloat*)&MVP[0][0]);
	glUniformMatrix4fv(mesh->model_handle, 1, GL_FALSE, (GLfloat*)&model[0][0]);
	glUniformMatrix4fv(mesh->view_handle, 1, GL_FALSE, (GLfloat*)&view[0][0]);

	glActiveTexture(GL_TEXTURE0+mesh->id);//works without +id
	glBindTexture(GL_TEXTURE_2D, mesh->tex_index);
	glUniform1i(mesh->tex_handle, mesh->id);//same??


	GLuint light = glGetUniformLocation(mesh->shader_index, "light");
	vec3 lightPos = {4.0,4.0,4.0};
	glUniform3f(light, lightPos[0], lightPos[1], lightPos[2]);

	glBindVertexArray(mesh->vao_index);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vbo_size);

	//int i;
	//for(i = 0; i < 72; i += 3)
	//	glDrawArrays(GL_LINE_LOOP, i, 3);
}
