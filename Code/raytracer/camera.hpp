#pragma once
#include "math.hpp"
#include <cmath>
#include <iostream>

struct Camera {
    Vec3 pos;
    Vec3 forward;
    Vec3 up;
    Vec3 right;
    float fov;
    int width;
    int height;
    float aspect;
    float scale;

    Camera() : fov(60), width(800), height(600) {}

    Ray generateRay(float x, float y) const {
        // 像素归一化坐标 [-1,1]
        float px = (2 * (x + 0.5f) / width  - 1) * aspect * scale;
        float py = (1 - 2 * (y + 0.5f) / height) * scale;

        Vec3 dir = normalize(forward + right * px + up * py);
        return Ray(pos, dir);
    }
};
