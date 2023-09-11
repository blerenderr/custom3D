#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Vec3 {
    double x;
    double y;
    double z;
    Vec3() { x = 0; y = 0; z = 0; }
    Vec3(double x, double y, double z) {
        this->x = x; this->y = y; this->z = z;
    }
};

struct Mesh {
    std::vector<Vec3> vertices;
    std::vector<Vec3> indices;
    Vec3 origin;
    double scale;
    void readFile(std::string filename);
    Mesh(std::string filename) {readFile(filename);}
    Mesh(std::string filename, Vec3 origin) {
        readFile(filename);
        this->origin = origin;
    }
};