#version 330 core

uniform sampler2D g_albedo;

in vec2 pos;

out vec4 final_color;

vec3 albedo;

void main()
{
	vec2 tex_coords = pos * vec2(0.5) + vec2(0.5);	
	tex_coords.y *= -1.0f;
	albedo = texture2D(g_albedo, tex_coords).rgb;
	final_color = vec4(albedo, 1.0);
}