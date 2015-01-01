#version 330

in vec3 vp;
uniform mat4 MVP;

void main()
{

    vec4 v = vec4(vp,1);
    gl_Position = MVP * v;
}
