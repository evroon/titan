#pragma once

#include "math/math.h"

#include "vec3.h"
#include "vec4.h"

#include "rect.h"

class mat4
{
public:
	mat4()
	{
		m[0] = 1.0; m[4] = 0.0; m[8] = 0.0; m[12] = 0.0;
		m[1] = 0.0; m[5] = 1.0; m[9] = 0.0; m[13] = 0.0;
		m[2] = 0.0; m[6] = 0.0; m[10] = 1.0; m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
	}
	mat4(float p_values[16])
	{
		for (int c = 0; c < 16; c++)
			m[c] = p_values[c];
	}
	mat4( float m0,  float m1,  float m2,  float m3,
		  float m4,  float m5,  float m6,  float m7,
		  float m8,  float m9,  float m10, float m11,
		  float m12, float m13, float m14, float m15)
	{
			m[0] = m0; m[4] = m4; m[8] = m8;  m[12] = m12;
			m[1] = m1; m[5] = m5; m[9] = m9;  m[13] = m13;
			m[2] = m2; m[6] = m6; m[10] = m10; m[14] = m14;
			m[3] = m3; m[7] = m7; m[11] = m11; m[15] = m15;
	}
	mat4(const vec4& p_c0, const vec4& p_c1, const vec4& p_c2, const vec4& p_c3)
	{
		m[0] = p_c0.x; m[4] = p_c1.x; m[8] = p_c2.x;  m[12] = p_c3.x;
		m[1] = p_c0.y; m[5] = p_c1.y; m[9] = p_c2.y;  m[13] = p_c3.y;
		m[2] = p_c0.z; m[6] = p_c1.z; m[10] = p_c2.z; m[14] = p_c3.z;
		m[3] = p_c0.w; m[7] = p_c1.w; m[11] = p_c2.w; m[15] = p_c3.w;
	}

	//Operators
    //Comparison
    bool operator==(const mat4 &r) const
    {
		for (int c = 0; c < 16; c++)
			if (m[c] != r.m[c])
				return false;
		return true;
    }
    bool operator!=(const mat4 &r) const
    {
		for (int c = 0; c < 16; c++)
			if (m[c] != r.m[c])
				return true;
		return false;
    }

