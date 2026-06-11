#ifndef MATH3D_H
#define MATH3D_H

#include <cmath>

struct Vec3
{
    float x, y, z;
};

struct Mat4
{
    float m[16];
};

inline Vec3 makeVec3(float x, float y, float z)
{
    return {x, y, z};
}

inline Vec3 subtract(Vec3 a, Vec3 b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline Vec3 cross(Vec3 a, Vec3 b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

inline float dot(Vec3 a, Vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vec3 normalize(Vec3 v)
{
    float len = std::sqrt(dot(v, v));

    if (len == 0.0f)
        return {0.0f, 0.0f, 0.0f};

    return {v.x / len, v.y / len, v.z / len};
}

inline Mat4 identity()
{
    Mat4 r = {};

    r.m[0]  = 1.0f;
    r.m[5]  = 1.0f;
    r.m[10] = 1.0f;
    r.m[15] = 1.0f;

    return r;
}

inline Mat4 multiply(Mat4 a, Mat4 b)
{
    Mat4 r = {};

    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            r.m[col * 4 + row] =
                a.m[0 * 4 + row] * b.m[col * 4 + 0] +
                a.m[1 * 4 + row] * b.m[col * 4 + 1] +
                a.m[2 * 4 + row] * b.m[col * 4 + 2] +
                a.m[3 * 4 + row] * b.m[col * 4 + 3];
        }
    }

    return r;
}

inline Mat4 translate(float x, float y, float z)
{
    Mat4 r = identity();

    r.m[12] = x;
    r.m[13] = y;
    r.m[14] = z;

    return r;
}

inline Mat4 scale(float s)
{
    Mat4 r = identity();

    r.m[0]  = s;
    r.m[5]  = s;
    r.m[10] = s;

    return r;
}

inline Mat4 rotateY(float angleDegrees)
{
    float a = angleDegrees * 3.14159265f / 180.0f;

    float c = std::cos(a);
    float s = std::sin(a);

    Mat4 r = identity();

    r.m[0]  = c;
    r.m[2]  = -s;
    r.m[8]  = s;
    r.m[10] = c;

    return r;
}

inline Mat4 perspective(float fovDegrees, float aspect, float nearPlane, float farPlane)
{
    float fovRad = fovDegrees * 3.14159265f / 180.0f;
    float f = 1.0f / std::tan(fovRad / 2.0f);

    Mat4 r = {};

    r.m[0]  = f / aspect;
    r.m[5]  = f;
    r.m[10] = (farPlane + nearPlane) / (nearPlane - farPlane);
    r.m[11] = -1.0f;
    r.m[14] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);

    return r;
}

inline Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up)
{
    Vec3 f = normalize(subtract(center, eye));
    Vec3 s = normalize(cross(f, up));
    Vec3 u = cross(s, f);

    Mat4 r = identity();

    r.m[0] = s.x;
    r.m[4] = s.y;
    r.m[8] = s.z;

    r.m[1] = u.x;
    r.m[5] = u.y;
    r.m[9] = u.z;

    r.m[2]  = -f.x;
    r.m[6]  = -f.y;
    r.m[10] = -f.z;

    r.m[12] = -dot(s, eye);
    r.m[13] = -dot(u, eye);
    r.m[14] = dot(f, eye);

    return r;
}

#endif