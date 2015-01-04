
#include "camera.h"

static void _reset_callback(GLFWwindow* window)
{
	glfwSetCursorPosCallback(window, NULL);
}

static void _fpc_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	struct camera* cam = glfwGetWindowUserPointer(window);

	switch(key)
	{
		case GLFW_KEY_W:
			if(action == GLFW_PRESS)
				cam->vel[2] = 0.4;
			else if(action == GLFW_RELEASE)
				cam->vel[2] = 0.0;
			break;
		case GLFW_KEY_A:
			if(action == GLFW_PRESS)
				cam->vel[0] = 0.4;
			else if(action == GLFW_RELEASE)
				cam->vel[0] = 0.0;
			break;
		case GLFW_KEY_S:
			if(action == GLFW_PRESS)
				cam->vel[2] = -0.4;
			else if(action == GLFW_RELEASE)
				cam->vel[2] = 0.0;
			break;
		case GLFW_KEY_D:
			if(action == GLFW_PRESS)
				cam->vel[0] = -0.4;
			else if(action == GLFW_RELEASE)
				cam->vel[0] = 0.0;
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);

	}
}

static void _fpc_apply(GLFWwindow* window)
{
	struct camera* cam = glfwGetWindowUserPointer(window);
	double xpos, ypos;
	int xwin, ywin;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwGetWindowSize(window, &xwin, &ywin);
	xwin /= 2;
	ywin /= 2;


	cam->hangle -= ((double)xpos-xwin)/1000;
	cam->vangle -= ((double)ypos-ywin)/1000;

	//if(cam->hangle > 2*PI)
	//	cam->hangle -= 2*PI;
	//if(cam->vangle > 2*PI)
	//	cam->vangle -= 2*PI;
	//is there way to do this more efficiently?
	identity(cam->view);
	rotatey(cam->view, cam->hangle);

	vec4 vel4 = {cam->vel[0], cam->vel[1], cam->vel[2], 0};
	mlt4_mv(vel4, cam->view);
	add(cam->pos, vel4);

	identity(cam->view);

	translate(cam->view, cam->pos[0], cam->pos[1], cam->pos[2]);
	rotatey(cam->view, cam->hangle);
	rotatex(cam->view, cam->vangle);

	//horizontalAngle += mouseSpeed * deltaTime * float(1024/2 - xpos );
	//verticalAngle   += mouseSpeed * deltaTime * float( 768/2 - ypos );

	glfwSetCursorPos(window, xwin, ywin);
}
/*
static void _fpc_pos_callback(GLFWwindow* window, double x, double y)
{
	mat4 matrix_x = IDMAT4;
	//mat4 matrix_y;

	struct camera* cam = glfwGetWindowUserPointer(window);
	rotatex(matrix_x, x-cam->centerx);
	//rotatey(matrix_y, y);

	mlt4_mm(cam->view, matrix_x);
}
*/

struct camera* first_person_camera(GLFWwindow* window)
{
	/*
	struct camera camera;
	struct camera* cam = &camera;
	memset(cam->view, 0, sizeof(mat4));
	identity(cam->view);
	*/

	struct camera* cam = malloc(sizeof(struct camera));
	memset(cam, 0, sizeof(struct camera));
	identity(cam->view);
	cam->apply = &_fpc_apply;

	glfwSetWindowUserPointer(window, cam);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	//glfwSetCursorPosCallback(window, _fpc_pos_callback);

	glfwSetKeyCallback(window, _fpc_key_callback);

	return cam;
}
