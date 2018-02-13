#version 430

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 view;
uniform sampler2D heightmap;

const vec2 terrain_size = vec2(10, 10);
const vec2 terrain_count = vec2(64, 64);
const float max_height = 50.0;

out vec3 pos;

float get_height(vec2 pos)
{
	vec2 position = pos / (terrain_size * terrain_count);
	position += vec2(0.5, 0.5);
	return texture(heightmap, position).x * max_height;
}

void main()
{
	for (int i = 0; i < gl_in.length(); i++)
	{
		vec4 position = gl_in[i].gl_Position;
		position.z = get_height(position.xy);
		
		pos = position.xyz;
		gl_Position = view * position;
		EmitVertex();
	}
	
	EndPrimitive();
}