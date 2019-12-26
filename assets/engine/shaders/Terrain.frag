#version 330 core

precision highp float;

uniform sampler2D grass;
uniform sampler2D rocks;
uniform sampler2D look_up;
uniform sampler2D shadow_map;

uniform mat4 model;
uniform vec3 ambient;

in vec2 texcoord;
in vec3 normal;
in vec3 pos;

out vec4 final_color;

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

void main()
{	
	vec3 source;
	vec2 offset = texcoord * vec2(512);
	
	vec3 atlas = sampler2D(look_up, pos.xy).rgb;
	
	if (atlas.r > 0.7)
		source = texture2D(grass, offset).rgb;
	else
		source = texture2D(rocks, offset).rgb;
			
	gl_FragData[0] = vec4(source, alpha);
	gl_FragData[1] = vec4(position, 1.0);
	gl_FragData[2] = vec4(normal, 1.0);
	gl_FragData[3] = vec4(vec3(1.0, 0.0, 0.0), 1.0);
}