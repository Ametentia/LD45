#if !defined(LUDUM_TYPES_H_)
#define LUDUM_TYPES_H_

#include <stdint.h>
#include <float.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uintptr_t umm;
typedef intptr_t  smm;

typedef float  f32;
typedef double f64;

typedef sfVector2f v2;
typedef sfVector3f v3;

inline v2 V2(f32 x, f32 y) {
    v2 result = { x, y };
    return result;
}

inline v3 V3(f32 x, f32 y, f32 z) {
    v3 result = { x, y, z };
    return result;
}

#define global static
#define internal static
#define local static

#define ArrayCount(x) (sizeof(x) / sizeof((x)[0]))
#define Assert(exp) do { if (!(exp)) { printf("[Assertion] %s (%s:%d)\n", #exp, __FILE__, __LINE__); asm("int3"); } } while (0)
#define OffsetOf(type, member) (umm) (&(((type *) 0)->member))
#define Swap(a, b) { auto __temp = a; a = b; b = __temp; }
#define cast

#define PI32 3.1415926535897f
#define Degrees(rad) ((rad) * (180.0f / PI32))
#define Radians(deg) ((deg) * (PI32 / 180.0f))

#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))


#endif  // LUDUM_TYPES_H_