	//Multiplication
    mat4 operator*(const mat4 &r) const
    {
        mat4 l = *this;
        l *= r;
        return l;
    }
    vec4 operator*(const vec4 &r) const
    {
		vec4 result;
		result.x = m[0] * r.x + m[4] * r.y + m[8] * r.z + m[12] * r.w;
		result.y = m[1] * r.x + m[5] * r.y + m[9] * r.z + m[13] * r.w;
		result.z = m[2] * r.x + m[6] * r.y + m[10] * r.z + m[14] * r.w;
		result.w = m[3] * r.x + m[7] * r.y + m[11] * r.z + m[15] * r.w;
		return result;
    }
    mat4 operator*(const float &r) const
    {
        mat4 l = *this;
        l *= r;
        return l;
    }
	mat4& operator*=(const mat4 &r)
	{
		mat4 l = *this;

		m[0]  = l.m[0] * r.m[0]  + l.m[4] * r.m[1]  + l.m[8]  * r.m[2]  + l.m[12] * r.m[3];
		m[1]  = l.m[1] * r.m[0]  + l.m[5] * r.m[1]  + l.m[9]  * r.m[2]  + l.m[13] * r.m[3];
		m[2]  = l.m[2] * r.m[0]  + l.m[6] * r.m[1]  + l.m[10] * r.m[2]  + l.m[14] * r.m[3];
		m[3]  = l.m[3] * r.m[0]  + l.m[7] * r.m[1]  + l.m[11] * r.m[2]  + l.m[15] * r.m[3];
		m[4]  = l.m[0] * r.m[4]  + l.m[4] * r.m[5]  + l.m[8]  * r.m[6]  + l.m[12] * r.m[7];
		m[5]  = l.m[1] * r.m[4]  + l.m[5] * r.m[5]  + l.m[9]  * r.m[6]  + l.m[13] * r.m[7];
		m[6]  = l.m[2] * r.m[4]  + l.m[6] * r.m[5]  + l.m[10] * r.m[6]  + l.m[14] * r.m[7];
		m[7]  = l.m[3] * r.m[4]  + l.m[7] * r.m[5]  + l.m[11] * r.m[6]  + l.m[15] * r.m[7];
		m[8]  = l.m[0] * r.m[8]  + l.m[4] * r.m[9]  + l.m[8]  * r.m[10] + l.m[12] * r.m[11];
		m[9]  = l.m[1] * r.m[8]  + l.m[5] * r.m[9]  + l.m[9]  * r.m[10] + l.m[13] * r.m[11];
		m[10] = l.m[2] * r.m[8]  + l.m[6] * r.m[9]  + l.m[10] * r.m[10] + l.m[14] * r.m[11];
		m[11] = l.m[3] * r.m[8]  + l.m[7] * r.m[9]  + l.m[11] * r.m[10] + l.m[15] * r.m[11];
		m[12] = l.m[0] * r.m[12] + l.m[4] * r.m[13] + l.m[8]  * r.m[14] + l.m[12] * r.m[15];
		m[13] = l.m[1] * r.m[12] + l.m[5] * r.m[13] + l.m[9]  * r.m[14] + l.m[13] * r.m[15];
		m[14] = l.m[2] * r.m[12] + l.m[6] * r.m[13] + l.m[10] * r.m[14] + l.m[14] * r.m[15];
		m[15] = l.m[3] * r.m[12] + l.m[7] * r.m[13] + l.m[11] * r.m[14] + l.m[15] * r.m[15];

		return *this;
	}
	mat4& operator*=(const float &r)
	{
		m[0] *= r; m[4] *= r; m[8] *= r; m[12] *= r;
		m[1] *= r; m[5] *= r; m[9] *= r; m[13] *= r;
		m[2] *= r; m[6] *= r; m[10] *= r; m[14] *= r;
		m[3] *= r; m[7] *= r; m[11] *= r; m[15] *= r;
		return *this;
	}
    //Addition
    mat4 operator+(const mat4 &r) const
    {
        mat4 l = *this;
        l += r;
        return l;
    }
    mat4 operator-(const mat4 &r) const
    {
        mat4 l = *this;
        l -= r;
        return l;
    }
	mat4& operator+=(const mat4 &r)
	{
		m[0]  += r.m[0];  m[1]  += r.m[1];	m[2]  += r.m[2];  m[3]  += r.m[3];
		m[4]  += r.m[4];  m[5]  += r.m[5];	m[6]  += r.m[6];  m[7]  += r.m[7];
		m[8]  += r.m[8];  m[9]  += r.m[9];	m[10] += r.m[10]; m[11] += r.m[11];
		m[12] += r.m[12]; m[13] += r.m[13]; m[14] += r.m[14]; m[15] += r.m[15];
		return *this;
	}
	mat4& operator-=(const mat4 &r)
	{
		m[0]  -= r.m[0];  m[1]  -= r.m[1];	m[2]  -= r.m[2];  m[3]  -= r.m[3];
		m[4]  -= r.m[4];  m[5]  -= r.m[5];	m[6]  -= r.m[6];  m[7]  -= r.m[7];
		m[8]  -= r.m[8];  m[9]  -= r.m[9];	m[10] -= r.m[10]; m[11] -= r.m[11];
		m[12] -= r.m[12]; m[13] -= r.m[13]; m[14] -= r.m[14]; m[15] -= r.m[15];
		return *this;
	}

	//Conversion
	operator float*() { return m; }
	String ToString() const
	{
		String s = "{ ";
		for (int c = 0; c < 16; c++)
			s += std::to_string(m[c]) + ", ";

		s += " }";
		return s;
	}

	//Info
	float det() const
	{
		return ((m[0] * m[5] * m[10]) +
				(m[4] * m[9] * m[2])  +
				(m[8] * m[1] * m[6])  -
				(m[8] * m[5] * m[2])  -
				(m[4] * m[1] * m[10]) -
				(m[0] * m[9] * m[6]));
	}

    //Default matrices
    void zeros()
    {
        for (int c = 0; c < 16; c++)
            m[c] = 0.0;
    }
    void identity()
    {
        *this = mat4();
    }

