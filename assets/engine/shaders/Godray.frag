#version 330 core

uniform vec2 sun_pos;
uniform vec3 sun_color;
uniform sampler2D g_material;

in vec2 pos;

out vec4 final_color;

void main()
{
	float stepsize = 0.01;
	
	vec2 delta = (pos - sun_pos) * stepsize;
	float radius = length(delta);
	int steps = 100;
	
	vec2 p = sun_pos;
	float intensity = (1.0 / radius) / 10.0 / steps;
	int hits = 0;
	
	for (int c = 0; c < steps; c++)
	{
		vec3 material = texture2D(g_material, p * vec2(0.5) + vec2(0.5)).rgb;
		
		if (material.b != 0.0 || material.g != 0.0)
			hits++;

		p += delta;
	}
	
	final_color = vec4(vec3(intensity * hits / steps), 1.0);
}