#pragma once

#include "Vec3.h"

class mat4;

//sources used: https://cs.stanford.edu/~acoates/quaternion.h

class Quaternion
{
public:
	Quaternion();
	Quaternion(float p_x, float p_y, float p_z, float p_w);
	Quaternion(const vec3& p_axis, float p_angle);
	Quaternion(const vec3& p_angles);

	virtual ~Quaternion();

	void slerp(const Quaternion& a, const Quaternion& b, float t);

	//returns a vector containing roll pitch yaw
	vec3 get_euler() const;

	float get_norm() const;
	void normalize();

	mat4 get_matrix() const;

	vec3 get_complex() const;
	float get_real() const;

	//operators
	Quaternion product(const Quaternion & rhs) const;

	Quaternion operator*(const Quaternion & rhs) const;
	Quaternion operator*(float s) const;
	Quaternion operator+(const Quaternion & rhs) const;
	Quaternion operator-(const Quaternion & rhs) const;
	Quaternion operator-() const;
	Quaternion operator/(float s) const;

	operator mat4() const;

	String to_string() const;


private:
	float x, y, z, w;
};

typedef Quaternion quat;