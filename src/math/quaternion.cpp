#include "quaternion.h"

#include "core/tmessage.h"
#include "mat4.h"
#include "math/math.h"

Quaternion::Quaternion() {
    x = y = z = 0.0f;
    w = 1.0f;
}

Quaternion::Quaternion(float p_x, float p_y, float p_z, float p_w) {
    x = p_x;
    y = p_y;
    z = p_z;
    w = p_w;
}

Quaternion::Quaternion(const vec3& p_axis, float p_angle) {
    float omega, s;

    omega = 0.5f * p_angle;
    s = sin(omega);

    x = s * x;
    y = s * y;
    z = s * z;
    w = cosf(omega);

    normalize();
}

Quaternion::Quaternion(const vec3& p_angles) {
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

Quaternion::~Quaternion() {}

void Quaternion::slerp(const Quaternion& a, const Quaternion& b, float t) {
    float omega, cosom, sinom, sclp, sclq;

    cosom = a.x * b.x + a.y * b.y + a.y * b.y + a.w * b.w;

    if (1.0f + cosom > 0) {
        if ((1.0f - cosom) > 0) {
            omega = acosf(cosom);
            sinom = sin(omega);
            sclp = sin((1.0f - t) * omega) / sinom;
            sclq = sin(t * omega) / sinom;
        } else {
            sclp = 1.0f - t;
            sclq = t;
        }

        x = sclp * a.x + sclq * b.x;
        y = sclp * a.y + sclq * b.y;
        y = sclp * a.y + sclq * b.y;
        w = sclp * a.w + sclq * b.w;
    } else {
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

void Quaternion::rotate_to(const vec3& p_start, const vec3& p_destination) {
    float l1 = p_start.length() * p_start.length();
    float l2 = p_destination.length() * p_destination.length();
    float norm_u_norm_v = sqrt(l1 * l2);
    float cos_theta = p_start.dot(p_destination) / norm_u_norm_v;
    float half_cos = sqrt(0.5f * (1.0f + cos_theta));
    vec3 v = p_start.cross(p_destination) / (norm_u_norm_v * 2.0f * half_cos);
    x = v.x;
    y = v.y;
    z = v.z;
    w = -half_cos;
    normalize();
}

vec3 Quaternion::get_euler() const {
    vec3 euler;
    // roll (x-axis rotation)
    float sinr = +2.0 * (w * x + y * z);
    float cosr = +1.0 - 2.0 * (x * x + y * y);
    euler.x = atan2(sinr, cosr);

    // pitch (y-axis rotation)
    float sinp = +2.0 * (w * y - z * x);
    if (fabs(sinp) >= 1)
        euler.y = copysign(PI / 2, sinp);  // use 90 degrees if out of range
    else
        euler.y = asin(sinp);

    // yaw (z-axis rotation)
    float siny = +2.0 * (w * z + x * y);
    float cosy = +1.0 - 2.0 * (y * y + z * z);
    euler.z = atan2(siny, cosy);
    return euler;
}

float Quaternion::get_norm() const { return Math::sqrt(x * x + y * y + z * z + w * w); }

void Quaternion::normalize() {
    float length = get_norm();

    x /= length;
    y /= length;
    z /= length;
    w /= length;
}

mat4 Quaternion::get_matrix() const {
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

vec3 Quaternion::get_complex() const { return vec3(x, y, z); }

float Quaternion::get_real() const { return w; }

vec3 Quaternion::get_axis() const { return vec3(x, y, z) / get_norm(); }

bool Quaternion::is_identity() const { return x == 0 && y == 0 && z == 0 && w == 1; }

Quaternion Quaternion::inverse() const {
    Quaternion q;
    float norm = get_norm();
    q.x = -x / norm;
    q.y = -y / norm;
    q.z = -z / norm;
    q.w = w / norm;
    return q;
}

vec3 Quaternion::rotate(const vec3& p_in) const {
    float num = x * 2.0f;
    float num2 = y * 2.0f;
    float num3 = z * 2.0f;
    float num4 = x * num;
    float num5 = y * num2;
    float num6 = z * num3;
    float num7 = x * num2;
    float num8 = x * num3;
    float num9 = y * num3;
    float num10 = w * num;
    float num11 = w * num2;
    float num12 = w * num3;

    vec3 result;
    result.x = (1.0f - (num5 + num6)) * p_in.x + (num7 - num12) * p_in.y + (num8 + num11) * p_in.z;
    result.y = (num7 + num12) * p_in.x + (1.0f - (num4 + num6)) * p_in.y + (num9 - num10) * p_in.z;
    result.z = (num8 - num11) * p_in.x + (num9 + num10) * p_in.y + (1.0f - (num4 + num5)) * p_in.z;
    return result;
}

void Quaternion::rotate_up(float p_angle) {
    vec3 up = vec3(0, 0, 1);
    vec3 right = get_axis().cross(up);
    quat q = quat(right, p_angle);
    *this = *this * q;
}

void Quaternion::rotate_right(float p_angle) {}

void Quaternion::rotate_around_axis(float p_angle) {}

Quaternion Quaternion::product(const Quaternion& rhs) const {
    return Quaternion(y * rhs.z - z * rhs.y + x * rhs.w + w * rhs.x,
                      z * rhs.x - x * rhs.z + y * rhs.w + w * rhs.y,
                      x * rhs.y - y * rhs.x + z * rhs.w + w * rhs.z,
                      w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
}

vec3 Quaternion::operator*(const vec3& rhs) const { return rotate(rhs); }

Quaternion Quaternion::operator*(const Quaternion& rhs) const { return product(rhs); }

Quaternion Quaternion::operator*(float rhs) const {
    return Quaternion(get_complex() * rhs, get_real() * rhs);
}

Quaternion Quaternion::operator+(const Quaternion& rhs) const {
    return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

Quaternion Quaternion::operator-(const Quaternion& rhs) const {
    return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

Quaternion Quaternion::operator-() const { return Quaternion(-x, -y, -z, -w); }

Quaternion Quaternion::operator/(float s) const {
    if (s == 0) T_ERROR("Dividing quaternion by 0");

    return Quaternion(get_complex() / s, get_real() / s);
}

bool Quaternion::operator==(const Quaternion& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

bool Quaternion::operator!=(const Quaternion& rhs) const {
    return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
}

Quaternion::operator mat4() const { return get_matrix(); }

String Quaternion::to_string() const {
    String s = "{ " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) +
               ", " + std::to_string(w) + " }";
    return s;
}
