//
// Created by Rahul Pardeshi on 10/5/2024.
//

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "err.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float_t f32;
typedef double_t f64;

#define PI 3.1415926
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(x, upper, lower) (MIN((upper), MAX((x), (lower)))

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

    // for quaternions!
    struct
    {
        float w_q;
        v3 v_q;
    };
    float v[4];
} v4;

typedef v4 quat;
static const quat quat_ident = {.w_q = 1, .v_q = v3_zero};

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

typedef union m2
{
    struct
    {
        float
        _11, _12,
        _21, _22;
    };
    float m[4];
    float m_g[2][2];

    v2 r[2];
} m2 ;

static const m2 m2_ident =
    {1, 0,
     0, 1
    };

static void m2_print(m2 m)
{
    v2_print(m.r[0]);
    v2_print(m.r[1]);
}

inline static m2 m2_mul_s(m2 m, float scalar)
{
    for (int i = 0; i < 4; i++)
        m.m[i] *= scalar;
    return m;
}

inline static m2 m2_div_s(m2 m, float scalar)
{
    for (int i = 0; i < 4; i++)
        m.m[i] /= scalar;
    return m;
}

inline static m2 m2_transpose(m2 m)
{
    return (m2)
    {
    m._11, m._21,
    m._12, m._22,
    };
}

inline static m2 m2_mul_m(m2 m, m2 m_2)
{
    m2 out;
    m_2 = m2_transpose(m_2);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            out.m_g[i][j] = v2_dot(m.r[i], m_2.r[j]);
    }
    return out;
}

inline static float m2_det(m2 m)
{
    return m._11*m._22 - m._12*m._21;
}

inline static v2 v2_mul_m2(m2 m, v2 v)
{
    v2 out;
    out.x = v2_dot(v, m.r[0]);
    out.y = v2_dot(v, m.r[1]);

    return out;
}

typedef union m3
{
    struct
    {
        float
        _11, _12, _13,
        _21, _22, _23,
        _31, _32, _33;
    };
    float m[9];
    float m_g[3][3];

    v3 r[3];
} m3;

static const m3 m3_ident =
    {1, 0, 0,
     0, 1, 0,
    0, 0, 1
    };

static void m3_print(m3 m)
{
    v3_print(m.r[0]);
    v3_print(m.r[1]);
    v3_print(m.r[2]);
}

inline static m3 m3_mul_s(m3 m, float scalar)
{
    for (int i = 0; i < 9; i++)
        m.m[i] *= scalar;
    return m;
}

inline static m3 m3_div_s(m3 m, float scalar)
{
    for (int i = 0; i < 4; i++)
        m.m[i] /= scalar;
    return m;
}

inline static m3 m3_transpose(m3 m)
{
    return (m3)
    {
    m._11, m._21, m._31,
    m._12, m._22, m._32,
        m._13, m._23, m._33
    };
}

inline static m3 m3_mul_m(m3 m, m3 m2)
{
    m3 out;
    m2 = m3_transpose(m2);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            out.m_g[i][j] = v3_dot(m.r[i], m2.r[j]);
    }
    return out;
}

inline static float m3_det(m3 m)
{
    return m._11 * (m._22*m._33 - m._23*m._32) -
           m._12 * (m._21*m._33 - m._23*m._31) +
           m._13 * (m._21*m._32 - m._22*m._31);
}

