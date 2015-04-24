#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "mesh.h"
#include "camera.h"
#include "log.h"

void _update_fps_counter(GLFWwindow* window)
{
	static double previous_seconds = 0;
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.25)
	{
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		sprintf (tmp, "Volant Engine @ fps: %.2f", fps);
		glfwSetWindowTitle (window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

}


//vexit(status)
//wreck all meshaes and objects
//terminate gl glfw etc
//exit(status)

int main(int argc, char** argv)
{
	//logging
	vlog_start();
	vlog("starting GLFW: %s\n", glfwGetVersionString());
	glfwSetErrorCallback(glfw_error_callback);

	if(!glfwInit())
	{
		vlog_err("ERROR: could not start GLFW3\n");
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 8);//anti alias

	//GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	//const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
	//if fullscreen
		//GLFWwindow* window = glfwCreateWindow(vidmode->width, vidmode->height, "Volant Engine", monitor, NULL);
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Volant Engine", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if(!window)
	{
		glfwTerminate();
		vlog_err("Failed to create window\n");
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	vlog_params();
	struct camera* cam = first_person_camera(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.598, 0.863, 0.945, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	struct mesh* akumesh = forge_mesh("data/aku.obj", "data/aku.dds", "data/aku.vs", "data/aku.fs");
	struct mesh* skullmesh = forge_mesh("data/skull.obj", "data/skull.dds", "data/aku.vs", "data/aku.fs");

	//mat4 view;
	//mat4 proj;
	while(!glfwWindowShouldClose(window))
	{
		_update_fps_counter (window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam->apply(window);
		//proj = cam->get_proj(window);
		draw_mesh(akumesh, cam->view);
		draw_mesh(skullmesh, cam->view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	vlog_glerr();
	wreck_mesh(akumesh);
	wreck_mesh(skullmesh);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
