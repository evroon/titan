#pragma once

#include <math.h>
#include <string>
#include "core/String.h"

template<class T> class Vec2
{
public:
	Vec2() { x = y = (T) 0.0f; }
	Vec2(T a) { x = y = a; }
	Vec2(T _x, T _y) { x = _x; y = _y; }
    
    //General operators
    bool operator==(const Vec2 &r) const { return x == r.x && y == r.y; }
    bool operator!=(const Vec2 &r) const { return x != r.x || y != r.y; }

	//Vec2 operators
	Vec2 operator+(const Vec2 &r) const { return Vec2(x + r.x, y + r.y); }
	Vec2 operator-(const Vec2 &r) const { return Vec2(x - r.x, y - r.y); }
	Vec2 operator*(const Vec2 &r) const { return Vec2(x * r.x, y * r.y); }
	Vec2 operator/(const Vec2 &r) const { return Vec2(x / r.x, y / r.y); }

	Vec2& operator+=(const Vec2 &r) { x += r.x; y += r.y; return *this; }
	Vec2& operator-=(const Vec2 &r) { x -= r.x; y -= r.y; return *this; }
	Vec2& operator*=(const Vec2 &r) { x *= r.x; y *= r.y; return *this; }
	Vec2& operator/=(const Vec2 &r) { x /= r.x; y /= r.y; return *this; }

	//Scalar operators
	Vec2 operator+(const T &r) const { return Vec2(x + r, y + r); }
	Vec2 operator-(const T &r) const { return Vec2(x - r, y - r); }
	Vec2 operator*(const T &r) const { return Vec2(x * r, y * r); }
	Vec2 operator/(const T &r) const { return Vec2(x / r, y / r); }

	Vec2& operator+=(const T &r) { x += r; y += r; return *this; }
	Vec2& operator-=(const T &r) { x -= r; y -= r; return *this; }
	Vec2& operator*=(const T &r) { x *= r; y *= r; return *this; }
	Vec2& operator/=(const T &r) { x /= r; y /= r; return *this; }

	//Index operator
	T& operator[](const int &i) { return v[i]; }
	operator String() const { return to_string(); }

	//Conversion
	String to_string() const
	{
		return "{ " + (std::string) std::to_string(x) + ", " + (std::string) std::to_string(y) + " }";
	}
    
    //Get info
    T length() const { return sqrt(x * x + y * y); }
	T angle(const Vec2 &v) const
	{
		T angle = fabs(acos(dot(v) / (length() * v.length())));
		return angle;
    }
    T dot(const Vec2<T> &v) const
    {
        T result = x * v.x + y * v.y;
        return result;
    }

	//properties
	T& get_x() { return x; }
	T& get_y() { return y; }

	void set_x(const T &p_x) { x = p_x; }
	void set_y(const T &p_y) { y = p_y; }
    
    //Set
    Vec2<T> normalize()
    {
		Vec2<T> result = *this;
        T inv = 1.0f / length();
		result.x *= inv;
		result.y *= inv;
		return result;
    }
    
	//Components
	union
	{
		struct { T x, y; };
		T v[2];
	};
};

typedef Vec2<double> vec2d;
typedef Vec2<float> vec2;
typedef Vec2<int> vec2i;
typedef Vec2<int> vec2ui;
typedef Vec2<char> vec2b;