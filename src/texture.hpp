#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "vec.hpp"

#pragma once

struct Texture {
    int width;
    int height;
    void* red;
    void* green;
    void* blue;
    Texture(std::string filename) {
        
    }
};