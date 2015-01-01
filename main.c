#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "shade.h"
#include "mesh.h"
#include "input.h"
#include "log.h"


void _update_fps_counter (GLFWwindow* window)
{
	static double previous_seconds = 0;
	static int frame_count;
	double current_seconds = glfwGetTime ();
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
		vlog_err ("ERROR: could not start GLFW3\n");
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);//anti alias

	//GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	//const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
	//if fullscreen
		//GLFWwindow* window = glfwCreateWindow(vidmode->width, vidmode->height, "Volant Engine", monitor, NULL);
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Volant Engine", NULL, NULL);

	if(!window)
	{
		glfwTerminate();
		vlog_err("Failed to create window");
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	vlog_params();
	struct camera* cam = first_person_camera(window);
	//mat4 proj = IDMAT4;
	//perspective(proj, 45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.598, 0.863, 0.945, 0);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	struct mesh* mycoolmesh = forge_mesh("davepls");
	GLuint shaderProgramID = load_program( "transform.vs", "color.fs" );

	GLuint mvp_id = glGetUniformLocation(shaderProgramID, "MVP");
	//GLuint view_index = glGetUniformLocation(shaderProgramID, "view");
	//GLuint proj_index = glGetUniformLocation(shaderProgramID, "proj");

	while(!glfwWindowShouldClose(window))
	{
		_update_fps_counter (window);
		glClear(GL_COLOR_BUFFER_BIT);


		cam->apply(window);

		//glUniformMatrix4fv(view_index, 1, GL_FALSE, (GLfloat*)&cam->view[0][0]);
		//glUniformMatrix4fv(proj_index, 1, GL_FALSE, (GLfloat*)&proj[0][0]);
		draw_mesh(mycoolmesh, shaderProgramID, mvp_id, cam->view);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	vlog_glerr();
	wreck_mesh(mycoolmesh);
	glDeleteProgram(shaderProgramID);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
