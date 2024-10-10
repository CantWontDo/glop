//
// Created by iamro on 10/5/2024.
//

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

typedef uint8_t u8;
typedef uint32_t u32;

typedef union v2
{
    struct
    {
        float x, y;
    };
    float v[2];
} v2;

static const v2 v2_uy = {1, 0};
static const v2 v2_ux = {0, 1};
static const v2 v2_zero = {0, 0};
static const v2 v2_one = {1, 1};

inline static v2 v2_add(v2 a, v2 b)
{
    return (v2){a.x + b.x, a.y + b.y};
}

inline static v2 v2_sub(v2 a, v2 b)
{
    return (v2){a.x - b.x, a.y - b.y};
}

inline static v2 v2_mul(v2 a, float scalar)
{
    return (v2){a.x * scalar, a.y * scalar};
}

inline static v2 v2_div(v2 a, float scalar)
{
    return (v2){a.x / scalar, a.y / scalar};
}

inline static float v2_dot(v2 a, v2 b)
{
    return a.x * b.x + a.y * b.y;
}

inline static float v2_len(v2 a)
{
    return sqrtf(v2_dot(a, a));
}

static void v2_print(v2 a)
{
    printf("[ %4.3f %4.3f ]\n", a.x, a.y);
}

inline static v2 v2_norm(v2 a)
{
    float len = v2_len(a);
    return (v2){a.x / len, a.y / len};
}

typedef union v3
{
    struct
    {
        float x, y, z;
    };
    float v[3];
} v3;

static const v3 v3_uy = {1, 0, 0};
static const v3 v3_ux = {0, 1, 0};
static const v3 v3_uz = {0, 0, 1};
static const v3 v3_zero = {0, 0, 0};
static const v3 v3_one = {1, 1, 1};

inline static v3 v3_add(v3 a, v3 b)
{
    return (v3){a.x + b.x, a.y + b.y, a.z + b.z};
}

inline static v3 v3_sub(v3 a, v3 b)
{
    return (v3){a.x - b.x, a.y - b.y, a.z - b.z};
}

inline static v3 v3_mul(v3 a, float scalar)
{
    return (v3){a.x * scalar, a.y * scalar, a.z * scalar};
}

inline static v3 v3_div(v3 a, float scalar)
{
    return (v3){a.x / scalar, a.y / scalar, a.z / scalar};
}

inline static float v3_dot(v3 a, v3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline static float v3_len(v3 a)
{
    return sqrtf(v3_dot(a, a));
}

inline static v3 v3_norm(v3 a)
{
    float len = v3_len(a);
    return (v3){a.x / len, a.y / len, a.z / len};
}

inline static v3 v3_cross(v3 a, v3 b)
{
    return (v3)
    {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

static void v3_print(v3 a)
{
    printf("[ %4.3f %4.3f %4.3f]\n", a.x, a.y, a.z);
}

typedef union v4
{
    struct
    {
        float x, y, z, w;
    };

    // for colors!
    struct
    {
        float r, g, b, a;
    };
    float v[4];
} v4;

static const v4 v4_uy = {1, 0, 0, 0};
static const v4 v4_ux = {0, 1, 0, 0};
static const v4 v4_uz = {0, 0, 1, 0};
static const v4 v4_uw = {0, 0, 0, 1};
static const v4 v4_zero = {0, 0, 0, 0};
static const v4 v4_one = {1, 1, 1, 1};

inline static v4 v4_add(v4 a, v4 b)
{
    return (v4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

inline static v4 v4_sub(v4 a, v4 b)
{
    return (v4){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

inline static v4 v4_mul(v4 a, float scalar)
{
    return (v4){a.x * scalar, a.y * scalar, a.z * scalar, a.w * scalar};
}

inline static v4 v4_div(v4 a, float scalar)
{
    return (v4){a.x / scalar, a.y / scalar, a.z / scalar, a.w / scalar};
}

inline static float v4_dot(v4 a, v4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline static float v4_len(v4 a)
{
    return sqrtf(v4_dot(a, a));
}

inline static v4 v4_norm(v4 a)
{
    float len = v4_len(a);
    return (v4){a.x / len, a.y / len, a.z / len, a.w / len};
}

static void v4_print(v4 a)
{
    printf("[ %4.3f %4.3f %4.3f %4.3f]\n", a.x, a.y, a.z, a.w);
}

typedef union m4
{
    struct
    {
        float
        _11, _12, _13, _14,
        _21, _22, _23, _24,
        _31, _32, _33, _34,
        _41, _42, _43, _44;
    };
    float m[16];
    float m_g[4][4];

    v4 r[4];
} m4;

static const m4 m4_ident =
    {1, 0, 0,0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
    };

static void m4_print(m4 m)
{
    v4_print(m.r[0]);
    v4_print(m.r[1]);
    v4_print(m.r[2]);
    v4_print(m.r[3]);
}

inline static m4 m4_mul_s(m4 m, float scalar)
{
    for (int i = 0; i < 16; i++)
        m.m[i] *= scalar;
    return m;
}

inline static m4 m4_div_s(m4 m, float scalar)
{
    for (int i = 0; i < 16; i++)
        m.m[i] /= scalar;
    return m;
}

inline static m4 m4_transpose(m4 m)
{
    return (m4)
    {
    m._11, m._21, m._31, m._41,
    m._12, m._22, m._32, m._42,
    m._13, m._23, m._33, m._43,
    m._14, m._24, m._34, m._44,
    };
}

inline static m4 m4_mul_m(m4 m, m4 m2)
{
    m4 out;
    m2 = m4_transpose(m2);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            out.m_g[i][j] = v4_dot(m.r[i], m2.r[j]);
    }
    return out;
}

inline static m4 m4_scale(float x, float y, float z)
{
    m4 out;
    out.r[0].x = x;
    out.r[1].y = y;
    out.r[2].z = z;
    return out;
}

inline static m4 m4_scale_u(float s)
{
    return m4_scale(s, s, s);
}

inline static m4 m4_scale_v(v3 a)
{
    return m4_scale(a.x, a.y, a.z);
}

inline static float m4_det(m4 m)
{
    return m._11 *
                (m._22*(m._33*m._44 - m._34*m._43) +
                m._23*(m._34*m._42 - m._32*m._44) +
                m._24*(m._32*m._43 - m._33*m._42)
                ) -
          m._12 *
                (m._21*(m._33*m._44 - m._34*m._43) +
                m._23*(m._34*m._41 - m._31*m._44) +
                m._24*(m._31*m._43 - m._33*m._41)
                ) +
          m._13 *
                (m._21*(m._32*m._44 - m._34*m._42) +
                m._22*(m._34*m._41 - m._31*m._44) +
                m._24*(m._31*m._42 - m._32*m._41)
                ) -
          m._14 *
                (m._21*(m._32*m._43 - m._33*m._42) +
                m._22*(m._33*m._41 - m._31*m._43) +
                m._23*(m._31*m._42 - m._32*m._41)
                );
}

inline static m4 m4_adj(m4 m)
{

}


inline static v4 v4_mul_m4(m4 m, v4 v)
{
    v4 out;

    out.x = v4_dot(v, m.r[0]);
    out.y = v4_dot(v, m.r[1]);
    out.z = v4_dot(v, m.r[2]);
    out.w = v4_dot(v, m.r[3]);

    return out;
}
#endif //TYPEDEFS_H
