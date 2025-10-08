#include "image.h"
#include "camera.h"
#include <iostream>

static unsigned char clamp01(float x){ if(x<0) x=0; if(x>1) x=1; return (unsigned char)(x*255.0f+0.5f); }

int main(){
    Camera cam;
    if(!cam.loadFromJSON("../ASCII/scene.json")){
        std::cerr << "Failed to load camera from ../ASCII/scene.json\n";
        return 1;
    }
    Image img(cam.width, cam.height);

    for(int y=0; y<cam.height; ++y){
        for(int x=0; x<cam.width; ++x){
            Ray r = cam.generateRay((float)x, (float)y);
            // 把方向[-1,1] 映射到 [0,1]
            float rx = 0.5f*(r.dir[0]+1.0f);
            float ry = 0.5f*(r.dir[1]+1.0f);
            float rz = 0.5f*(r.dir[2]+1.0f);
            img.setPixel(x,y, clamp01(rx), clamp01(ry), clamp01(rz));
        }
    }
    img.write("../Output/dirs.ppm");
    std::cout << "Wrote ../Output/dirs.ppm\n";
    return 0;
}
