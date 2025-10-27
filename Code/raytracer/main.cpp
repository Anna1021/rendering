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
    if (!scene.load("/Users/xinxinzi/Documents/postgraduate/rendering/cw1/ASCII/scene.txt")) {
        std::cerr << "❌ Failed to load scene.txt\n";
        return -1;
    }

    Camera& cam = scene.camera;
    std::vector<Shape*>& shapes = scene.objects;

    BVH bvh(shapes);
    Image img(cam.width, cam.height);

    for (int y = 0; y < cam.height; ++y) {
        for (int x = 0; x < cam.width; ++x) {
            Ray r = cam.generateRay(x, y);

            HitInfo hit;
            if (bvh.Intersect(r, hit)) {
                img(x, y) = hit.normal * 0.5f + Vec3(0.5f, 0.5f, 0.5f);
            } else {
                img(x, y) = Vec3(0.7f, 0.8f, 1.0f);
            }
        }
    }

    img.save("out.ppm");
    std::cout << "✅ Render completed → out.ppm\n";
    return 0;
}
