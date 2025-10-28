#pragma once
#include <cmath>
#include <iostream>
#include <algorithm>

struct Vec3 {
    float x, y, z;
    Vec3(float v = 0) : x(v), y(v), z(v) {}
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    Vec3 operator/(float s) const { return {x / s, y / s, z / s}; }

    Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

    float length() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3 normalized() const {
        float len = length();
        return len > 1e-6f ? *this / len : Vec3(0, 0, 0);
    }
};

inline float dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}
inline Vec3 normalize(const Vec3& v) { return v.normalized(); }

inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    os << "(" << v.x << "," << v.y << "," << v.z << ")";
    return os;
}

// ✅ Ray — 完全统一：兼容所有旧代码 (aabb, sphere, cube, plane)
struct Ray {
    Vec3 o;  // origin
    Vec3 d;  // direction
    float tmin = 1e-4f;
    float tmax = 1e30f;

    Ray() = default;
    Ray(const Vec3& origin, const Vec3& dir)
        : o(origin), d(dir.normalized()) {}

    Vec3 at(float t) const { return o + d * t; }
};
