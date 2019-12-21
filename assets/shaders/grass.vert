#version 330

layout (location = 0) in vec3 a_position;

uniform mat4 view;

out struct vertex_data
{
    vec2 tex_coord;
    vec3 normal;
    vec3 position;
	int instance_id;
} vertex_out;

void main()
{
	vec4 pos = vec4(a_position, 1.0);
	float temp = pos.z;
	pos.z = -pos.y;
	pos.y = temp;
		
	vertex_out.tex_coord = a_position.xy * vec2(0.5) + vec2(0.5);
	vertex_out.normal = vec3(0, 0, 1);
	vertex_out.position = pos.xyz;
	vertex_out.instance_id = gl_InstanceID;
	
	gl_Position = pos;
}