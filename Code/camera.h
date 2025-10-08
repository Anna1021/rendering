#ifndef CAMERA_H
#define CAMERA_H
#include <string>
#include <array>

struct Ray {
    std::array<float,3> origin;
    std::array<float,3> dir;
};

class Camera {
public:
    bool loadFromJSON(const std::string& jsonPath);
    Ray generateRay(float px, float py) const;

    int width=0, height=0;

private:
    std::array<float,3> pos{0,0,0};
    std::array<float,3> gaze{0,0,-1};
    std::array<float,3> right{1,0,0};
    std::array<float,3> up{0,1,0};
    float focal=50.0f;
    float sensorW=36.0f;
    float sensorH=24.0f;

    static std::array<float,3> normalize(const std::array<float,3>& v);
    static std::array<float,3> cross(const std::array<float,3>& a, const std::array<float,3>& b);
    static float dot(const std::array<float,3>& a, const std::array<float,3>& b);

    static bool extractArray3(const std::string& s, const std::string& key, std::array<float,3>& out);
    static bool extractNumber(const std::string& s, const std::string& key, float& out);
    static bool extractInt2(const std::string& s, const std::string& key, int& a, int& b);
};

#endif // CAMERA_H
