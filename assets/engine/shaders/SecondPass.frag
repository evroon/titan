#version 330 core

uniform sampler2D render_buffer;
uniform float gamma;
uniform float exposure;

in vec2 pos;
out vec4 final_color;

void main()
{
	vec2 tex_coords = pos * vec2(0.5) + vec2(0.5);
	
	vec3 hdr = texture2D(render_buffer, tex_coords).rgb;
	
    vec3 mapped = vec3(1.0) - exp(-hdr * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));
	
	final_color = vec4(mapped, 1.0);
}