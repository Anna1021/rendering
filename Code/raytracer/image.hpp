#pragma once
#include <vector>
#include <fstream>
#include "math.hpp"

class Image {
public:
    int W, H;
    std::vector<Vec3> data;

    Image(int w, int h) : W(w), H(h), data(w*h) {}

    Vec3& operator()(int x, int y) {
        return data[y*W + x];
    }

    void save(const std::string& filename){
        std::ofstream f(filename);
        f << "P3\n" << W << " " << H << "\n255\n";
        for (auto& p : data){
            f << int(p.x*255) << " " << int(p.y*255) << " " << int(p.z*255) << "\n";
        }
    }
};
