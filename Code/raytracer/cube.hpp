#pragma once
#include "shape.hpp"


class Cube : public Shape {
public:
    Vec3 center {0, 0, -5};
    float halfSize = 1.0f;

    bool Intersect(const Ray& ray, HitInfo& hit) const override {
        
        Vec3 o = ray.o - center;
        Vec3 d = ray.d;

        float tmin = ray.tmin;
        float tmax = ray.tmax;

        auto slab = [&](float o, float d, float minv, float maxv) {
            const float EPS = 1e-8f;
            if (std::fabs(d) < EPS) {
                if (o < minv || o > maxv) return false;
                return true;
            }
            float t0 = (minv - o) / d;
            float t1 = (maxv - o) / d;
            if (t0 > t1) std::swap(t0, t1);
            tmin = std::max(tmin, t0);
            tmax = std::min(tmax, t1);
            return tmin <= tmax;
        };

        if (!slab(o.x, d.x, -halfSize, halfSize)) return false;
        if (!slab(o.y, d.y, -halfSize, halfSize)) return false;
        if (!slab(o.z, d.z, -halfSize, halfSize)) return false;

        float t = tmin;
        if (t < ray.tmin || t > ray.tmax || t >= hit.t) return false;

        Vec3 p = ray.o + ray.d * t;

        
        Vec3 n;
        Vec3 localP = p - center;
        float bias = halfSize + 1e-3f;
        if (std::fabs(localP.x - halfSize) < 1e-3f) n = {1,0,0};
        else if (std::fabs(localP.x + halfSize) < 1e-3f) n = {-1,0,0};
        else if (std::fabs(localP.y - halfSize) < 1e-3f) n = {0,1,0};
        else if (std::fabs(localP.y + halfSize) < 1e-3f) n = {0,-1,0};
        else if (std::fabs(localP.z - halfSize) < 1e-3f) n = {0,0,1};
        else n = {0,0,-1};

        hit.t = t;
        hit.pos = p;
        hit.normal = n;
        return true;
    }
    AABB GetAABB() const override {
        AABB box;
        Vec3 r = {halfSize, halfSize, halfSize};
        box.expand(center - r);
        box.expand(center + r);
        return box;
    }

};
