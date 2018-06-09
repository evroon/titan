#version 330 core

precision highp float;

uniform sampler2D grass;
uniform sampler2D rocks;
uniform sampler2D look_up;
uniform sampler2D shadow_map;
uniform sampler2D normalmap;
uniform sampler2D heightmap;

uniform mat4 model;
uniform vec3 ambient;

uniform vec2 selection_pos;
uniform float selection_radius;
uniform float selection_width;
uniform vec4 selection_color;
uniform vec3 light_direction;

in vec3 pos;

const vec2 terrain_size = vec2(10, 10);
const vec2 terrain_count = vec2(64, 64);

float bandpass(float x, float start, float edge1, float edge2, float end)
{
	if (x < start)
		return 0.0;
	if (x > end)
		return 0.0;
	if (x < edge1)
		return smoothstep(start, edge1, x);
	if (x > edge2)
		return smoothstep(-end, -edge2, -x);
	
	return 1.0;
}

vec3 editor_selection()
{
	float r = length(pos.xy - selection_pos);
	
	float off1 = .5;
	float off2 = .5;
	
	return selection_color.xyz * bandpass(r, selection_radius, selection_radius + selection_width * off1, selection_radius + selection_width * off2, selection_radius + selection_width);
}

vec3 get_normal(vec2 pos)
{	
	vec2 position = pos / (terrain_size * terrain_count);
	position += vec2(0.5, 0.5);
	return texture(normalmap, position).xyz;
}

vec3 get_height(vec2 pos)
{	
	vec2 position = pos / (terrain_size * terrain_count);
	position += vec2(0.5, 0.5);
	return texture(heightmap, position).xyz;
}

void main()
{
	vec3 source;
	vec3 atlas = texture2D(look_up, pos.xy / 100.0).rgb;
	vec2 offset = pos.xy;
	
	if (atlas.r < 0.5)
		source = texture(grass, offset).rgb;
	else
		source = texture(rocks, offset).rgb;
		
	source += editor_selection();
	
	vec3 normal = get_normal(pos.xy);
			
	gl_FragData[0] = vec4(source.rgb, 1.0);
	gl_FragData[1] = vec4(pos, 1.0);
	gl_FragData[2] = vec4(normal, 1.0);
	gl_FragData[3] = vec4(vec3(1.0, 0.0, 0.0), 1.0);
}