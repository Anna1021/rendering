#pragma once
#include <vector>
#include <algorithm>
#include "shape.hpp"
#include "aabb.hpp"

struct BVHNode {
    AABB box;
    BVHNode *left = nullptr;
    BVHNode *right = nullptr;
    std::vector<Shape*> objects;   // leaf: store shapes
};

class BVH {
public:
    BVHNode* root = nullptr;

    BVH(const std::vector<Shape*>& shapes) {
        root = build(shapes.begin(), shapes.end());
    }

    ~BVH() {
        destroy(root);
    }

    bool Intersect(const Ray& r, HitInfo& hit) const {
        return traverse(root, r, hit);
    }

private:
    using It = std::vector<Shape*>::const_iterator;

    BVHNode* build(It begin, It end) {
        int n = end - begin;
        BVHNode* node = new BVHNode();

        // Build bounding box for this node
        for (auto it = begin; it != end; ++it) {
            node->box = AABB::Union(node->box, (*it)->GetAABB());
        }

        // Leaf node condition
        if (n <= 2) {
            node->objects.assign(begin, end);
            return node;
        }

        // Choose split axis based on longest extent
        Vec3 diag = node->box.max - node->box.min;
        int axis = (diag.x > diag.y && diag.x > diag.z) ? 0 :
                   (diag.y > diag.z) ? 1 : 2;

        // Sort objects by centroid
        std::vector<Shape*> sorted(begin, end);
        std::sort(sorted.begin(), sorted.end(), [&](Shape* a, Shape* b) {
            Vec3 ca = (a->GetAABB().min + a->GetAABB().max) * 0.5f;
            Vec3 cb = (b->GetAABB().min + b->GetAABB().max) * 0.5f;
            if (axis == 0) return ca.x < cb.x;
            if (axis == 1) return ca.y < cb.y;
            return ca.z < cb.z;
        });

        It mid = sorted.begin() + n / 2;

        node->left  = build(sorted.begin(), mid);
        node->right = build(mid, sorted.end());
        return node;
    }

    bool traverse(BVHNode* node, const Ray& r, HitInfo& hit) const {
        if (!node || !IntersectAABB(r, node->box)) return false;

        bool hitSomething = false;

        // Check objects in leaf
        for (auto obj : node->objects) {
            HitInfo h = hit;
            if (obj->Intersect(r, h)) {
                hit = h;
                hitSomething = true;
            }
        }

        // Recurse children
        hitSomething |= traverse(node->left,  r, hit);
        hitSomething |= traverse(node->right, r, hit);
        return hitSomething;
    }

    void destroy(BVHNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
};
