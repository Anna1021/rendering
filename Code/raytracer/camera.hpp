#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cmath>
#include "math.hpp"

class Camera {
public:
    Vec3 pos, forward, up, right;
    int width = 640, height = 480;
    float fov = 60.0f;   // degrees
    float aspect = 4.0f/3.0f;
    float scale = std::tan((fov * 3.1415926f / 180.f)*0.5f);

    bool load(const std::string& filename) {
        std::ifstream in(filename);
        if (!in.is_open()) { std::cerr << "Error: cannot open " << filename << "\n"; return false; }
        std::string tag;
        while (in >> tag) {
            if (tag == "camera") {
                in >> pos.x >> pos.y >> pos.z;
                in >> forward.x >> forward.y >> forward.z;
                in >> up.x >> up.y >> up.z;
                in >> fov >> width >> height;
                aspect = float(width) / height;
                scale  = std::tan((fov * 3.1415926f / 180.f)*0.5f);
                right  = normalize(cross(forward, up));
                up     = normalize(cross(right, forward));
                std::cout << "Camera loaded: " << width << "x" << height << " fov=" << fov << "\n";
                return true;
            }
        }
        return false;
    }

    Ray generateRay(float px, float py) const {
        float x = ( (px + 0.5f) / width * 2 - 1 ) * aspect * scale;
        float y = ( 1 - (py + 0.5f) / height * 2 ) * scale;
        // 相机坐标 dirCam = (x, y, -1)
        Vec3 dirCam = normalize(Vec3(x, y, -1));
        // 变到世界坐标。注意 Blender 相机朝 -Z，因此这里用 -forward
        Vec3 dirWorld = normalize(right * dirCam.x + up * dirCam.y - forward * dirCam.z);
        return { pos, dirWorld };
    }
};
