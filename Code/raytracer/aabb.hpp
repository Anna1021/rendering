#pragma once
#include "math.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

// ======================================================
// AABB - Axis Aligned Bounding Box
// 支持：
//   • 初始化 / 扩展点
//   • 合并两个 AABB
//   • 光线与 AABB 相交检测
//   • reset()（可选，为兼容旧代码）
// ======================================================
struct AABB {
    Vec3 min;
    Vec3 max;

    // 默认构造：空盒
    AABB()
        : min(1e30f, 1e30f, 1e30f),
          max(-1e30f, -1e30f, -1e30f) {}

    // 手动构造：给定 min/max
    AABB(const Vec3& mi, const Vec3& ma)
        : min(mi), max(ma) {}

    // === 重置（兼容 plane.hpp 旧代码）===
    void reset() {
        min = Vec3(1e30f, 1e30f, 1e30f);
        max = Vec3(-1e30f, -1e30f, -1e30f);
    }

    // === 扩展盒子以包含点 p ===
    void expand(const Vec3& p) {
        min.x = std::min(min.x, p.x);
        min.y = std::min(min.y, p.y);
        min.z = std::min(min.z, p.z);
        max.x = std::max(max.x, p.x);
        max.y = std::max(max.y, p.y);
        max.z = std::max(max.z, p.z);
    }

    // === 合并两个盒子 ===
    static AABB Union(const AABB& a, const AABB& b) {
        return AABB(
            Vec3(
                std::min(a.min.x, b.min.x),
                std::min(a.min.y, b.min.y),
                std::min(a.min.z, b.min.z)
            ),
            Vec3(
                std::max(a.max.x, b.max.x),
                std::max(a.max.y, b.max.y),
                std::max(a.max.z, b.max.z)
            )
        );
    }

    // === 获取中心点 ===
    Vec3 center() const {
        return (min + max) * 0.5f;
    }

    // === 获取尺寸 ===
    Vec3 size() const {
        return max - min;
    }
};

// ======================================================
// 光线与 AABB 相交检测
// ======================================================
inline bool IntersectAABB(const Ray& r, const AABB& box) {
    float tmin = r.tmin;
    float tmax = r.tmax;

    for (int i = 0; i < 3; i++) {
        float ro = (&r.o.x)[i];
        float rd = (&r.d.x)[i];
        float minb = (&box.min.x)[i];
        float maxb = (&box.max.x)[i];

        // 防止除零
        if (fabs(rd) < 1e-8f) {
            if (ro < minb || ro > maxb) return false;
            continue;
        }

        float invD = 1.0f / rd;
        float t0 = (minb - ro) * invD;
        float t1 = (maxb - ro) * invD;
        if (invD < 0.0f) std::swap(t0, t1);

        tmin = std::max(tmin, t0);
        tmax = std::min(tmax, t1);
        if (tmax < tmin) return false;
    }
    return true;
}
