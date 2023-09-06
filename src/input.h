#include <SDL.h>
#include <SDL_events.h>

const double PI = 3.14159265359;
const double ONE_DEGREE = PI/180;

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

#ifndef CAMERA_STRUCT
#define CAMERA_STRUCT
struct Camera {
    //position
    double cx; double cy; double cz;
    //rotation
    double tx; double ty; double tz;
    //disp. surface pos (relative)
    double ex; double ey; double ez;
};
#endif

void handleUserInput(SDL_Event event, Input *movement);

void handleCameraMovement(Input *movement, Camera *cam);