#include <math.h>

#pragma once

struct Vec3 {
    double x;
    double y;
    double z;
    Vec3() { x = 0; y = 0; z = 0; }
    Vec3(double x, double y, double z) {
        this->x = x; this->y = y; this->z = z;
    }
    double dotProduct(Vec3 other);
    Vec3 crossProduct(Vec3 other);
    Vec3 add(Vec3 other);
    Vec3 subtract(Vec3 other);
    void normalize();
};

struct Vec2 {
    double x;
    double y;
    Vec2() { x = 0; y = 0; }
    Vec2(double x, double y) {
        this->x = x; this->y = y;
    }
    void normalize();
};