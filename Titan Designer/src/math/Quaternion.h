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

	void rotate_to(const vec3& p_start, const vec3& p_destination);

	//returns a vector containing roll pitch yaw
	vec3 get_euler() const;

	float get_norm() const;
	void normalize();

	mat4 get_matrix() const;

	vec3 get_complex() const;
	float get_real() const;
	vec3 get_axis() const;

	bool is_identity() const;

	Quaternion inverse() const;

	vec3 rotate(const vec3& p_in) const;

	void rotate_up(float p_angle);
	void rotate_right(float p_angle);
	void rotate_around_axis(float p_angle);


	//operators
	Quaternion product(const Quaternion & rhs) const;

	vec3 operator*(const vec3& rhs) const;
	Quaternion operator*(const Quaternion & rhs) const;
	Quaternion operator*(float rhs) const;
	Quaternion operator+(const Quaternion & rhs) const;
	Quaternion operator-(const Quaternion & rhs) const;
	Quaternion operator-() const;
	Quaternion operator/(float s) const;
	bool operator==(const Quaternion& rhs) const;
	bool operator!=(const Quaternion& rhs) const;

	operator mat4() const;

	String to_string() const;


private:
	float x, y, z, w;
};

typedef Quaternion quat;