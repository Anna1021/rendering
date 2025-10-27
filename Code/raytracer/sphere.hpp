#pragma once
#include <cmath>
#include "shape.hpp"
#include "math.hpp"
#include "hit.hpp"


class Sphere : public Shape {
public:
    Vec3 center {0, 0, -3};
    float radius = 1.0f;

    bool Intersect(const Ray& ray, HitInfo& hit) const override {
        
        Vec3 oc = ray.o - center;
        float a = dot(ray.d, ray.d);
        float b = 2.0f * dot(oc, ray.d);
        float c = dot(oc, oc) - radius * radius;

        float disc = b*b - 4*a*c;
        if (disc < 0) return false; 

        float sqrtD = std::sqrt(disc);
        float t0 = (-b - sqrtD) / (2*a);
        float t1 = (-b + sqrtD) / (2*a);

        float t = t0;
        if (t < ray.tmin || t > ray.tmax) {
            t = t1;
            if (t < ray.tmin || t > ray.tmax) return false;
        }


        if (t >= hit.t) return false;

        hit.t = t;
        hit.pos = ray.o + ray.d * t;
        hit.normal = normalize(hit.pos - center);
        return true;
    }


    AABB GetAABB() const override {
        AABB box;
        Vec3 r = {radius, radius, radius};
        box.expand(center - r);
        box.expand(center + r);
        return box;
    }
};
