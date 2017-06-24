#pragma once

#include <math.h>

#include "core/Array.h"
#include "Vec2.h"
#include "Vec3.h"

#define PI 3.14159265359f
#define TWOPI PI * 2.0f
#define HALFPI PI / 2.0f
#define QUARTERPI PI / 4.0f
#define ROOT2 1.414214f

#define RAD_TO_DEG 180.0f / PI
#define DEG_TO_RAD PI / 180.0f

#define EPSILON 1.0e-6f

#define MIN(X, Y) X < Y ? X : Y
#define MAX(X, Y) X > Y ? X : Y

#define to_float static_cast<float>
#define to_int static_cast<int>

class Math
{
public:

	static inline int mod(int p_x, int p_y) { return p_x % p_y; }

	static inline float sin(float p_x) { return ::sin(p_x); }
	static inline float asin(float p_x) { return ::asin(p_x); }

	static inline float cos(float p_x) { return ::cos(p_x); }
	static inline float acos(float p_x) { return ::acos(p_x); }

	static inline float tan(float p_x) { return ::tan(p_x); }
	static inline float atan(float p_x) { return ::atan(p_x); }

	static inline float sqrt(float p_x) { return ::sqrtf(p_x); }
	static inline float pow(float p_x) { return p_x * p_x; }
	static inline float pow(float p_x, float p_y) { return ::pow(p_x, p_y); }

	static inline float exp(float p_x) { return ::exp(p_x); }
	static inline float log(float p_x) { return ::log(p_x); }

	static inline float floor(float p_x) { return ::floor(p_x); }
	static inline float ceil(float p_x) { return ::ceil(p_x); }

	static inline float lerp(float p_x, float p_y, float p_z) { return p_x + (p_y - p_x) * p_z; }

	static inline int sgn(float p_x) { return p_x >= 0 ? 1 : -1; }

	static inline float abs(float p_x) { return ::abs(p_x); }

	static inline float DegToRad(float p_degrees) { return p_degrees * PI / 180; }
	static inline float RadToDeg(float p_radians) { return p_radians / PI * 180; }
			
	static vec2 GetVector(float angle)
	{
		return vec2(cos(angle), sin(angle));
	}
	
	static float floor(float value, float proj)
	{
		return std::floor(value / proj) * proj;
	}
	
	static float modulo(float value, float proj)
	{
		return value - floor(value, proj);
	}
	
	static inline float distance(float p_a, float p_b) { return abs(abs(p_a) - abs(p_b)); }
	
	static float closest_to(float value, float a, float b)
	{
		if (distance(value, a) < distance(value, b))
			return a;
		else
			return b;
	}
	
	static float clip(float value, float a, float b)
	{
		if (value < a)
			return a;
		else if (value > b)
			return b;
		
		return value;
	}

	static float get_barry_centric(const vec3& p_p1, const vec3& p_p2, const vec3& p_p3, const vec2& p_pos)
	{
		float det = (p_p2.z - p_p3.z) * (p_p1.x - p_p3.x) + (p_p3.x - p_p2.x) * (p_p1.z - p_p3.z);

		float l1 = ((p_p2.z - p_p3.z) * (p_pos.x - p_p3.x) + (p_p3.x - p_p2.x) * (p_pos.y - p_p3.z)) / det;
		float l2 = ((p_p3.z - p_p1.z) * (p_pos.x - p_p3.x) + (p_p1.x - p_p3.x) * (p_pos.y - p_p3.z)) / det;
		float l3 = 1.0f - l1 - l2;

		return l1 * p_p1.y + l2 * p_p2.y + l3 * p_p3.y;
	}
};