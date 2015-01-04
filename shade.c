#include <GL/glew.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shade.h"
#include "log.h"

static size_t get_file_size(FILE* fp)
{
	size_t size;
	fseek(fp, 0L, SEEK_END);

	size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	return size;
}

static GLuint load_shader(const char* path, int type)
{
	char *line = NULL;
	size_t len;
	GLuint shaderID = glCreateShader(type);

	FILE* fptr;
	fptr = fopen(path, "r");

	size_t size = get_file_size(fptr);

	GLchar* shaderData = (GLchar*) malloc(size);
	memset(shaderData, 0, size);

	if(fptr != 0)
	{
		while(getline(&line, &len, fptr) != -1)
			strcat(shaderData, line);
		fclose(fptr);
	}
	else
	{
		vlog_err("Failed to open %s.\n", path);
		return 0;
	}

	glShaderSource(shaderID, 1, (const GLchar* const*)&shaderData , NULL);//WTF
	glCompileShader(shaderID);

	int params = -1;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &params);
	if(GL_TRUE != params)
	{
		vlog_err("ERROR: GL shader index %i did not compile\n", shaderID);
		int loglenmax = 2048;
		int loglen = 0;
		char log[2048];
		glGetShaderInfoLog(shaderID, loglenmax, &loglen, log);
		vlog("shader info log for GL index %u:\n%s\n", shaderID, log);
		return 0;
	}

	return shaderID;

}

GLuint load_program(const char * vs_path, const char * fs_path)
{
	GLuint vertexShaderID = load_shader(vs_path, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = load_shader(fs_path, GL_FRAGMENT_SHADER);

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	int params = -1;
	glGetProgramiv(programID, GL_LINK_STATUS, &params);
	if(GL_TRUE != params)
	{
		vlog_err("ERROR: could not link shader program GL index %u\n", programID);
		int loglenmax = 2048;
		int loglen = 0;
		char log[2048];
		glGetProgramInfoLog(programID, loglenmax, &loglen, log);
		vlog("program info log for GL index %u:\n%s", programID, log);
		return 0;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

int clear_shader(GLuint shader)
{
	if(shader)
	{
		glDeleteShader(shader);
		return 0;
	}

	return 1;
}


/*
void print_all (GLuint programme) {
  printf ("--------------------\nshader programme %i info:\n", programme);
  int params = -1;
  glGetProgramiv (programme, GL_LINK_STATUS, &params);
  printf ("GL_LINK_STATUS = %i\n", params);

  glGetProgramiv (programme, GL_ATTACHED_SHADERS, &params);
  printf ("GL_ATTACHED_SHADERS = %i\n", params);

  glGetProgramiv (programme, GL_ACTIVE_ATTRIBUTES, &params);
  printf ("GL_ACTIVE_ATTRIBUTES = %i\n", params);
  for (int i = 0; i < params; i++) {
    char name[64];
    int max_length = 64;
    int actual_length = 0;
    int size = 0;
    GLenum type;
    glGetActiveAttrib (
      programme,
      i,
      max_length,
      &actual_length,
      &size,
      &type,
      name
    );
    if (size > 1) {
      for (int j = 0; j < size; j++) {
        char long_name[64];
        sprintf (long_name, "%s[%i]", name, j);
        int location = glGetAttribLocation (programme, long_name);
        printf ("  %i) type:%s name:%s location:%i\n",
          i, GL_type_to_string (type), long_name, location);
      }
    } else {
      int location = glGetAttribLocation (programme, name);
      printf ("  %i) type:%s name:%s location:%i\n",
        i, GL_type_to_string (type), name, location);
    }
  }

  glGetProgramiv (programme, GL_ACTIVE_UNIFORMS, &params);
  printf ("GL_ACTIVE_UNIFORMS = %i\n", params);
  for (int i = 0; i < params; i++) {
    char name[64];
    int max_length = 64;
    int actual_length = 0;
    int size = 0;
    GLenum type;
    glGetActiveUniform (
      programme,
      i,
      max_length,
      &actual_length,
      &size,
      &type,
      name
    );
    if (size > 1) {
      for (int j = 0; j < size; j++) {
        char long_name[64];
        sprintf (long_name, "%s[%i]", name, j);
        int location = glGetUniformLocation (programme, long_name);
        printf ("  %i) type:%s name:%s location:%i\n",
          i, GL_type_to_string (type), long_name, location);
      }
    } else {
      int location = glGetUniformLocation (programme, name);
      printf ("  %i) type:%s name:%s location:%i\n",
        i, GL_type_to_string (type), name, location);
    }
  }

  _print_programme_info_log (programme);
}
*
*
const char* GL_type_to_string (GLenum type) {
  switch (type) {
    case GL_BOOL: return "bool";
    case GL_INT: return "int";
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
    default: break;
  }
  return "other";
}
*
*
bool is_valid (GLuint programme) {
  glValidateProgram (programme);
  int params = -1;
  glGetProgramiv (programme, GL_VALIDATE_STATUS, &params);
  printf ("program %i GL_VALIDATE_STATUS = %i\n", programme, params);
  if (GL_TRUE != params) {
    _print_programme_info_log (programme);
    return false;
  }
  return true;
}
* */
