#version 330 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec2 vuv;
layout(location = 1) in vec3 vn;

out vec2 UV;
out vec3 pos;
out vec3 normal;
out vec3 eye_dir;
out vec3 light_dir;

uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light;

void main()
{
	gl_Position =  MVP * vec4(vp,1);

	pos = (model * vec4(vp,1)).xyz;

	vec3 pos = ( view * model * vec4(vp,1)).xyz;
	eye_dir = vec3(0,0,0) - pos;

	vec3 light_pos = ( view * vec4(light,1)).xyz;
	light_dir = light_pos + eye_dir;

	//use inverse transpose
	normal = ( view * model * vec4(vn,0)).xyz;

	UV = vuv;
}
/*
#version 330 core

in vec3 vp;
uniform mat4 MVP;

void main()
{

    vec4 v = vec4(vp,1);
    gl_Position = MVP * v;
}
*/
