#include <stdio.h>
#include <stdlib.h>

//#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

GLFWwindow* volant_init(int width, int height)
{
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
	
	
	GLFWwindow* window;
	window = glfwCreateWindow(width, height, "Volant Engine", NULL, NULL);
	if(window == NULL)
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return NULL;
	}
	
	
	glfwMakeContextCurrent(window);
	
	if(glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return NULL;
	}
	
	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	return window;
	
}


