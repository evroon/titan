#version 330 core

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D noise;
uniform sampler2D kernel;

uniform mat4 projection;

uniform vec2 window_size;

uniform int kernel_size;
uniform float radius;

in vec2 pos;
out vec4 final_color;

//https://learnopengl.com/#!Advanced-Lighting/SSAO

void main()
{
	vec2 tex_coords = pos * vec2(0.5) + vec2(0.5);
	
	vec2 noise_scale = window_size / 4.0;
	
	vec3 position = texture2D(g_position, tex_coords).rgb;
	vec3 normal   = texture2D(g_normal, tex_coords).rgb;
	vec3 random   = texture2D(noise, tex_coords * noise_scale).xyz;
	
	vec3 tangent   = normalize(random - normal * dot(random, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal);
	
	float bias = 0.0;	
	float occlusion = kernel_size;
	
	for (float x = 0; x < 8; x += 1.0)
	{
		for (float y = 0; y < 8; y += 1.0)
		{
			vec3 s = texture2D(kernel, vec2(x, y) / 8.0).rgb;
			vec3 sample = position + s * radius;
			
			vec4 offset = vec4(sample, 1.0);
			offset      = projection * offset;
			offset.xyz /= offset.w;
			offset.xyz  = offset.xyz * 0.5 + 0.5;
			
			float sample_depth = texture2D(g_position, offset.xy).z;
			float intensity = smoothstep(0.0, 1.0, radius / abs(position.z - sample_depth));
			float occluded = (sample_depth >= sample.z + bias ? 1.0 : 0.0);
			
			occluded *= intensity;
			occlusion -= occluded;
		}
	}
	
	occlusion /= kernel_size;
	final_color = vec4(vec3(occlusion), 1.0);
}