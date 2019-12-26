#version 330 core

precision highp float;

uniform sampler2D grass;
uniform sampler2D rocks;
uniform sampler2D look_up;
uniform sampler2D shadow_map;
uniform sampler2D normalmap;
uniform sampler2D heightmap;
uniform sampler2D virtualtex;
uniform sampler2D indirection;

uniform mat4 model;
uniform vec3 ambient;

uniform vec2 selection_pos;
uniform float selection_radius;
uniform float selection_width;
uniform vec4 selection_color;
uniform vec3 light_direction;
uniform vec3 terrain_size;
uniform float water_height;

in vec3 pos;

const vec2 terrain_count = vec2(8, 8);
const vec2 page_size = vec2(128, 128);
const vec2 physical_tex_size = vec2(1024, 1024);

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
	vec2 position = pos / (terrain_size.xy * terrain_count);
	position += vec2(0.5, 0.5);
	return texture(normalmap, position).xyz;
}

vec3 get_height(vec2 pos)
{	
	vec2 position = pos / (terrain_size.xy * terrain_count);
	position += vec2(0.5, 0.5);
	return texture(heightmap, position).xyz;
}

vec3 get_color(vec3 normal)
{
	vec2 global_coords = pos.xy / terrain_count / terrain_size.xy + 0.5;
	float diff = dot(normal, vec3(0.0, 0.0, 1.0));
	vec2 tex_coords = global_coords * 500.0;
	vec3 color;
	vec3 sand_color = vec3(209, 198, 75) / 255.0;

	// sand_border is the height above water until which sand is rendered;
	// border1 is where grass starts appearing.
	// border2 is where rocks don't appear anymore.
	float sand_border = 0.5;
	float border1 = 0.65;
	float border2 = 0.75;

	if (pos.z < water_height + sand_border) {
		return sand_color;
	} else if (diff >= border2)
		color = texture2D(grass, tex_coords).rgb;
	else if (diff > border1) {
		float step = (diff - border1) / (border2 - border1);
		color = mix(texture2D(rocks, tex_coords).rgb, texture2D(grass, tex_coords).rgb, step);
	} else
		color = texture2D(rocks, tex_coords).rgb;
	
	if (pos.z < water_height + 2.0 * sand_border)
		color = mix(color, sand_color, (water_height + sand_border * 2.0 - pos.z) / sand_border);
	
	return color;
}

void main()
{
	vec3 source;
	vec2 offset = pos.xy;
	
	vec3 normal = get_normal(pos.xy);
	
	source = get_color(normal);
	source += editor_selection();
	
	gl_FragData[0] = vec4(source.rgb, 1.0);
	gl_FragData[1] = vec4(pos, 1.0);
	gl_FragData[2] = vec4(normal, 1.0);
	gl_FragData[3] = vec4(vec3(1.0, 0.0, 0.0), 1.0);
}