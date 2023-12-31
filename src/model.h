#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "vec3.h"

#pragma once

struct Mesh {
    std::vector<Vec3> vertices;
    std::vector<Vec3> indices;
    Vec3 origin;
    double scale;
    void readFile(std::string filename);
    Mesh(std::string filename) {
        readFile(filename);
        this->origin = Vec3(0, 0, 0);
        this->scale = 1.0;
    }
    Mesh(std::string filename, Vec3 origin) {
        readFile(filename);
        this->origin = origin;
        this->scale = 1.0;
    }
    Mesh(std::string filename, Vec3 origin, double scale) {
        readFile(filename);
        this->origin = origin;
        this->scale = scale;
    }
};