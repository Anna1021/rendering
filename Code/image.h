#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

class Image {
public:
    int width, height;
    std::vector<unsigned char> data; // RGB 每像素3字节

    Image(int w, int h);
    Image(const std::string& filename);

    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void write(const std::string& filename) const;

private:
    void read(const std::string& filename);
};

#endif
