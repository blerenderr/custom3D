#include <SDL_events.h>
#include "camera.h"

#pragma once
#ifndef PI_GUARD
#define PI_GUARD
const double PI = 3.14159265359;
#endif
const double ONE_DEGREE = PI/180;
const double SPEED_MODIFIER = 2;

struct Input {
    SDL_Event *event;
    Camera *cam;
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
    Input(SDL_Event *event,Camera *cam) {
        this->event = event;
        this->cam = cam;
    }
    bool handleUserInput();
    void lockAngles();
    void handleCameraMovement();
};
