#include "camera.h"

void Camera::updateXfov(int screenHeight) {
    xFov = 2*atan(screenHeight/(ez));
}
void Camera::updateYfov(int screenWidth) {
    yFov = 2*atan(screenWidth/(ez));
}
void Camera::updateUnitVectors() {
    zDir = Vec3(sin(ty), -sin(tx), cos(ty));
    zDir.normalize();
    xDir = zDir.crossProduct(Vec3(0,1,0));
    xDir.normalize();
    yDir = xDir.crossProduct(zDir);
    yDir.normalize();
}