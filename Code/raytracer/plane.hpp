#pragma once
#include "shape.hpp"
#include "aabb.hpp"
#include "math.hpp"
#include "hit.hpp"
#include <cmath>
#include <iostream>

class Plane : public Shape {
public:
    Vec3 v0, v1, v2, v3;   // 四个顶点（Blender 导出顺序）
    Vec3 normal;          // 法线
    AABB box;             // 包围盒（用于 BVH）

    Plane() = default;

    void setFromPoints(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d) {
        v0 = a; v1 = b; v2 = c; v3 = d;

        // 计算法线
        normal = normalize(cross(v1 - v0, v2 - v0));

        // 如果朝下（y为负），翻转使地面法线向上
        if (normal.y < 0) normal = normal * -1.0f;

        // 构建包围盒
        box.min = Vec3( 1e9f,  1e9f,  1e9f);
        box.max = Vec3(-1e9f, -1e9f, -1e9f);
        box.expand(v0); box.expand(v1); box.expand(v2); box.expand(v3);

        std::cout << "📏 Plane loaded: normal=("
                  << normal.x << "," << normal.y << "," << normal.z << ")\n";
    }

    bool Intersect(const Ray& ray, HitInfo& hit) const override {
        float denom = dot(normal, ray.d);
        if (fabs(denom) < 1e-6f) return false; // 平行，无交点

        float t = dot(v0 - ray.o, normal) / denom;
        if (t < ray.tmin || t >= hit.t) return false;

        Vec3 P = ray.o + ray.d * t;

        // === 判断点 P 是否在四边形内 ===
        auto sameSide = [&](const Vec3& A, const Vec3& B, const Vec3& P, const Vec3& n) {
            return dot(cross(B - A, P - A), n) >= 0;
        };

        if (!sameSide(v0, v1, P, normal)) return false;
        if (!sameSide(v1, v2, P, normal)) return false;
        if (!sameSide(v2, v3, P, normal)) return false;
        if (!sameSide(v3, v0, P, normal)) return false;

        // ✅ 合法命中
        hit.t = t;
        hit.pos = P;
        hit.normal = normal;
        return true;
    }

    AABB GetAABB() const override {
        return box;
    }
};
