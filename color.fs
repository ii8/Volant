#version 330 core

in vec2 UV;
uniform sampler2D sampler;

out vec3 color;

void main()
{
	//gl_FragColor = texture(sampler, UV).rgb;
	color = texture(sampler, UV).rgb;
}

/*
out vec4 frag_colour;

void main()
{
	//frag_colour = vec4(0.5, 0.0, 0.5, 1.0);
	if(any(lessThan(vBC, vec3(0.02))))
	{
		frag_colour = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		frag_colour = vec4(0.5, 0.5, 0.5, 1.0);
	}

}

in vec3 vBC;

void main()
{

	if(any(lessThan(vBC, vec3(0.02))))
	{
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		gl_FragColor = vec4(0.5, 0.0, 0.5, 1.0);
	}

}
*/
