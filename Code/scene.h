#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>

struct Light {
    std::string type;
    std::string name;
    float location[3];
    float color[3];
    float energy;
};

struct Sphere {
    std::string name;
    float center[3];
    float radius;
};

struct Cube {
    std::string name;
    float location[3];
    float rotation[3];
    float scale;
};

struct Plane {
    std::string name;
    float corners[4][3]; // 四个角点
};

struct Scene {
    std::vector<Light> lights;
    std::vector<Sphere> spheres;
    std::vector<Cube> cubes;
    std::vector<Plane> planes;

    bool loadFromJSON(const std::string& path);
};

#endif
