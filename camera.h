#include "vmath.h"

struct camera
{
	mat4 view;
	vec3 vel;
	vec3 pos;

	double vangle;
	double hangle;
	float fov;

	void (*apply)(GLFWwindow*);
	void (*get_view)(GLFWwindow*);
	void (*get_proj)(GLFWwindow*);
};



struct camera* first_person_camera(GLFWwindow* window);
