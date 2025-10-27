#include "scene.h"
#include <fstream>
#include <sstream>
#include <iostream>


static void parseArray(const std::string& line, float out[3]) {
    size_t l = line.find("[");
    size_t r = line.find("]");
    if (l == std::string::npos || r == std::string::npos) return;
    std::string nums = line.substr(l+1, r-l-1);
    std::replace(nums.begin(), nums.end(), ',', ' ');
    std::stringstream ss(nums);
    ss >> out[0] >> out[1] >> out[2];
}

bool Scene::loadFromJSON(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Cannot open " << path << "\n";
        return false;
    }
    std::string line;

    while (std::getline(f, line)) {
        // Light
        if (line.find("\"type\": \"point\"") != std::string::npos) {
            Light l;
            l.type = "point";
            while (std::getline(f, line) && line.find("}") == std::string::npos) {
                if (line.find("\"name\"") != std::string::npos) {
                    size_t q1 = line.find("\"", line.find(":")+1);
                    size_t q2 = line.find("\"", q1+1);
                    l.name = line.substr(q1+1, q2-q1-1);
                } else if (line.find("\"location\"") != std::string::npos) {
                    parseArray(line, l.location);
                } else if (line.find("\"color\"") != std::string::npos) {
                    parseArray(line, l.color);
                } else if (line.find("\"energy\"") != std::string::npos) {
                    l.energy = std::stof(line.substr(line.find(":")+1));
                }
            }
            lights.push_back(l);
        }

        // Sphere
        if (line.find("\"center\"") != std::string::npos) {
            Sphere s;
            parseArray(line, s.center);
            // read radius
            std::getline(f, line);
            s.radius = std::stof(line.substr(line.find(":")+1));
            spheres.push_back(s);
        }

        // Cube
        if (line.find("\"cubes\"") != std::string::npos) {
            while (std::getline(f, line) && line.find("]") == std::string::npos) {
                if (line.find("\"location\"") != std::string::npos) {
                    Cube c;
                    parseArray(line, c.location);
                    // rotation
                    std::getline(f, line); parseArray(line, c.rotation);
                    // scale
                    std::getline(f, line); c.scale = std::stof(line.substr(line.find(":")+1));
                    cubes.push_back(c);
                }
            }
        }

        // Plane
        if (line.find("\"planes\"") != std::string::npos) {
            while (std::getline(f, line) && line.find("]") == std::string::npos) {
                if (line.find("\"corners\"") != std::string::npos) {
                    Plane p;
                    for (int i=0; i<4; i++) {
                        std::getline(f, line);
                        parseArray(line, p.corners[i]);
                    }
                    planes.push_back(p);
                }
            }
        }
    }

    return true;
}
