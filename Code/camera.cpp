#include "camera.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

// load from json
bool Camera::loadFromJSON(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string data = buffer.str();

    if(!extractArray3(data,"location",pos)) return false;
    if(!extractArray3(data,"gaze",gaze)) return false;
    extractArray3(data,"up",up); // optional, fallback to {0,1,0}
    if(!extractNumber(data,"focal_length",focal)) return false;
    if(!extractNumber(data,"sensor_width",sensorW)) return false;
    if(!extractNumber(data,"sensor_height",sensorH)) return false;
    if(!extractInt2(data,"resolution",width,height)) return false;

    // --- normalize gaze & up
    auto normalize3 = [](std::array<float,3>& v){
        float len = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
        if(len > 0.0f) for(int i=0;i<3;i++) v[i] /= len;
    };
    normalize3(gaze);
    normalize3(up);

    // --- compute right = gaze × up
    right = {
        gaze[1]*up[2] - gaze[2]*up[1],
        gaze[2]*up[0] - gaze[0]*up[2],
        gaze[0]*up[1] - gaze[1]*up[0]
    };
    normalize3(right);

    // --- recompute up = right × gaze
    up = {
        right[1]*gaze[2] - right[2]*gaze[1],
        right[2]*gaze[0] - right[0]*gaze[2],
        right[0]*gaze[1] - right[1]*gaze[0]
    };
    return true;
}


// ========================
// helper function
// ========================
bool Camera::extractArray3(const std::string& s, const std::string& key, std::array<float,3>& out){
    auto k = "\"" + key + "\"";
    size_t p = s.find(k);
    if(p==std::string::npos) return false;
    p = s.find('[', p);
    if(p==std::string::npos) return false;
    size_t q = s.find(']', p);
    if(q==std::string::npos) return false;
    std::string sub = s.substr(p+1, q-p-1);
    std::replace(sub.begin(), sub.end(), ',', ' ');
    std::istringstream iss(sub);
    iss >> out[0] >> out[1] >> out[2];
    return !iss.fail();
}

bool Camera::extractNumber(const std::string& s, const std::string& key, float& out){
    auto k = "\"" + key + "\"";
    size_t p = s.find(k);
    if(p==std::string::npos) return false;
    p = s.find(':', p);
    if(p==std::string::npos) return false;
    std::string sub = s.substr(p+1, 64);
    std::istringstream iss(sub);
    iss >> out;
    return !iss.fail();
}

bool Camera::extractInt2(const std::string& s, const std::string& key, int& a, int& b){
    auto k = "\"" + key + "\"";
    size_t p = s.find(k);
    if(p==std::string::npos) return false;
    p = s.find('[', p);
    if(p==std::string::npos) return false;
    size_t q = s.find(']', p);
    if(q==std::string::npos) return false;
    std::string sub = s.substr(p+1, q-p-1);
    std::replace(sub.begin(), sub.end(), ',', ' ');
    std::istringstream iss(sub);
    iss >> a >> b;
    return !iss.fail();
}

// ========================
// 改进后的 generateRay
// ========================
Ray Camera::generateRay(float px, float py) const {
    // 像素坐标归一化到 [-0.5,0.5]
    float ndcX = ( (px + 0.5f) / width  - 0.5f ) * (sensorW / focal);
    float ndcY = ( (py + 0.5f) / height - 0.5f ) * (sensorH / focal);

    // 相机空间 -> 世界空间
    std::array<float,3> dir = {
        ndcX * right[0] + ndcY * up[0] + gaze[0],
        ndcX * right[1] + ndcY * up[1] + gaze[1],
        ndcX * right[2] + ndcY * up[2] + gaze[2]
    };

    // 单位化
    float len = std::sqrt(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);
    if(len > 0.0f){
        for(int i=0;i<3;i++) dir[i] /= len;
    }

    Ray r;
    r.origin = pos; // 从 JSON 相机位置出发
    r.dir = dir;
    return r;
}
