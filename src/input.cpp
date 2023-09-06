#include "input.h"

void handleUserInput(SDL_Event event, Input *movement) {
    switch(event.key.keysym.sym) {
        case 'w':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->forward = true;
                break;
                case SDL_KEYUP:
                    movement->forward = false;
                break;
            }
        break;
        case 's':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->backward = true;
                break;
                case SDL_KEYUP:
                    movement->backward = false;
                break;
            }
        break;
        case 'a':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->strafe_left = true;
                break;
                case SDL_KEYUP:
                    movement->strafe_left = false;
                break;
            }
        break;
        case 'd':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->strafe_right = true;
                break;
                case SDL_KEYUP:
                    movement->strafe_right = false;
                break;
            }
        break;
        case ' ':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->up = true;
                break;
                case SDL_KEYUP:
                    movement->up = false;
                break;
            }
        break;
        case 'c':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->down = true;
                break;
                case SDL_KEYUP:
                    movement->down = false;
                break;
            }
        break;
        case 'l':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->look_left = true;
                break;
                case SDL_KEYUP:
                    movement->look_left = false;
                break;
            }
        break;
        case '\'':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->look_right = true;
                break;
                case SDL_KEYUP:
                    movement->look_right = false;
                break;
            }
        break;
        case 'p':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->look_up = true;
                break;
                case SDL_KEYUP:
                    movement->look_up = false;
                break;
            }
        break;
        case ';':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->look_down = true;
                break;
                case SDL_KEYUP:
                    movement->look_down = false;
                break;
            }
        break;
    }

}

void handleCameraMovement(Input *movement, Camera *cam) {
    if(movement->strafe_left) {
        cam->cz += 0.5*cos(cam->ty - PI/2.0);
        cam->cx += 0.5*sin(cam->ty - PI/2.0);
    }
    if(movement->strafe_right) {
        cam->cz += 0.5*cos(cam->ty + PI/2.0);
        cam->cx += 0.5*sin(cam->ty + PI/2.0);
    }
    if(movement->forward) {
        cam->cz += 0.5*cos(cam->ty); 
        cam->cx += 0.5*sin(cam->ty);
    }
    if(movement->backward) {
        cam->cz -= 0.5*cos(cam->ty); 
        cam->cx -= 0.5*sin(cam->ty);
    }
    if(movement->up) {
        cam->cy += 0.5;
    }
    if(movement->down) {
        cam->cy -= 0.5;
    }
    if(movement->look_left) {
        cam->ty -= 1.5*ONE_DEGREE;
    }
    if(movement->look_right) {
        cam->ty += 1.5*ONE_DEGREE;
    }
    if(movement->look_up) {
        cam->tx -= 1.5*ONE_DEGREE;
    }
    if(movement->look_down) {
        cam-> tx += 1.5*ONE_DEGREE;
    }
}