#include "log.h"

int _vlog_glerr(char *file, int line)
{

	GLenum glErr;
	int retCode = 0;

	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		vlog_err("glError in file %s @ line %d: %x\n", file, line, glErr);
		retCode = 1;
	}
	return retCode;
}

void vlog_start()
{
	FILE* file = fopen(LOG_FILE, "w");
	if(!file)
	{
		fprintf(stderr, "ERROR: could not open LOG_FILE log file %s for writing\n", LOG_FILE);
		return;
	}
	fprintf (file, "%s - %s\n\n", __DATE__, __TIME__);
	fclose (file);
	return;
}


void vlog(const char* message, ...)
{
	va_list argptr;
	FILE* file = fopen (LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "ERROR: could not open LOG_FILE %s file for appending\n", LOG_FILE);
		return;
	}
	va_start (argptr, message);
	vfprintf (file, message, argptr);
	va_end (argptr);
	fclose (file);
	return;
}

void vlog_err(const char* message, ...)
{
	va_list argptr;
	FILE* file = fopen (LOG_FILE, "a");
	if (!file)
	{
		fprintf(stderr, "ERROR: could not open LOG_FILE %s file for appending\n", LOG_FILE);
		return;
	}

	va_start (argptr, message);
	vfprintf (file, message, argptr);
	va_end (argptr);
	va_start (argptr, message);
	vfprintf (stderr, message, argptr);
	va_end (argptr);
	fclose (file);
	return;
}

void glfw_error_callback (int error, const char* description)
{
	vlog_err ("GLFW ERROR: code %i msg: %s\n", error, description);
}


void vlog_params()
{
	int params[] =
	{
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
	};
	const char* names[] =
	{
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};

	vlog("Renderer: %s\n", glGetString (GL_RENDERER));
	vlog("OpenGL version supported %s\n", glGetString (GL_VERSION));

	vlog ("GL Context Params:\n");
	// integers - only works if the order is 0-10 integer return types
	int i;
	for(i = 0; i < 10; i++)
	{
		int v = 0;
		glGetIntegerv(params[i], &v);
		vlog ("%s %i\n", names[i], v);
	}
	// others
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	vlog("%s %i %i\n", names[10], v[0], v[1]);

	unsigned char s = 0;
	glGetBooleanv (params[11], &s);
	vlog("%s %u\n", names[11], (unsigned int)s);
	vlog("-----------------------------\n");
}
