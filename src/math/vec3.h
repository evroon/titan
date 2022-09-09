#pragma once

#include "core/string.h"
#include "vec2.h"

template <class T>
class Vec3 {
   public:
    Vec3() { x = y = z = T(0); }
    Vec3(T a) { x = y = z = a; }
    Vec3(T _x, T _y, T _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    Vec3(Vec2<T> xy, T _z) {
        x = xy.x;
        y = xy.y;
        z = _z;
    }
    Vec3(T _x, Vec2<T> yz) {
        x = _x;
        y = yz.x;
        z = yz.y;
    }

    // General operators
    bool operator==(const Vec3& r) const { return x == r.x && y == r.y && z == r.z; }
    bool operator!=(const Vec3& r) const { return x != r.x || y != r.y || z != r.z; }

    // Vec3 operators
    Vec3 operator+(const Vec3& r) const { return Vec3(x + r.x, y + r.y, z + r.z); }
    Vec3 operator-(const Vec3& r) const { return Vec3(x - r.x, y - r.y, z - r.z); }
    Vec3 operator*(const Vec3& r) const { return Vec3(x * r.x, y * r.y, z * r.z); }
    Vec3 operator/(const Vec3& r) const { return Vec3(x / r.x, y / r.y, z / r.z); }

    Vec3& operator+=(const Vec3& r) {
        x += r.x;
        y += r.y;
        z += r.z;
        return *this;
    }
    Vec3& operator-=(const Vec3& r) {
        x -= r.x;
        y -= r.y;
        z -= r.z;
        return *this;
    }
    Vec3& operator*=(const Vec3& r) {
        x *= r.x;
        y *= r.y;
        z *= r.z;
        return *this;
    }
    Vec3& operator/=(const Vec3& r) {
        x /= r.x;
        y /= r.y;
        z /= r.z;
        return *this;
    }

    // Scalar operators
    Vec3 operator+(const T& r) const { return Vec3(x + r, y + r, z + r); }
    Vec3 operator-(const T& r) const { return Vec3(x - r, y - r, z - r); }
    Vec3 operator*(const T& r) const { return Vec3(x * r, y * r, z * r); }
    Vec3 operator/(const T& r) const { return Vec3(x / r, y / r, z / r); }

    Vec3& operator+=(const T& r) {
        x += r;
        y += r;
        z += r;
        return *this;
    }
    Vec3& operator-=(const T& r) {
        x -= r;
        y -= r;
        z -= r;
        return *this;
    }
    Vec3& operator*=(const T& r) {
        x *= r;
        y *= r;
        z *= r;
        return *this;
    }
    Vec3& operator/=(const T& r) {
        x /= r;
        y /= r;
        z /= r;
        return *this;
    }

    // Index operator
    T& operator[](const int& i) { return v[i]; }

    // Conversion
    String to_string() const {
        return "{ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) +
               " }";
    }

    // Get info
    T length() const { return sqrtf(x * x + y * y + z * z); }
    T angle(const Vec3<T>& v) const {
        T angle = fabs(acos(dot(v) / (length() * v.length())));
        return angle;
    }
    T dot(const Vec3<T>& v) const {
        T result = x * v.x + y * v.y + z * v.z;
        return result;
    }
    Vec3 cross(const Vec3<T>& r) const {
        Vec3<T> result;
        result.x = y * r.z - z * r.y;
        result.y = z * r.x - x * r.z;
        result.z = x * r.y - y * r.x;
        return result;
    }

    // properties
    const T& get_x() const { return x; }
    const T& get_y() const { return y; }
    const T& get_z() const { return z; }

    void set_x(const T& p_x) { x = p_x; }
    void set_y(const T& p_y) { y = p_y; }
    void set_z(const T& p_z) { z = p_z; }

    Vec2<T> get_xy() const { return {x, y}; }
    Vec2<T> get_yz() const { return {y, z}; }
    Vec2<T> get_xz() const { return {x, z}; }

    // Set
    Vec3& normalize() {
        T inv = 1.0f / length();
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    // Components
    union {
        struct {
            T x, y, z;
        };
        struct {
            T r, g, b;
        };
        T v[3];
    };
};

typedef Vec3<double> vec3d;
typedef Vec3<float> vec3;
typedef Vec3<int> vec3i;
typedef Vec3<char> vec3b;
