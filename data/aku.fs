#version 330 core

in vec2 UV;
in vec3 pos;
in vec3 normal;
in vec3 eye_dir;
in vec3 light_dir;
out vec3 color;

uniform sampler2D sampler;
uniform mat4 MV;
uniform vec3 light;

void main()
{
	vec3 LightColor = vec3(1,0.9,0.8);
	float LightPower = 150.0f;

	vec3 MaterialDiffuseColor = texture2D( sampler, UV ).rgb;
	vec3 MaterialAmbientColor = vec3(0.2,0.2,0.2) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	float distance = length(light - pos);

	vec3 n = normalize( normal );
	vec3 l = normalize( light_dir );
	float cosTheta = clamp( dot( n,l ), 0,1 );

	vec3 E = normalize(eye_dir);
	vec3 R = reflect(-l,n);
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	color =
		MaterialAmbientColor +
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);

}
