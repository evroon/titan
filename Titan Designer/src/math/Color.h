#pragma once

#include "Vec3.h"
#include "Vec4.h"

#define T float
#define TO_RGB(X) Color::FromRGB(vec3i(X))

class Color
{
public:
	Color() { rgba = vec4(1.0f); }
	Color(vec4 val) { rgba = val; }
	Color(vec3 val) : Color(val.x, val.y, val.z, 1.0f) { }
	Color(T r, T g, T b) : Color(r, g, b, 1.0f) { }
	Color(T r, T g, T b, T a) { rgba = vec4(r, g, b, a); }

	void FromRGBA(vec4 val) { rgba = val / vec4(256.0f); }
	static Color FromRGBA(vec4i val) { return { val.x / 256.0f, val.y / 256.0f, val.z / 256.0f, val.w / 256.0f }; }

	void FromRGB(vec3 val) { rgba = { val.x / 256.0f, val.y / 256.0f, val.z / 256.0f, 1.0f }; }
	static Color FromRGB(vec3i val) { return { val.x / 256.0f, val.y / 256.0f, val.z / 256.0f, 1.0f }; }
	
	void from_hsv(vec3 val);
	static Color from_hsv(vec3i val);

	vec3 get_rgb() const { return vec3(r, g, b); }
	vec4 get_rgba() const { return vec4(r, g, b, a); }

	vec3 get_hsv() const;

	//(operators are exactly the same as Vec4)

	//General operators
	bool operator==(const Color &r) const { return x == r.x && y == r.y && z == r.z && w == r.w; }
	bool operator!=(const Color &r) const { return x != r.x || y != r.y || z != r.z || w != r.w; }

	//Vec4 operators
	Color operator+(const Color &r) const { return Color(x + r.x, y + r.y, z + r.z, w + r.w); }
	Color operator-(const Color &r) const { return Color(x - r.x, y - r.y, z - r.z, w - r.w); }
	Color operator*(const Color &r) const { return Color(x * r.x, y * r.y, z * r.z, w * r.w); }
	Color operator/(const Color &r) const { return Color(x / r.x, y / r.y, z / r.z, w / r.w); }

	Color& operator+=(const Color &r) { x += r.x; y += r.y; z += r.z; w += r.w; return *this; }
	Color& operator-=(const Color &r) { x -= r.x; y -= r.y; z -= r.z; w -= r.w; return *this; }
	Color& operator*=(const Color &r) { x *= r.x; y *= r.y; z *= r.z; w *= r.w; return *this; }
	Color& operator/=(const Color &r) { x /= r.x; y /= r.y; z /= r.z; w /= r.w; return *this; }

	//Scalar operators
	Color operator+(const T &r) const { return Color(x + r, y + r, z + r, w + r); }
	Color operator-(const T &r) const { return Color(x - r, y - r, z - r, w - r); }
	Color operator*(const T &r) const { return Color(x * r, y * r, z * r, w * r); }
	Color operator/(const T &r) const { return Color(x / r, y / r, z / r, w / r); }

	Color& operator+=(const T &r) { x += r; y += r; z += r; w += r; return *this; }
	Color& operator-=(const T &r) { x -= r; y -= r; z -= r; w -= r; return *this; }
	Color& operator*=(const T &r) { x *= r; y *= r; z *= r; w *= r; return *this; }
	Color& operator/=(const T &r) { x /= r; y /= r; z /= r; w /= r; return *this; }

	//Index operator
	T& operator[](const int &i) { return v[i]; }

	String toString() const;

	//Fundamental colors
	static Color White, Black, Red, Green, Blue, LightBlue, Grey, DarkGrey;

	//Components
	union
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		T v[4];
		vec4 rgba;
		// struct { vec3 rgb; float a; };
	};
};

#undef T