#version 330 core

uniform vec3 sun_color;
uniform vec3 sun_direction;
uniform vec3 sky_color;

in vec3 position;

void main()
{
	float dot = max(0.0, dot(normalize(position), -sun_direction));
	float intensity = (exp(pow(dot, 500)) - 1.0);
	vec3 sun;
	
	if (intensity > 1.0)
	{
		sun = intensity * vec3(1.0);
		gl_FragData[3] = vec4(0.0, 0.0, 0.1, 1.0);
	}
	else
		gl_FragData[3] = vec4(1.0, 0.0, 1.0, 1.0);
	
	
	vec3 result = sky_color + sun;
	
	gl_FragData[0] = vec4(result, 1.0);
}