#include "image.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

Image::Image(int w, int h) : width(w), height(h), data(3*w*h, 0) {}

Image::Image(const std::string& filename) {
    read(filename);
}

void Image::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    int idx = 3 * (y * width + x);
    data[idx] = r;
    data[idx + 1] = g;
    data[idx + 2] = b;
}

void Image::write(const std::string& filename) const {
    std::ofstream ofs(filename);
    if (!ofs) throw std::runtime_error("Cannot open file for writing: " + filename);
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; i++) {
        ofs << (int)data[3*i] << " "
            << (int)data[3*i+1] << " "
            << (int)data[3*i+2] << "\n";
    }
    ofs.close();
}

void Image::read(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) throw std::runtime_error("Cannot open file for reading: " + filename);

    std::string header;
    ifs >> header;
    if (header != "P3") throw std::runtime_error("Only ASCII PPM (P3) supported");

    int maxval;
    ifs >> width >> height >> maxval;

    data.resize(3 * width * height);
    for (int i = 0; i < width * height; i++) {
        int r, g, b;
        ifs >> r >> g >> b;
        data[3*i] = (unsigned char)r;
        data[3*i+1] = (unsigned char)g;
        data[3*i+2] = (unsigned char)b;
    }
}
