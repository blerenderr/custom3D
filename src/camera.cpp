#include "camera.h"
#include <iostream>

void Camera::updateXfov(int screenHeight) {
    xFov = 2*atan(screenHeight/(ez));
}
void Camera::updateYfov(int screenWidth) {
    yFov = 2*atan(screenWidth/(ez));
}
void Camera::updateUnitVectors() {
    // this is so horrible but it works
    bool zIsNegative = (ty < 3*PI/2.0 && ty > PI/2.0);
    zDir = Vec3(sin(ty), -sin(tx), zIsNegative ? -(1 - sin(ty)*sin(ty) - sin(tx)*sin(tx)) : (1 - sin(ty)*sin(ty) - sin(tx)*sin(tx)));
    std::cout << zDir.x << " " << zDir.y << " " << zDir.z << std::endl;
    zDir.normalize();
    xDir = zDir.crossProduct(Vec3(0,1,0));
    xDir.normalize();
    yDir = xDir.crossProduct(zDir);
    yDir.normalize();
}