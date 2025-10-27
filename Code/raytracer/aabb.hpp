#pragma once
#include "math.hpp"

struct AABB {
    Vec3 min;
    Vec3 max;

    AABB() : min(1e30f, 1e30f, 1e30f), max(-1e30f, -1e30f, -1e30f) {}

    void expand(const Vec3& p) {
        min.x = std::min(min.x, p.x);
        min.y = std::min(min.y, p.y);
        min.z = std::min(min.z, p.z);
        max.x = std::max(max.x, p.x);
        max.y = std::max(max.y, p.y);
        max.z = std::max(max.z, p.z);
    }

    static AABB Union(const AABB& a, const AABB& b) {
        AABB r;
        r.min = { std::min(a.min.x,b.min.x), std::min(a.min.y,b.min.y), std::min(a.min.z,b.min.z) };
        r.max = { std::max(a.max.x,b.max.x), std::max(a.max.y,b.max.y), std::max(a.max.z,b.max.z) };
        return r;
    }
};
inline bool IntersectAABB(const Ray& r, const AABB& box) {
    float tmin = r.tmin;
    float tmax = r.tmax;
    for (int i = 0; i < 3; i++) {
        float ro = (&r.o.x)[i];
        float rd = (&r.d.x)[i];
        float minb = (&box.min.x)[i];
        float maxb = (&box.max.x)[i];

        float invD = 1.0f / rd;
        float t0 = (minb - ro) * invD;
        float t1 = (maxb - ro) * invD;
        if (invD < 0) std::swap(t0, t1);

        tmin = std::max(tmin, t0);
        tmax = std::min(tmax, t1);
        if (tmax < tmin) return false;
    }
    return true;
}
