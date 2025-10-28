#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

#include "math.hpp"
#include "hit.hpp"
#include "shape.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "cube.hpp"
#include "aabb.hpp"
#include "bvh.hpp"
#include "camera.hpp"
#include "image.hpp"
#include "scene_loader.hpp"

int main() {
    Scene scene;

    // ✅ 使用相对路径（确保 ASCII/scene.txt 与 raytracer 在同目录）
    if (!scene.load("../../ASCII/scene.txt")) {
        std::cerr << "❌ Failed to load scene file (ASCII/scene.txt)\n";
        return -1;
    }

    Camera& cam = scene.camera;
    std::vector<Shape*>& shapes = scene.objects;

    if (shapes.empty()) {
        std::cerr << "⚠️ No objects found in scene.txt, please check its path or content.\n";
        return -1;
    }

    BVH bvh(shapes);
    Image img(cam.width, cam.height);

    std::cout << "🎨 Start rendering...\n";

    for (int y = 0; y < cam.height; ++y) {
        for (int x = 0; x < cam.width; ++x) {
            Ray r = cam.generateRay(x, y);

            HitInfo hit;
            bool hitFound = false;

            // === BVH（检测球体与立方体）
            if (bvh.Intersect(r, hit)) {
                hitFound = true;
            }

            // === 检测平面（Plane）
            for (auto* s : shapes) {
                Plane* plane = dynamic_cast<Plane*>(s);
                if (plane) {
                    HitInfo hp;
                    if (plane->Intersect(r, hp) && hp.t < hit.t) {
                        hit = hp;
                        hitFound = true;
                    }
                }
            }

            // === 简单着色
            if (hitFound) {
                // 将法线映射到颜色
                img(x, y) = hit.normal * 0.5f + Vec3(0.5f, 0.5f, 0.5f);
            } else {
                // 天空颜色
                img(x, y) = Vec3(0.7f, 0.8f, 1.0f);
            }
        }
    }

    img.save("out.ppm");
    std::cout << "✅ Render completed → out.ppm\n";
    return 0;
}
