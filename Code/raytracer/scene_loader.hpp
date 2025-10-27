#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "camera.hpp"
#include "sphere.hpp"
#include "cube.hpp"
#include "plane.hpp"

struct Scene {
    Camera camera;
    std::vector<Shape*> objects;   // 统一放进 BVH 的对象（包含 plane/sphere/cube）

    bool load(const std::string& path) {
        std::ifstream in(path);
        if (!in.is_open()) {
            std::cerr << "❌ Cannot open " << path << "\n";
            return false;
        }

        std::string tag;
        while (in >> tag) {
            if (tag == "camera") {
                in >> camera.pos.x >> camera.pos.y >> camera.pos.z;
                in >> camera.forward.x >> camera.forward.y >> camera.forward.z;
                in >> camera.up.x >> camera.up.y >> camera.up.z;
                in >> camera.fov >> camera.width >> camera.height;

                camera.aspect = float(camera.width) / camera.height;
                camera.scale  = std::tan((camera.fov * 3.1415926f / 180.f) * 0.5f);
                camera.right  = normalize(cross(camera.forward, camera.up));
                camera.up     = normalize(cross(camera.right, camera.forward));
                std::cout << "📷 Camera " << camera.width << "x" << camera.height
                          << " fov=" << camera.fov << "\n";
            }
            else if (tag == "light") {
                // 先跳过：位置 + 强度
                Vec3 L; float I; in >> L.x >> L.y >> L.z >> I;
                // 可存到自定义数组，后面做光照时使用
            }
            else if (tag == "sphere") {
                auto* s = new Sphere();
                in >> s->center.x >> s->center.y >> s->center.z >> s->radius;
                objects.push_back(s);
            }
            else if (tag == "cube") {
                auto* c = new Cube();
                in >> c->center.x >> c->center.y >> c->center.z >> c->halfSize;
                objects.push_back(c);
}

            else if (tag == "plane") {
                auto* p = new Plane();
                Vec3 v0, v1, v2, v3;
                in >> v0.x >> v0.y >> v0.z
                   >> v1.x >> v1.y >> v1.z
                   >> v2.x >> v2.y >> v2.z
                   >> v3.x >> v3.y >> v3.z;
                p->setFromPoints(v0, v1, v2, v3);
                objects.push_back(p);
            }
            else {
                // 跳过整行未知内容
                std::string line; std::getline(in, line);
            }
        }

        std::cout << "✅ Loaded " << objects.size() << " objects\n";
        return true;
    }
};
