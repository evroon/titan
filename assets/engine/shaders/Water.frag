#version 330 core

uniform sampler2D normals;
uniform sampler2D reflection_tex;

uniform vec3 light_direction;
uniform vec3 light_color;
uniform vec3 ambient;
uniform vec3 camera_position;
uniform float time;

in vec3 position;
in vec4 screenspace_position;

void main()
{
	vec3 light_dir = light_direction;
	
	float t = time / 10000.0;
	float scale_xy = 5.0f;
	
	vec3 n1 = texture2D(normals, position.xy / scale_xy + vec2(t / 1.1, t)).rgb;
	vec3 n2 = texture2D(normals, position.xy / scale_xy + vec2(t, -t)).rgb;
	
	vec3 normal = (n1.xyz + n2.xyz) / 2.0;
	normal = normal * 2.0 - 1.0;
	normal *= -1.0;
	normal = normalize(normal);
	
	vec2 ndc = (screenspace_position.xy / screenspace_position.w) * 0.5 + 0.5;
	ndc -= abs(normal.xy) / 1000.0;
	
	vec4 reflect_color = texture2D(reflection_tex, ndc * vec2(1, -1));
	
	vec3 result = (ambient) * reflect_color.xyz + ambient * 0.3;
	
	gl_FragData[0] = vec4(result, 1.0);
	gl_FragData[1] = vec4(position, 1.0);
	gl_FragData[2] = vec4(normal, 1.0);
	gl_FragData[3] = vec4(0.0, 0.0, 1.0, 1.0);
}