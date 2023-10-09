#include "vec3.h"

double Vec3::dotProduct(Vec3 other) {
    return this->x*other.x + this->y*other.y + this->z*other.z;
}
Vec3 Vec3::crossProduct(Vec3 other) {
    return Vec3(abs(this->y*other.z - this->z*other.y),
                abs(this->z*other.x - this->x*other.z),
                abs(this->x*other.y - this->y*other.x));
}
Vec3 Vec3::add(Vec3 other) {
    return Vec3(this->x+other.x, this->y+other.y, this->z+other.z);
}
Vec3 Vec3::subtract(Vec3 other) {
    return Vec3(this->x-other.x, this->y-other.y, this->z-other.z);
}
void Vec3::normalize() {
    double invsqrt = 1/sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
    this->x *= invsqrt; this->y *= invsqrt; this->z *= invsqrt;
}