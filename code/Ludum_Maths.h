#if !defined(LUDUM_MATHS_H_)
#define LUDUM_MATHS_H_

#include <math.h>

#define Abs(x) ((x) < 0 ? -(x) : (x))
#define Lerp(start, end, a) ((1 - a) * (start) + (a) * (end))

inline f32 Sin(f32 angle) {
    f32 result = sinf(angle);
    return result;
}

inline f32 Cos(f32 angle) {
    f32 result = cosf(angle);
    return result;
}

inline v2 Perp(v2 a) {
    v2 result = { -a.y, a.x };
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

inline f32 RandomUnilateral() {
    f32 result = cast(f32) rand() / cast(f32) RAND_MAX;
    return result;
}

inline f32 RandomBilateral() {
    f32 result = -1.0f + (2.0f * RandomUnilateral());
    return result;
}

inline u32 RandomChoice(u32 choices) {
    u32 result = (rand() % choices);
    return result;
}

inline u32 RandomBetween(u32 min, u32 max) {
    u32 result = min + cast(u32) (RandomUnilateral() * (max - min));
    return result;
}

inline s32 RandomBetween(s32 min, s32 max) {
    s32 result = min + cast(s32) (RandomUnilateral() * (max - min));
    return result;
}

inline f32 RandomBetween(f32 min, f32 max) {
    f32 result = min + (RandomUnilateral() * (max - min));
    return result;
}

#endif  // LUDUM_MATHS_H_
