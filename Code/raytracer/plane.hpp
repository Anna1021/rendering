#pragma once
#include "shape.hpp"
#include "aabb.hpp"

// ===========================================
// Plane class: supports four vertices (from Blender)
// - 可通过 setFromPoints() 定义矩形平面
// - 自动支持双面检测（正反面都可见）
// - 若未设置，则默认为 y=-1 无限地面
// ===========================================
class Plane : public Shape {
public:
    Vec3 v0, v1, v2, v3;    // 四个角点
    Vec3 normal{0, 1, 0};   // 单位法线
    Vec3 p0{0, -1, 0};      // 参考点
    AABB box;               // 包围盒

    // === 默认构造：无限地面 y=-1 ===
    Plane() {
        box.min = {-1e6f, -1.0f, -1e6f};
        box.max = { 1e6f, -1.0f,  1e6f};
        p0 = {0, -1, 0};
        normal = {0, 1, 0};
        v0 = {-1e6f, -1, -1e6f};
        v1 = { 1e6f, -1, -1e6f};
        v2 = { 1e6f, -1,  1e6f};
        v3 = {-1e6f, -1,  1e6f};
    }

    // === 从 Blender 导出的 4 点设置平面 ===
    void setFromPoints(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d) {
        v0 = a; v1 = b; v2 = c; v3 = d;
        p0 = v0;
        // 注意：使用 v2-v0, v1-v0 保证与 Blender 导出方向一致
        normal = normalize(cross(v2 - v0, v1 - v0));

        // 初始化 AABB（有限平面）
        box.min = Vec3(1e9f, 1e9f, 1e9f);
        box.max = Vec3(-1e9f, -1e9f, -1e9f);
        box.expand(v0);
        box.expand(v1);
        box.expand(v2);
        box.expand(v3);
    }

    // === 光线相交检测 ===
    bool Intersect(const Ray& ray, HitInfo& hit) const override {
        float denom = dot(ray.d, normal);
        if (fabs(denom) < 1e-6f) return false; // 平行

        float t = dot(p0 - ray.o, normal) / denom;
        if (t < ray.tmin || t > ray.tmax || t >= hit.t) return false;

        Vec3 P = ray.o + ray.d * t;

        // === 判断命中点是否在四边形内部 ===
        Vec3 e0 = v1 - v0, e1 = v2 - v1, e2 = v3 - v2, e3 = v0 - v3;
        Vec3 c0 = cross(e0, P - v0);
        Vec3 c1 = cross(e1, P - v1);
        Vec3 c2 = cross(e2, P - v2);
        Vec3 c3 = cross(e3, P - v3);

        float s0 = dot(c0, normal);
        float s1 = dot(c1, normal);
        float s2 = dot(c2, normal);
        float s3 = dot(c3, normal);

        if (!((s0 >= 0 && s1 >= 0 && s2 >= 0 && s3 >= 0) ||
              (s0 <= 0 && s1 <= 0 && s2 <= 0 && s3 <= 0))) {
            return false;
        }

        // === 命中有效 ===
        hit.t = t;
        hit.pos = P;

        // ✅ 双面支持：若从背面打入，则翻转法线
        hit.normal = denom < 0 ? normal : (normal * -1.0f);

        return true;
    }

    AABB GetAABB() const override {
        return box;
    }
};
