#version 330
 
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

#define MAX_GRASS_INSTANCES 2048

in struct vertex_data
{
    vec2 tex_coord;
    vec3 normal;
    vec3 position;
	int instance_id;
} vertex_out[3];

out struct geom_data
{
    vec2 tex_coord;
    vec3 normal;
    vec3 position;
} geom_out;

layout(std140) uniform matrix_block
{
	vec4 pos[2 * MAX_GRASS_INSTANCES];
};

uniform mat4 view;
uniform vec3 block_offset;
uniform sampler2D heightmap;

const vec2 terrain_size = vec2(10, 10);
const vec2 terrain_count = vec2(64, 64);
const float max_height = 50.0;

float get_height(vec2 pos)
{
	vec2 position = pos / (terrain_size * terrain_count);
	position += vec2(0.5, 0.5);
	return texture(heightmap, position).x * max_height;
}

void main()
{
	for (int i = 0; i < 3; i++)
	{
		vec3 position = (vertex_out[i].position + 1.0) / 8.0;
		
		if (vertex_out[i].instance_id % 2 == 0)
		{
			float temp = position.x + 0.5;
			position.x = position.y - 0.5;
			position.y = temp;
		}
		int instance = vertex_out[i].instance_id;
		
		vec3 final_position = position + pos[instance].xyz + block_offset;
		final_position.z += get_height(final_position.xy);
		
		vec4 p = view * vec4(final_position, 1.0);
		
		gl_Position = p;
		geom_out.position = final_position;
		geom_out.normal = vertex_out[i].normal;
		geom_out.tex_coord = vertex_out[i].tex_coord;
 
		EmitVertex();
	}
}