inline static v3 v3_mul_m3(m3 m, v3 v)
{
    v3 out;
    out.x = v3_dot(v, m.r[0]);
    out.y = v3_dot(v, m.r[1]);
    out.z = v3_dot(v, m.r[2]);

    return out;
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
    m4 out = m4_ident;
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

inline static m4 m4_transform(float x, float y, float z)
{
    m4 out = m4_ident;
    out.r[3] = (v4){x, y, z, 1};
    return out;
}

inline static float deg_to_rad(float degrees)
{
    return degrees * PI / 180.0;
}

inline static float rad_to_deg(float radians)
{
    return radians * 180 / PI;
}

inline static m4 m4_rot_x(float radians)
{
    m4 out = m4_ident;

    out.r[1].y = cosf(radians);
    out.r[1].z = sinf(radians);

    out.r[2].y = -sinf(radians);
    out.r[2].y = cosf(radians);

    return out;
}

inline static m4 m4_rot_y(float radians)
{
    m4 out = m4_ident;

    out.r[0].x = cosf(radians);
    out.r[0].z = -sinf(radians);

    out.r[2].x = sinf(radians);
    out.r[2].z = cosf(radians);

    return out;
}

inline static m4 m4_rot_z(float radians)
{
    m4 out = m4_ident;

    out.r[0].x = cosf(radians);
    out.r[0].y = sinf(radians);

    out.r[1].x = -sinf(radians);
    out.r[1].y = cosf(radians);

    return out;
}

inline static m4 m4_transform_v(v3 v)
{
    return m4_transform(v.x, v.y, v.z);
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
    m3 cofac_11 = {m._22, m._23, m._24,
                    m._32, m._33, m._34,
                    m._42, m._43, m._44};
    m3 cofac_12 = {m._21, m._23, m._24,
                     m._31, m._33, m._34,
                        m._41, m._43, m._44
                    };
    m3 cofac_13 = {m._21, m._22, m._24,
                        m._31, m._32, m._34,
                            m._41, m._42, m._44};
    m3 cofac_14 = {m._21, m._22, m._23,
                        m._31, m._32, m._33,
                            m._41, m._42, m._43};

    m3 cofac_21 = {m._12, m._13, m._14,
                    m._32, m._33, m._34,
                    m._42, m._43, m._44};
    m3 cofac_22 = {m._11, m._13, m._14,
                     m._31, m._33, m._34,
                        m._41, m._43, m._44
                    };
    m3 cofac_23 = {m._11, m._12, m._14,
                        m._31, m._32, m._34,
                            m._41, m._42, m._44};
    m3 cofac_24 = {m._11, m._12, m._13,
                        m._31, m._32, m._33,
                            m._41, m._42, m._43};

    m3 cofac_31 = {m._12, m._13, m._14,
                    m._22, m._23, m._24,
                    m._42, m._43, m._44};
    m3 cofac_32 = {m._11, m._13, m._14,
                     m._21, m._23, m._24,
                        m._41, m._43, m._44
                    };
    m3 cofac_33 = {m._11, m._12, m._14,
                        m._21, m._22, m._24,
                            m._41, m._42, m._44};
    m3 cofac_34 = {m._11, m._12, m._13,
                        m._21, m._22, m._23,
                            m._41, m._42, m._43};

    m3 cofac_41 = {m._12, m._13, m._14,
                    m._22, m._23, m._24,
                    m._32, m._33, m._34};
    m3 cofac_42 = {m._11, m._13, m._14,
                     m._21, m._23, m._24,
                        m._31, m._33, m._34
                    };
    m3 cofac_43 = {m._11, m._12, m._14,
                        m._21, m._22, m._24,
                            m._31, m._32, m._34};
    m3 cofac_44 = {m._11, m._12, m._13,
                        m._21, m._22, m._23,
                            m._31, m._32, m._33};

    m4 adj_bt =
        {
    m3_det(cofac_11), -m3_det(cofac_12), m3_det(cofac_13), -m3_det(cofac_14),
    -m3_det(cofac_21), m3_det(cofac_22), -m3_det(cofac_23), m3_det(cofac_24),
    m3_det(cofac_31), -m3_det(cofac_32), m3_det(cofac_33), -m3_det(cofac_34),
    -m3_det(cofac_41), m3_det(cofac_42), -m3_det(cofac_43), m3_det(cofac_44)
        };

    return m4_transpose(adj_bt);
}

inline static m4 m4_inv(m4 m)
{
    float det = m4_det(m);
    if (det == 0)
        log_err("Matrix cannot be inverted!");
    return m4_div_s(m4_adj(m), det);
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

inline static quat quat_conjugate(quat q)
{
    quat out = q;
    out.v_q = v3_mul(out.v_q, -1);
    return out;
}

inline static quat quat_mul(quat q1, quat q2)
{
   quat out = quat_ident;
   out.w_q = q1.w_q*q2.w_q - v3_dot(q1.v_q, q2.v_q);
   out.v_q = v3_add(v3_add(v3_mul(q2.v_q, q1.w_q), v3_mul(q1.v_q, q2.w_q)),
        v3_cross(q1.v_q, q2.v_q));

   return out;
}

inline static quat quat_from_euler_rad(float pitch, float heading, float bank)
{
    quat out = quat_ident;

    float h_cos = cosf(heading / 2);
    float h_sin = sinf(heading / 2);

    float p_cos = cosf(pitch / 2);
    float p_sin = sinf(pitch / 2);

    float b_cos = cosf(bank / 2);
    float b_sin = sinf(bank / 2);

    out.x = h_cos*p_cos*b_cos + h_sin*p_sin*b_sin;
    out.y = -h_cos*p_sin*b_cos - h_sin*p_cos*b_sin;
    out.z = h_cos*p_sin*b_sin - h_sin*p_cos*b_cos;
    out.w = h_sin*p_sin*b_cos - h_cos*p_cos*b_sin;

    return out;
}
inline static quat quat_from_euler(float pitch, float heading, float bank)
{
    return quat_from_euler_rad(deg_to_rad(pitch), deg_to_rad(heading), deg_to_rad(bank));
}

inline static quat quat_slerp(quat q1, quat q2, float t)
{
    quat out;

    float cos_omega = v4_dot(q1, q2);

    if (cos_omega < 0.0f)
    {
        q2 = v4_mul(q2, -1);
        cos_omega = -cos_omega;
    }

    float k0, k1;
    if (cos_omega > 0.9999f)
    {
        k0 = 1.0f - t;
        k1 = t;
    }
    else
    {
        // compute sin of angle with trig identity
        float sin_omega = sqrtf(1.0f - cos_omega*cos_omega);

        // compute angle from sin and cosine
        float omega = atan2(sin_omega, cos_omega);
        float one_over_sin_omega = 1.0f / sin_omega;

        k0 = sin((1.0f - t) * omega) * one_over_sin_omega;
        k1 = sin(t * omega) * one_over_sin_omega;
    }

    out = v4_add(v4_mul(q1, k0), v4_mul(q2, k1));
    return out;
}

inline static m4 quat_to_m4(quat q)
{
    m4 out = m4_ident;

    float w = q.w_q;
    float x = q.v_q.x;
    float y = q.v_q.y;
    float z = q.v_q.z;

    out._11 = 1 - 2*y*y - 2*z*z;
    out._12 = 2*x*y + 2*w*z;
    out._13 = 2*x*z - 2*w*y;

    out._21 = 2*x*y - 2*w*z;
    out._22 = 1 - 2*x*x - 2*z*z;
    out._23 = 2*y*z + 2*w*x;

    out._31 = 2*x*z + 2*w*y;
    out._32 = 2*y*z - 2*w*x;
    out._33 = 1 - 2*x*x - 2*y*y;

    return out;
}

static char *read_txt_file(char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        log_err("Couldn't open file at %s\n", file_name);
        return 0;
    }

    char *buffer = 0;
    fseek(file, 0L, SEEK_END);
    const int buffer_size = ftell(file);
    rewind(file);

    buffer = calloc(buffer_size, sizeof(char));
    int read_size = fread(buffer, sizeof(char), buffer_size, file);
    buffer = (char *)realloc(buffer, read_size + 1);
    fclose(file);
    return buffer;
}

static u8 *read_bin_file(char *file_name)
{
    FILE *file = fopen(file_name, "rb");
    if (!file)
    {
        log_err("Couldn't open file at %s\n", file_name);
        return 0;
    }

    fseek(file, 0L, SEEK_END);
    const int buffer_size = ftell(file);
    rewind(file);

    u8 *buffer = malloc(buffer_size);
    if (!fread(buffer, buffer_size, 1, file))
    {
        fclose(file);
        free(buffer);
        log_err("couldn't read from file!");
    }
    fclose(file);
    return buffer;
}

#endif //TYPEDEFS_H
