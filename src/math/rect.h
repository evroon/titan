#pragma once

#include "core/array.h"
#include "vec2.h"

template <typename T>
class Rect2 {
   public:
    Rect2(T left, T right, T top, T bottom) {
        size.x = (right - left) / T(2);
        size.y = (top - bottom) / T(2);

        pos.x = left + size.x;
        pos.y = bottom + size.y;
    }
    Rect2(Vec2<T> _pos, Vec2<T> _size) {
        pos = _pos;
        size = _size;
    }
    Rect2(Vec2<T> p_size) : Rect2(Vec2<T>(), p_size) {}
    Rect2() : Rect2(Vec2<T>(), Vec2<T>(1)) {}

    // get area
    T get_area() const { return 4 * size.x * size.y; }

    // get edges
    T get_right() const { return pos.x + size.x; }
    T get_left() const { return pos.x - size.x; }
    T get_top() const { return pos.y + size.y; }
    T get_bottom() const { return pos.y - size.y; }

    // get vertices
    Vec2<T> get_upper_left() const { return Vec2<T>(get_left(), get_top()); }
    Vec2<T> get_upper_right() const { return Vec2<T>(get_right(), get_top()); }
    Vec2<T> get_bottom_left() const {
        return Vec2<T>(get_left(), get_bottom());
    }
    Vec2<T> get_bottom_right() const {
        return Vec2<T>(get_right(), get_bottom());
    }

    // mix
    T mix_horiz(double value) const {
        return get_left() + T(size.x * 2.0 * value);
    }
    T mix_vert(double value) const {
        return get_bottom() + T(size.y * 2.0 * value);
    }

    // check overlaps
    bool is_in_box(const Vec2<T>& pos) const {
        return pos.x >= get_left() && pos.x <= get_right() &&
               pos.y >= get_bottom() && pos.y <= get_top();
    }
    bool is_in_box(const Rect2<T>& area) const {
        return is_in_box(area.get_upper_left()) &&
               is_in_box(area.get_upper_right()) &&
               is_in_box(area.get_bottom_left()) &&
               is_in_box(area.get_bottom_right());
    }
    bool is_overlapping(const Rect2<T>& area) const {
        return is_in_box(area.get_upper_left()) ||
               is_in_box(area.get_upper_right()) ||
               is_in_box(area.get_bottom_left()) ||
               is_in_box(area.get_bottom_right()) || area.is_in_box(pos);
    }

    // crop
    Rect2<T> crop(const T& left, const T& right, const T& top,
                  const T& bottom) const {
        return Rect2<T>(get_left() + left, get_right() - right, get_top() - top,
                        get_bottom() + bottom);
    }
    Rect2<T> crop(const T& all) const { return crop(all, all, all, all); }

    // extend
    Rect2<T> extend(const T& left, const T& right, const T& top,
                    const T& bottom) const {
        return crop(-left, -right, -top, -bottom);
    }
    Rect2<T> extend(const T& all) const { return extend(all, all, all, all); }

    // align
    Rect2<T> align_left(const Vec2<T>& size) const {
        T left = get_left();
        T right = get_left() + size.x * T(2);
        T top = pos.y + size.y;
        T bottom = pos.y - size.y;

        return Rect2<T>(left, right, top, bottom);
    }
    Rect2<T> align_right(const Vec2<T>& size) const {
        T left = get_right() - size.x * T(2);
        T right = get_right();
        T top = pos.y + size.y;
        T bottom = pos.y - size.y;

        return Rect2<T>(left, right, top, bottom);
    }
    Rect2<T> align_top(const Vec2<T>& size) const {
        T left = pos.x - size.x;
        T right = pos.x + size.x;
        T top = get_top();
        T bottom = get_top() - size.y * T(2);

        return Rect2<T>(left, right, top, bottom);
    }
    Rect2<T> align_bottom(const Vec2<T>& size) const {
        T left = pos.x - size.x;
        T right = pos.x + size.x;
        T top = get_top() + size.y * T(2);
        T bottom = get_top();

        return Rect2<T>(left, right, top, bottom);
    }

    Rect2<T> align_full_left(const T& width) const {
        return align_left(Vec2<T>(width, size.y));
    }
    Rect2<T> align_full_right(const T& width) const {
        return align_right(Vec2<T>(width, size.y));
    }
    Rect2<T> align_full_top(const T& height) const {
        return align_top(Vec2<T>(size.x, height));
    }
    Rect2<T> align_full_bottom(const T& height) const {
        return align_bottom(Vec2<T>(size.x, height));
    }

    // split
    Array<Rect2<T>> horiz_split_2(const T& boundary) const {
        Array<Rect2<T>> r;

        r.push_back(
            rect2(get_left(), get_left() + boundary, get_top(), get_bottom()));
        r.push_back(
            rect2(get_left() + boundary, get_right(), get_top(), get_bottom()));

        return r;
    }
    Array<Rect2<T>> horiz_split_3(const T& boundary_1,
                                  const T& boundary_2) const {
        Array<Rect2<T>> r;

        r.push_back(rect2(get_left(), get_left() + boundary_1, get_top(),
                          get_bottom()));
        r.push_back(rect2(get_left() + boundary_1, get_left() + boundary_2,
                          get_top(), get_bottom()));
        r.push_back(rect2(get_left() + boundary_2, get_right(), get_top(),
                          get_bottom()));

        return r;
    }

    Array<Rect2<T>> default_horiz_split_2() const {
        return horiz_split_2(pos.x);
    }
    Array<Rect2<T>> default_horiz_split_3() const {
        return horiz_split_3(get_left() + size.x * 2 / 3,
                             get_left() + size.x * 4 / 3);
    }

    // conversions
    operator String() const {
        return "{ pos: " + pos.to_string() + ", size: " + size.to_string() +
               " }";
    }

    // getters and setters
    void set_pos(const vec2& p_pos) { pos = p_pos; }
    vec2 get_pos() const { return pos; }

    void set_size(const vec2& p_size) { size = p_size; }
    vec2 get_size() const { return size; }

    Vec2<T> pos;
    Vec2<T> size;
};

typedef Rect2<double> rect2d;
typedef Rect2<int> rect2i;
typedef Rect2<int> rect2u;
typedef Rect2<float> rect2;
