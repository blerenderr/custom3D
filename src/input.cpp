#include "input.h"

bool Input::handleUserInput() {
    if(SDL_PollEvent(event)) {
        if(event->type == SDL_QUIT) {return false;}
        switch(event->key.keysym.sym) {
            case 'q':
                return false;
            case 'w':
                switch(event->type) {
                    case SDL_KEYDOWN:
                        forward = true;
                    break;
                    case SDL_KEYUP:
                        forward = false;
                    break;
                }
            break;
            case 's':
                switch(event->type) {
                    case SDL_KEYDOWN:
                        backward = true;
                    break;
                    case SDL_KEYUP:
                        backward = false;
                    break;
                }
            break;
            case 'a':
                switch(event->type) {
                    case SDL_KEYDOWN:
                        strafe_left = true;
                    break;
                    case SDL_KEYUP:
                        strafe_left = false;
                    break;
                }
            break;
            case 'd':
                switch(event->type) {
                    case SDL_KEYDOWN:
                        strafe_right = true;
                    break;
                    case SDL_KEYUP:
                        strafe_right = false;
                    break;
                }
            break;
            case ' ':
                switch(event->type) {
                    case SDL_KEYDOWN:
                        up = true;
                    break;
                    case SDL_KEYUP:
                        up = false;
                    break;
                }
            break;
            case 'c':
                switch(event->type) {
                    case SDL_KEYDOWN:
                        down = true;
                    break;
                    case SDL_KEYUP:
                        down = false;
                    break;
                }
            break;
            case 'l':
                switch(event->type) {
                    case SDL_KEYDOWN:
                        look_left = true;
                    break;
                    case SDL_KEYUP:
                        look_left = false;
                    break;
                }
            break;
            case '\'':
                switch(event->type) {
                    case SDL_KEYDOWN:
                        look_right = true;
                    break;
                    case SDL_KEYUP:
                        look_right = false;
                    break;
                }
            break;
            case 'p':
                switch(event->type) {
                    case SDL_KEYDOWN:
                        look_up = true;
                    break;
                    case SDL_KEYUP:
                        look_up = false;
                    break;
                }
            break;
            case ';':
                switch(event->type) {
                    case SDL_KEYDOWN:
                        look_down = true;
                    break;
                    case SDL_KEYUP:
                        look_down = false;
                    break;
                }
            break;
        }
    }
    return true;

}

void Input::lockAngles() {
    if(cam->tx > PI/2) {cam->tx = PI/2;}
    if(cam->tx < -PI/2) {cam->tx = -PI/2;}
    if(cam->ty >= 2*PI) {cam->ty -= 2*PI;}
    if(cam->ty < 0) {cam->ty += 2*PI;}
}

void Input::handleCameraMovement() {
    bool changed = false;
    if(strafe_left) {
        cam->cz += 1.5*SPEED_MODIFIER*cos(cam->ty - PI/2.0);
        cam->cx += 1.5*SPEED_MODIFIER*sin(cam->ty - PI/2.0);
        changed = true;
    }
    if(strafe_right) {
        cam->cz += 1.5*SPEED_MODIFIER*cos(cam->ty + PI/2.0);
        cam->cx += 1.5*SPEED_MODIFIER*sin(cam->ty + PI/2.0);
        changed = true;
    }
    if(forward) {
        cam->cz += SPEED_MODIFIER*cos(cam->ty); 
        cam->cx += SPEED_MODIFIER*sin(cam->ty);
        changed = true;
    }
    if(backward) {
        cam->cz -= SPEED_MODIFIER*cos(cam->ty); 
        cam->cx -= SPEED_MODIFIER*sin(cam->ty);
        changed = true;
    }
    if(up) {
        cam->cy += SPEED_MODIFIER;
        changed = true;
    }
    if(down) {
        cam->cy -= SPEED_MODIFIER;
        changed = true;
    }
    if(look_left) {
        cam->ty -= 2.4*SPEED_MODIFIER*ONE_DEGREE;
        changed = true;
    }
    if(look_right) {
        cam->ty += 2.4*SPEED_MODIFIER*ONE_DEGREE;
        changed = true;
    }
    if(look_up) {
        cam->tx -= 2.0*SPEED_MODIFIER*ONE_DEGREE;
        changed = true;
    }
    if(look_down) {
        cam->tx += 2.0*SPEED_MODIFIER*ONE_DEGREE;
        changed = true;
    }
    lockAngles();
    if(changed) {
        cam->updateUnitVectors();
    }
}