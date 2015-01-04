#version 330 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;
uniform mat4 MVP;

void main()
{
	//gl_Position =  MVP * vec4(UV, 1, 1) * vec4(vp,1);
	gl_Position =  MVP * vec4(vp,1);
	UV = vertexUV;
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
