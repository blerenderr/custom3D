#include <math.h>
#include "vec.hpp"
#pragma once

#ifndef PI_GUARD
#define PI_GUARD
const double PI = 3.14159265359;
#endif

struct Camera {
    //position
    double cx; double cy; double cz;
    //rotation
    double tx; double ty; double tz;
    //disp. surface pos (relative)
    double ex; double ey; double ez;

    // face direction (Z) unit vector
    Vec3 zDir;
    // relative Y unit vector
    Vec3 yDir;
    // relative X unit vector
    Vec3 xDir;

    double xFov;
    double yFov;

    Camera(double surfaceDistance, int screenHeight, int screenWidth) {
        ez = surfaceDistance;
        updateXfov(screenHeight);
        updateYfov(screenWidth);
        updateUnitVectors();
    }
    void updateXfov(int screenHeight);
    void updateYfov(int screenWidth);
    void updateUnitVectors();
};