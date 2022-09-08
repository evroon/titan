#pragma once

template <typename T>
class Ellipse2 {
    Ellipse2(Vec2<T> _pos, Vec2<T> _size) {
        pos = _pos;
        size = _size;
    }

    T GetArea() { return PI * size.x * size.y; }

   private:
    Vec2<T> pos;
    Vec2<T> size;
};

typedef Ellipse2<double> ellipse2;
