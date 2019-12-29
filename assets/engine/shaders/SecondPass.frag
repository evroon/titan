#version 330 core

uniform sampler2D render_buffer;
uniform sampler2D blur_buffer;
uniform sampler2D depth_buffer;
uniform sampler2D bloom_buffer;
uniform float gamma;
uniform float exposure;
uniform float dof_focus;
uniform float dof_rate;

uniform bool dof_enabled;
uniform bool bloom_enabled;

in vec2 pos;
out vec4 final_color;

vec3 get_color()
{
	vec2 tex_coords = pos * vec2(0.5) + vec2(0.5);
	vec3 render_color = texture2D(render_buffer, tex_coords).rgb;

	if (!dof_enabled)
		return render_color;
	
	vec3 blur_color = texture2D(blur_buffer, tex_coords).rgb;
	float depth = texture2D(depth_buffer, tex_coords).r;	
	float blur_factor = pow(abs(dof_focus - depth), dof_rate);

	vec3 bloom_color = vec3(0.0);

	if (bloom_enabled)
		bloom_color = texture2D(bloom_buffer, tex_coords).rgb;

	return mix(render_color, blur_color, blur_factor) + bloom_color;
}

void main()
{	
	vec3 hdr = get_color();
    vec3 mapped = vec3(1.0) - exp(-hdr * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));
	
	final_color = vec4(mapped, 1.0);
}