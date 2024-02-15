#include <fstream>
#include <string>
#include <vector>

#pragma once

typedef unsigned char BYTE;

struct Texture {
    int width;
    int height;
    std::vector<char> data;
    void readFile(std::string filename);
    Texture(std::string filename, int width, int height) {
        readFile(filename);
        this->width = width; this->height = height;
    }
    void readTexture(int x, int y, BYTE* r, BYTE* g, BYTE* b);
};