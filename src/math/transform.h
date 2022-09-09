#pragma once

#include "core/vector.h"
#include "mat4.h"
#include "quaternion.h"
#include "vec2.h"

class Transform {
   public:
    Transform() : Transform(vec3(), vec3(1.0f), 0) {}

    Transform(const vec3& p) : Transform(p, vec3(1.0f), 0) {}
    Transform(const rect2& p_area) : Transform(p_area.pos, p_area.size, 0) {}

    Transform(const vec3& p, const vec3& s) : Transform(p, s, 0) {}
    Transform(const vec3& p, const vec2& s) : Transform(p, vec3(s, 0.0f), 0) {}
    Transform(const vec2& p, const vec3& s) : Transform(vec3(p, 0.0f), s, 0) {}
    Transform(const vec2& p, const vec2& s) : Transform(vec3(p, 0.0f), vec3(s, 1.0f), 0) {}

    Transform(const vec3& p, const vec3& s, const vec3& r);
    Transform(const vec3& p, const vec2& s, const vec3& r) : Transform(p, vec3(s, 0.0f), r) {}
    Transform(const vec2& p, const vec3& s, const vec3& r) : Transform(vec3(p, 0.0f), s, r) {}
    Transform(const vec2& p, const vec2& s, const vec3& r)
        : Transform(vec3(p, 0.0f), vec3(s, 1.0f), r) {}

    ~Transform();
    bool operator==(const Transform& r) const;
    bool operator!=(const Transform& r) const;
    operator String() const;
    String ToString() const;

    void update(const vec3& p_pos, const vec3& p_size, const vec3& p_rot);
    void update(const vec3& p_pos, const vec3& p_size);
    void update();

    // helper
    void update(const vec3& p_pos, const vec2& p_size);
    void update(const vec2& p_pos, const vec3& p_size);
    void update(const vec2& p_pos, const vec2& p_size);

    void set_pos2d(const vec2& p_pos);
    void set_pos(const vec3& p_pos);
    const vec3& get_pos() const;

    void set_size2d(const vec2& p_size);
    void set_size(const vec3& p_size);
    const vec3& get_size() const;

    void set_rotation(const vec3& p_rotation);
    vec3 get_rotation() const;

    const mat4& get_model() const;
    const quat& get_quat() const;

   private:
    vec3 pos, size;
    Quaternion rotation;
    mat4 model;
};
