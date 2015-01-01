
//WTF is this?
#include "log.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>


#include "vmath.h"


struct camera
{
	mat4 view;
	double xvel;
	double yvel;
	double zvel;

	double xpos;
	double ypos;
	double zpos;

	double vangle;
	double hangle;
	void (*apply)(GLFWwindow*);
};



struct camera* first_person_camera(GLFWwindow* window);
