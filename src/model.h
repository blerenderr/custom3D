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
    void readFile(std::string filename);
    Mesh(std::string filename) {readFile(filename);}
    Mesh(std::string filename, Vec3 origin) {
        readFile(filename);
        this->origin = origin;
    }
};