const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int VERTS_ARR_SIZE = 8;
const int POLYS_ARR_SIZE = 12;
const double PI = 3.14159265359;
const double ONE_DEGREE = PI/180;

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Poly {
    Vec3 a;
    Vec3 b;
    Vec3 c;
};

struct Input {
    bool strafe_left;
    bool strafe_right;
    bool forward;
    bool backward;
    bool up;
    bool down;
    bool look_left;
    bool look_right;
    bool look_up;
    bool look_down;
};

struct Camera {
    //position
    double cx; double cy; double cz;
    //rotation
    double tx; double ty; double tz;
    //disp. surface pos (relative)
    double ex; double ey; double ez;
};