#include "camera.h"
#include "image.h"
#include "scene.h"
#include <iostream>

int main() {
    Camera cam;
    cam.loadFromJSON("../ASCII/scene.json");

    Scene scene;
    scene.loadFromJSON("../ASCII/scene.json");

    std::cout << "Lights loaded: " << scene.lights.size() << "\n";
    std::cout << "Spheres loaded: " << scene.spheres.size() << "\n";
    std::cout << "Cubes loaded: " << scene.cubes.size() << "\n";
    std::cout << "Planes loaded: " << scene.planes.size() << "\n";

    Image img(cam.width, cam.height);
for (int y = 0; y < cam.height; y++) {
    for (int x = 0; x < cam.width; x++) {
        Ray ray = cam.generateRay(x, y);

        
        float r = 0.5f * (ray.dir[0] + 1.0f);
        float g = 0.5f * (ray.dir[1] + 1.0f);
        float b = 0.5f * (ray.dir[2] + 1.0f);

        
        img.setPixel(x, y,
            static_cast<unsigned char>(r * 255),
            static_cast<unsigned char>(g * 255),
            static_cast<unsigned char>(b * 255));
    }
}

    img.write("../Output/dirs.ppm");
    return 0;
}