	//Transformations
    void translate(const vec3 &trans)
    {
        m[12] = trans.x;
		m[13] = trans.y;
		m[14] = trans.z;
    }

	void rotate(float x, float y, float z, float angle)
	{
		rotate(vec3(x, y, z), angle);
	}
	void rotate(const vec3 &axis, float angle)
	{
		//Math::DegToRad(angle);
		// float c = cosf(angle);
		// float s = sinf(angle);
		vec3 v = axis;
		v.normalize();
		/*float xx = v.x * v.x;
		float yy = v.y * v.y;
		float zz = v.z * v.z;
		float xy = v.x * v.y;
		float yz = v.y * v.z;
		float zx = v.z * v.x;
		float xs = v.x * s;
		float ys = v.y * s;
		float zs = v.z * s;
		m[0] = (1.0f - c) * xx + c;  m[4] = (1.0f - c) * xy - zs; m[8] = (1.0f - c) * zx + ys; m[12] = 0.0;
		m[1] = (1.0f - c) * xy + zs; m[5] = (1.0f - c) * yy + c;  m[9] = (1.0f - c) * yz - xs; m[13] = 0.0;
		m[2] = (1.0f - c) * zx - ys; m[6] = (1.0f - c) * yz + xs; m[10] = (1.0f - c) * zz + c; m[14] = 0.0;
		m[3] = 0.0;                  m[7] = 0.0;                  m[11] = 0.0;                 m[15] = 1.0;*/

	}
	void rotate_x(float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		m[0] = 1.0; m[4] = 0.0; m[8] = 0.0; m[12] = 0.0;
		m[1] = 0.0; m[5] = c;  m[9] = -s;  m[13] = 0.0;
		m[2] = 0.0; m[6] = s;  m[10] = c;  m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
	}
	void rotate_y(float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		m[0] = c;  m[4] = 0.0; m[8] = s;  m[12] = 0.0;
		m[1] = 0.0; m[5] = 1.0; m[9] = 0.0; m[13] = 0.0;
		m[2] = -s;  m[6] = 0.0; m[10] = c;  m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
	}
	void rotate_z(float angle)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		m[0] = c;	m[4] = -s;	m[8]  = 0.0; m[12] = 0.0;
		m[1] = s;	m[5] = c;	m[9]  = 0.0; m[13] = 0.0;
		m[2] = 0.0; m[6] = 0.0; m[10] = 1.0; m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
	}
	void scale(float x, float y, float z)
	{
		m[0] = x;   m[4] = 0.0; m[8]  = 0.0; m[12] = 0.0;
		m[1] = 0.0; m[5] = y;   m[9]  = 0.0; m[13] = 0.0;
		m[2] = 0.0; m[6] = 0.0; m[10] = z;   m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
	}
	void scale(const vec3 &v)
	{
		m[0] = v.x; m[4] = 0.0; m[8] = 0.0;  m[12] = 0.0;
		m[1] = 0.0; m[5] = v.y; m[9] = 0.0;  m[13] = 0.0;
		m[2] = 0.0; m[6] = 0.0; m[10] = v.z; m[14] = 0.0;
		m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
	}
	mat4 inverse() const
	{
		float inv[16], det;
		int i;

		inv[0] =	m[5] * m[10] * m[15] -
					m[5] * m[11] * m[14] -
					m[9] * m[6] * m[15] +
					m[9] * m[7] * m[14] +
					m[13] * m[6] * m[11] -
					m[13] * m[7] * m[10];

		inv[4] = -m[4] * m[10] * m[15] +
			m[4] * m[11] * m[14] +
			m[8] * m[6] * m[15] -
			m[8] * m[7] * m[14] -
			m[12] * m[6] * m[11] +
			m[12] * m[7] * m[10];

		inv[8] = m[4] * m[9] * m[15] -
			m[4] * m[11] * m[13] -
			m[8] * m[5] * m[15] +
			m[8] * m[7] * m[13] +
			m[12] * m[5] * m[11] -
			m[12] * m[7] * m[9];

		inv[12] = -m[4] * m[9] * m[14] +
			m[4] * m[10] * m[13] +
			m[8] * m[5] * m[14] -
			m[8] * m[6] * m[13] -
			m[12] * m[5] * m[10] +
			m[12] * m[6] * m[9];

		inv[1] = -m[1] * m[10] * m[15] +
			m[1] * m[11] * m[14] +
			m[9] * m[2] * m[15] -
			m[9] * m[3] * m[14] -
			m[13] * m[2] * m[11] +
			m[13] * m[3] * m[10];

		inv[5] = m[0] * m[10] * m[15] -
			m[0] * m[11] * m[14] -
			m[8] * m[2] * m[15] +
			m[8] * m[3] * m[14] +
			m[12] * m[2] * m[11] -
			m[12] * m[3] * m[10];

		inv[9] = -m[0] * m[9] * m[15] +
			m[0] * m[11] * m[13] +
			m[8] * m[1] * m[15] -
			m[8] * m[3] * m[13] -
			m[12] * m[1] * m[11] +
			m[12] * m[3] * m[9];

		inv[13] = m[0] * m[9] * m[14] -
			m[0] * m[10] * m[13] -
			m[8] * m[1] * m[14] +
			m[8] * m[2] * m[13] +
			m[12] * m[1] * m[10] -
			m[12] * m[2] * m[9];

		inv[2] = m[1] * m[6] * m[15] -
			m[1] * m[7] * m[14] -
			m[5] * m[2] * m[15] +
			m[5] * m[3] * m[14] +
			m[13] * m[2] * m[7] -
			m[13] * m[3] * m[6];

		inv[6] = -m[0] * m[6] * m[15] +
			m[0] * m[7] * m[14] +
			m[4] * m[2] * m[15] -
			m[4] * m[3] * m[14] -
			m[12] * m[2] * m[7] +
			m[12] * m[3] * m[6];

		inv[10] = m[0] * m[5] * m[15] -
			m[0] * m[7] * m[13] -
			m[4] * m[1] * m[15] +
			m[4] * m[3] * m[13] +
			m[12] * m[1] * m[7] -
			m[12] * m[3] * m[5];

		inv[14] = -m[0] * m[5] * m[14] +
			m[0] * m[6] * m[13] +
			m[4] * m[1] * m[14] -
			m[4] * m[2] * m[13] -
			m[12] * m[1] * m[6] +
			m[12] * m[2] * m[5];

		inv[3] = -m[1] * m[6] * m[11] +
			m[1] * m[7] * m[10] +
			m[5] * m[2] * m[11] -
			m[5] * m[3] * m[10] -
			m[9] * m[2] * m[7] +
			m[9] * m[3] * m[6];

		inv[7] = m[0] * m[6] * m[11] -
			m[0] * m[7] * m[10] -
			m[4] * m[2] * m[11] +
			m[4] * m[3] * m[10] +
			m[8] * m[2] * m[7] -
			m[8] * m[3] * m[6];

		inv[11] = -m[0] * m[5] * m[11] +
			m[0] * m[7] * m[9] +
			m[4] * m[1] * m[11] -
			m[4] * m[3] * m[9] -
			m[8] * m[1] * m[7] +
			m[8] * m[3] * m[5];

		inv[15] = m[0] * m[5] * m[10] -
			m[0] * m[6] * m[9] -
			m[4] * m[1] * m[10] +
			m[4] * m[2] * m[9] +
			m[8] * m[1] * m[6] -
			m[8] * m[2] * m[5];

		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (det == 0)
			return mat4();

		det = 1.0f / det;
		float invOut[16];

		for (i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;

		return invOut;
	}

	//Perspective matrices
	void perspective(const rect2& p_window, float fov, float aspect, float p_near, float p_far);
	void orthographic(const rect2& p_window, float p_near, float p_far);

	//view matrices
	void look_at(const vec3& p_eye, const vec3& p_target, const vec3& p_up);

	//Static Contructors
	static mat4 Scale(const vec3 &v)
	{
		mat4 result;
		result.scale(v);
		return result;
	}
	static mat4 Translate(const vec3 &v)
	{
		mat4 result;
		result.translate(v);
		return result;
	}
	static mat4 Rotate(const vec3 &v)
	{
		mat4 result;
		//result.rotate(v);
		return result;
	}

	//Data
	float m[16];
};