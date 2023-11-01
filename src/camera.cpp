#include "camera.h"
#include <iostream>

void Camera::updateXfov(int screenHeight) {
    xFov = 2*atan(screenHeight/(ez));
}
void Camera::updateYfov(int screenWidth) {
    yFov = 2*atan(screenWidth/(ez));
}
void Camera::updateUnitVectors() {
    double z = cos(ty)*cos(tx);
    double x = sin(ty)*cos(tx);
    double y = -sin(tx);

    zDir = Vec3(x, y, z);
    zDir.normalize();
    xDir = zDir.crossProduct(Vec3(0,1,0));
    xDir.normalize();
    yDir = xDir.crossProduct(zDir);
    yDir.normalize();
}