#pragma once
#include "math.hpp"
#include "hit.hpp"
#include "aabb.hpp"
class Shape {
public:
    virtual ~Shape() = default;


    virtual bool Intersect(const Ray& ray, HitInfo& hit) const = 0;
    virtual AABB GetAABB() const = 0;

};
