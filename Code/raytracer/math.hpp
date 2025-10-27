#pragma once
#include <cmath>
#include <iostream>

// ======================
// 3D 向量结构 Vec3
// ======================
struct Vec3 {
    float x = 0, y = 0, z = 0;

    Vec3() = default;
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    // 运算符重载
    Vec3 operator+(const Vec3& b) const { return {x + b.x, y + b.y, z + b.z}; }
    Vec3 operator-(const Vec3& b) const { return {x - b.x, y - b.y, z - b.z}; }
    Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    Vec3 operator/(float s) const { return {x / s, y / s, z / s}; }

    Vec3& operator+=(const Vec3& b) {
        x += b.x; y += b.y; z += b.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& b) {
        x -= b.x; y -= b.y; z -= b.z;
        return *this;
    }

    Vec3& operator*=(float s) {
        x *= s; y *= s; z *= s;
        return *this;
    }

    Vec3& operator/=(float s) {
        x /= s; y /= s; z /= s;
        return *this;
    }
};

// ======================
// 向量工具函数
// ======================

// 点积
inline float dot(const Vec3& a, const Vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// 叉积
inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// 向量长度
inline float length(const Vec3& v) {
    return std::sqrt(dot(v, v));
}

// 归一化
inline Vec3 normalize(const Vec3& v) {
    float L = length(v);
    return (L > 0) ? v / L : v;
}

// 分量乘法（有时 shading 用）
inline Vec3 mul(const Vec3& a, const Vec3& b) {
    return {a.x * b.x, a.y * b.y, a.z * b.z};
}

// 输出重载（用于调试）
inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// ======================
// 射线结构 Ray
// ======================
struct Ray {
    Vec3 o; // origin
    Vec3 d; // direction (应当归一化)
    float tmin = 1e-4f;
    float tmax = 1e30f;
};
