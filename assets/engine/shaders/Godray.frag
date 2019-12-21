#version 330 core

uniform vec2 sun_pos;
uniform vec3 sun_color;
uniform sampler2D g_material;

in vec2 pos;

out vec4 final_color;

void main()
{
	float stepsize = 0.01;
	int steps = 100;
	
	vec2 tex_coords = pos * vec2(0.5) + vec2(0.5);
	
	vec2 l = pos - sun_pos;
	vec2 delta = l * stepsize;
	
	vec2 p = sun_pos;
	float intensity = 0.0;
	
	for (int c = 0; c < steps; c++)
	{	
		vec3 material = texture2D(g_material, p * vec2(0.5) + vec2(0.5)).rgb;
		
		if (material.z < 0.2 && material.x == 0.0 && material.y == 0.0)
			intensity += 0.02;
		
		p += delta;
	}
	
	final_color = vec4(vec3(intensity), 1.0);
}