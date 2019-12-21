#include "Mat4.h"

void mat4::perspective(const rect2& p_window, float fov, float aspect, float p_near, float p_far)
{
	float y = Math::tan(fov * PI / 2.0f / 180.0f);
	float x = y / aspect;

	m[0] = 1.0f / x; m[4] = 0.0;      m[8] = 0.0;									m[12] = 0.0;
	m[1] = 0.0;      m[5] = 1.0f / y; m[9] = 0.0;									m[13] = 0.0;
	m[2] = 0.0;      m[6] = 0.0;      m[10] = -(p_far + p_near) / (p_far - p_near); m[14] = -(2.0f * p_far * p_near) / (p_far - p_near);
	m[3] = 0.0;      m[7] = 0.0;      m[11] = -1.0;									m[15] = 0.0;
}

void mat4::orthographic(const rect2& p_window, float p_near, float p_far)
{
	float left = p_window.get_left();
	float right = p_window.get_right();
	float top = p_window.get_top();
	float bottom = p_window.get_bottom();

	m[0] = 2 / (right - left);	m[4] = 0.0f;				m[8] = 0.0f;					m[12] = -(right + left) / (right - left);
	m[1] = 0.0f;				m[5] = 2 / (top - bottom);	m[9] = 0.0f;					m[13] = -(top + bottom) / (top - bottom);
	m[2] = 0.0f;				m[6] = 0.0f;				m[10] = -2 / (p_far - p_near);	m[14] = -(p_far + p_near) / (p_far - p_near);
	m[3] = 0.0f;				m[7] = 0.0f;				m[11] = 0.0f;					m[15] = 1.0f;
}

void mat4::look_at(const vec3& p_eye, const vec3& p_target, const vec3& p_up)
{
	//https://www.3dgep.com/understanding-the-view-matrix/

	vec3 forward = (p_target - p_eye).normalize();
	vec3 right = p_up.cross(forward).normalize();
	vec3 up = forward.cross(right);

	mat4 orientation =
	{
		vec4(right, 0),
		vec4(up, 0),
		vec4(forward, 0),
		vec4(vec3(), 1)
	};

	mat4 translation;
	translation.translate(vec3(-1.0f) * p_eye);

	*this = orientation * translation;
}