#include "camera.h"

void Camera::updateXfov(int screenHeight) {
    xFov = 2*atan(screenHeight/(ez));
}
void Camera::updateYfov(int screenWidth) {
    yFov = 2*atan(screenWidth/(ez));
}
void Camera::updateUnitVectors() {
    // there is probably a better solution to this...
    double x = sin(ty);
    double y = -sin(tx);
    bool zIsNegative = (ty < 3*PI/2.0 && ty > PI/2.0);
    zDir = Vec3(x, y, zIsNegative ? -abs(sqrt(1 - x*x - y*y)) : abs(sqrt(1 - x*x - y*y)));
    zDir.normalize();
    xDir = zDir.crossProduct(Vec3(0,1,0));
    xDir.normalize();
    yDir = xDir.crossProduct(zDir);
    yDir.normalize();
}