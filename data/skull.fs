#version 330 core

in vec2 UV;
uniform sampler2D sampler;

out vec3 color;

void main()
{
	color = texture(sampler, UV).rgb;
}
