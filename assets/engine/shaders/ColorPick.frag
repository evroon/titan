#version 330 core

precision highp float;

in vec2 pos;
out vec4 final_color;

uniform vec3 color_hsv;

#define PI 3.1415

vec3 hsv_to_rgb(vec3 hsv)
{	
	float h = hsv.x;
	float s = hsv.y;
	float v = hsv.z;

	float q = h * 6.0;

	float c = v * s;
	float x = c * (1.0 - abs(int(q) % 2 + q - int(q) - 1.0));
	
	float m = v - c;

	switch (int(q))
	{
	case 0:
		return vec3(c, x, 0.0) + vec3(m);
	case 1:
		return vec3(x, c, 0.0) + vec3(m);
	case 2:
		return vec3(0.0, c, x) + vec3(m);
	case 3:
		return vec3(0.0, x, c) + vec3(m);
	case 4:
		return vec3(x, 0.0, c) + vec3(m);
	case 5:
		return vec3(c, 0.0, x) + vec3(m);
	}
	
	return vec3(0.0, 0.0, 0.0);
}

void main()
{
	vec2 tex_coords = pos * vec2(0.5) + vec2(0.5);
	
	float angle = atan(pos.y, pos.x);
	float radius = length(pos);
	float alias = 0.001;
	float target_angle = color_hsv.x - PI;
	
	if (angle < target_angle + alias && angle > target_angle - alias && radius <= 1.0)
	{
		final_color = vec4(0.6, 0.6, 0.6, 1.0);
		return;
	}
	
	float h = (angle + PI) / 2.0 / PI;
	float s = radius;
	float v = color_hsv.z;
	float a = 1.0;
	
	if (radius > 1.0)
		a = 0.0;
	
	final_color = vec4(hsv_to_rgb(vec3(h, s, v)), a);
}