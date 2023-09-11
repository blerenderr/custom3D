#include "input.h"

const double SPEED_MODIFIER = 1;

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
        cam->cz += 1.5*SPEED_MODIFIER*cos(cam->ty - PI/2.0);
        cam->cx += 1.5*SPEED_MODIFIER*sin(cam->ty - PI/2.0);
    }
    if(movement->strafe_right) {
        cam->cz += 1.5*SPEED_MODIFIER*cos(cam->ty + PI/2.0);
        cam->cx += 1.5*SPEED_MODIFIER*sin(cam->ty + PI/2.0);
    }
    if(movement->forward) {
        cam->cz += SPEED_MODIFIER*cos(cam->ty); 
        cam->cx += SPEED_MODIFIER*sin(cam->ty);
    }
    if(movement->backward) {
        cam->cz -= SPEED_MODIFIER*cos(cam->ty); 
        cam->cx -= SPEED_MODIFIER*sin(cam->ty);
    }
    if(movement->up) {
        cam->cy += SPEED_MODIFIER;
    }
    if(movement->down) {
        cam->cy -= SPEED_MODIFIER;
    }
    if(movement->look_left) {
        cam->ty -= 2.4*SPEED_MODIFIER*ONE_DEGREE;
    }
    if(movement->look_right) {
        cam->ty += 2.4*SPEED_MODIFIER*ONE_DEGREE;
    }
    if(movement->look_up) {
        cam->tx -= 2.0*SPEED_MODIFIER*ONE_DEGREE;
    }
    if(movement->look_down) {
        cam-> tx += 2.0*SPEED_MODIFIER*ONE_DEGREE;
    }
}