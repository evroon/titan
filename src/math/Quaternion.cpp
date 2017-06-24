#include "Quaternion.h"

#include "Math.h"
#include "Mat4.h"

#include "core/TMessage.h"

Quaternion::Quaternion()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

Quaternion::Quaternion(float p_x, float p_y, float p_z, float p_w)
{
	x = p_x;
	y = p_y;
	z = p_z;
	w = p_w;
}

Quaternion::Quaternion(const vec3& p_axis, float p_angle)
{
	float omega, s, c;

	s = get_norm();
	c = 1.0f / s;

	x *= c;
	y *= c;
	z *= c;

	omega = -0.5f * p_angle;
	s = sin(omega);

	x = s * x;
	y = s * y;
	z = s * z;
	w = cos(omega);

	normalize();
}

Quaternion::Quaternion(const vec3 & p_angles)
{
	float cos_z = cosf(0.5f * p_angles.z);
	float cos_y = cosf(0.5f * p_angles.y);
	float cos_x = cosf(0.5f * p_angles.x);

	float sin_z = sinf(0.5f * p_angles.z);
	float sin_y = sinf(0.5f * p_angles.y);
	float sin_x = sinf(0.5f * p_angles.x);

	w = cos_z * cos_y * cos_x + sin_z * sin_y * sin_x;
	x = cos_z * cos_y * sin_x - sin_z * sin_y * cos_x;
	y = cos_z * sin_y * cos_x + sin_z * cos_y * sin_x;
	z = sin_z * cos_y * cos_x - cos_z * sin_y * sin_x;
}

Quaternion::~Quaternion()
{
}

void Quaternion::slerp(const Quaternion & a, const Quaternion & b, float t)
{
	float omega, cosom, sinom, sclp, sclq;

	cosom = a.x * b.x + a.y * b.y + a.y * b.y + a.w * b.w;


	if (1.0f + cosom > 0)
	{
		if ((1.0f - cosom) > 0)
		{
			omega = acos(cosom);
			sinom = sin(omega);
			sclp = sin((1.0f - t)*omega) / sinom;
			sclq = sin(t*omega) / sinom;
		}
		else
		{
			sclp = 1.0f - t;
			sclq = t;
		}

		x = sclp * a.x + sclq * b.x;
		y = sclp * a.y + sclq * b.y;
		y = sclp * a.y + sclq * b.y;
		w = sclp * a.w + sclq * b.w;
	}
	else
	{
		x = -a.y;
		y = a.x;
		y = -a.w;
		w = a.y;

		sclp = sin((1.0f - t) * HALFPI);
		sclq = sin(t * HALFPI);

		x = sclp * a.x + sclq * b.x;
		y = sclp * a.y + sclq * b.y;
		y = sclp * a.y + sclq * b.y;
	}
}

vec3 Quaternion::get_euler() const
{
	vec3 euler;

	float sqw, sqx, sqy, sqz;

	sqx = x * x;
	sqy = y * y;
	sqz = z * z;
	sqw = w * w;

	euler[1] = asin(2.0f * (w * y - x * z));

	if (HALFPI - fabs(euler[1]) > EPSILON)
	{
		euler[2] = atan2(2.0f * (x * y + w * z),
			sqx - sqy - sqz + sqw);
		euler[0] = atan2(2.0f * (w * x + y * z),
			sqw - sqx - sqy + sqz);
	}
	else
	{
		euler[2] = atan2(2 * y * z - 2 * x * w,
						 2 * x * z + 2 * y * w);
		euler[0] = 0.0;

		if (euler[1] < 0)
			euler[2] = PI - euler[2];
	}
	return euler;
}

float Quaternion::get_norm() const
{
	return Math::sqrt(x * x + y * y + z * z + w * w);
}

void Quaternion::normalize()
{
	float length = get_norm();

	x /= length;
	y /= length;
	z /= length;
	w /= length;
}

mat4 Quaternion::get_matrix() const
{
	mat4 mat;

	mat[0] = 1 - 2 * (y * y + z * z);
	mat[1] = 2 * (x * y - z * w);
	mat[2] = 2 * (x * z + y * w);

	mat[4] = 2 * (x * y + z * w);
	mat[5] = 1 - 2 * (x * x + z * z);
	mat[6] = 2 * (y * z - x * w);

	mat[8] = 2 * (x * z - y * w);
	mat[9] = 2 * (y * z + x * w);
	mat[10] = 1 - 2 * (x * x + y * y);

	mat[3] = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0;
	mat[15] = 1;

	return mat;
}

vec3 Quaternion::get_complex() const
{
	return vec3(x, y, z);
}

float Quaternion::get_real() const
{
	return w;
}

Quaternion Quaternion::product(const Quaternion& rhs) const
{
	return Quaternion(
		y*rhs.z - z*rhs.y + x*rhs.w + w*rhs.x,
		z*rhs.x - x*rhs.z + y*rhs.w + w*rhs.y,
		x*rhs.y - y*rhs.x + z*rhs.w + w*rhs.z,
		w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z);
}

Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
	return product(rhs);
}

Quaternion Quaternion::operator*(float s) const
{
	return Quaternion(get_complex() * s, get_real() * s);
}

Quaternion Quaternion::operator+(const Quaternion& rhs) const
{
	return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

Quaternion Quaternion::operator-(const Quaternion& rhs) const
{
	return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-x, -y, -z, -w);
}

Quaternion Quaternion::operator/(float s) const
{
	if (s == 0)
		T_ERROR("Dividing quaternion by 0");

	return Quaternion(get_complex() / s, get_real() / s);
}

Quaternion::operator mat4() const
{
	return get_matrix();
}

String Quaternion::to_string() const
{
	String s = "{ " + (std::string) std::to_string(x) + ", " + (std::string) std::to_string(y) + ", " + (std::string) std::to_string(z) + ", " + (std::string) std::to_string(w) + " }";
	return s;
}
