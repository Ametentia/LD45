#if !defined(LUDUM_MATHS_H_)
#define LUDUM_MATHS_H_

#include <math.h>

#define Abs(x) ((x) < 0 ? -(x) : (x))

inline f32 Sin(f32 angle) {
    f32 result = sinf(angle);
    return result;
}

inline f32 Cos(f32 angle) {
    f32 result = cosf(angle);
    return result;
}

inline v2 operator+(v2 a, v2 b) {
    v2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

inline v2 &operator+=(v2 &a, v2 b) {
    a = a + b;
    return a;
}

inline v2 operator-(v2 a, v2 b) {
    v2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return result;
}

inline v2 operator-(v2 a) {
    v2 result;
    result.x = -a.x;
    result.y = -a.y;

    return result;
}

inline v2 &operator-=(v2 &a, v2 b) {
    a = a - b;
    return a;
}

inline v2 operator*(v2 a, f32 b) {
    v2 result;
    result.x = b * a.x;
    result.y = b * a.y;

    return result;
}

inline v2 operator*(f32 a, v2 b) {
    v2 result = b * a;
    return result;
}

inline v2 &operator*=(v2 &a, f32 b) {
    a = a * b;
    return a;
}

#endif  // LUDUM_MATHS_H_
