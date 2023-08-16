#include <SDL_events.h>
#include <SDL_rect.h>
#include <iostream>
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_quit.h>
#include <thread>
#include <chrono>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int VERTS_ARR_SIZE = 8;
const int POLYS_ARR_SIZE = 2;
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
    // bool look_up;
    // bool look_down;
};

struct Camera {
    //position
    double cx; double cy; double cz;
    //rotation
    double tx; double ty; double tz;
    //disp. surface pos (relative)
    double ex; double ey; double ez;
};

void printDriverInfo() {
    for(int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        SDL_RendererInfo info;
        SDL_GetRenderDriverInfo(i, &info);
        cout << info.name << endl
        << info.flags << endl
        << info.num_texture_formats << endl
        << *info.texture_formats << endl
        << info.max_texture_width << endl
        << info.max_texture_height << endl << endl;
    }
}

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
        case ',':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->look_left = true;
                break;
                case SDL_KEYUP:
                    movement->look_left = false;
                break;
            }
        break;
        case '.':
            switch(event.type) {
                case SDL_KEYDOWN:
                    movement->look_right = true;
                break;
                case SDL_KEYUP:
                    movement->look_right = false;
                break;
            }
        break;
    }

}

void handleCameraMovement(Input *movement, Camera *cam) {
    if(movement->strafe_left) {
        cam->cx -= 0.5;
    }
    if(movement->strafe_right) {
        cam->cx += 0.5;
    }
    if(movement->forward) {
        cam->cz += 0.1;
    }
    if(movement->backward) {
        cam->cz -= 0.1;
    }
    if(movement->up) {
        cam->cy += 0.5;
    }
    if(movement->down) {
        cam->cy -= 0.5;
    }
    if(movement->look_left) {
        cam->ty -= ONE_DEGREE;
    }
    if(movement->look_right) {
        cam-> ty += ONE_DEGREE;
    }
}

void constructMatrix(Camera *cam, Vec3 verts[], bool displaySurface[][SCREEN_WIDTH]) {
    double cx = cam->cx; double cy = cam->cy; double cz = cam->cz;
    double tx = cam->tx; double ty = cam->ty; double tz = cam->tz;
    double ex = cam->ex; double ey = cam->ey; double ez = cam->ez;
    //cout << "field of view (rad): " << 2*atan(1.0/ez) << endl;

    for(int i = 0; i < VERTS_ARR_SIZE; i++) {
        int ax = verts[i].x; int ay = verts[i].y; int az = verts[i].z;
        double dx = cos(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx)) - sin(ty)*(az-cz);
        double dy = sin(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) + cos(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
        double dz = cos(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) - sin(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
        //cout << "dx: " << dx << " dy: " << dy << " dz: " << dz << endl;

        double bx = (ez/dz)*dx + ex;
        double by = (ez/dz)*dy + ey;
        //cout << "bx: " << bx << " by: " << by << endl;

        int dispX = bx + (SCREEN_WIDTH / 2.0);
        int dispY = -by + (SCREEN_HEIGHT / 2.0);
        if((dispX >= 0 && dispX <= SCREEN_WIDTH) && (dispY >= 0 && dispY <= SCREEN_HEIGHT)) {
            displaySurface[dispY][dispX] = true;
        }
    }
}

void drawMatrix(SDL_Renderer *pRenderer, bool displaySurface[][SCREEN_WIDTH]) {
    SDL_SetRenderDrawColor(pRenderer, 255,255,255,0);
    for(int i = 0; i < SCREEN_HEIGHT; i++) {
        for(int j = 0; j < SCREEN_WIDTH; j++) {
            if(displaySurface[i][j]) {
                SDL_RenderDrawPoint(pRenderer, j, i);
                SDL_RenderDrawPoint(pRenderer, j+1, i+1);
                SDL_RenderDrawPoint(pRenderer, j+1, i);
                SDL_RenderDrawPoint(pRenderer, j, i+1);
            }   
        }   
    }
    
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Window *pWindow = SDL_CreateWindow("hi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, 0, SDL_RENDERER_SOFTWARE);

    Input movement;
    Camera cam;
    cam.cx = 0.0; cam.cy = 0.0; cam.cz = 0.0;
    cam.tx = 0.0; cam.ty = 0.0; cam.tz = 0.0;
    cam.ex = 0.0; cam.ey = 0.0; cam.ez = 50.0;
    
    bool displaySurface[SCREEN_HEIGHT][SCREEN_WIDTH];

    Vec3 verts[VERTS_ARR_SIZE];
    verts[0].x = 10; verts[0].y = 10; verts[0].z = 10;
    verts[1].x = 20; verts[1].y = 10; verts[1].z = 10;
    verts[2].x = 10; verts[2].y = 20; verts[2].z = 10;
    verts[3].x = 20; verts[3].y = 20; verts[3].z = 10;
    verts[4].x = 10; verts[4].y = 10; verts[4].z = 15;
    verts[5].x = 20; verts[5].y = 10; verts[5].z = 15;
    verts[6].x = 10; verts[6].y = 20; verts[6].z = 15;
    verts[7].x = 20; verts[7].y = 20; verts[7].z = 15;


	SDL_Event event;
    while (true) {
        for(int i = 0; i < SCREEN_HEIGHT; i++) {
            for(int j = 0; j < SCREEN_WIDTH; j++) {
                displaySurface[i][j] = false;
            }
        }
        SDL_SetRenderDrawColor(pRenderer, 0,0,0,0);
        SDL_RenderClear(pRenderer);

		if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
				break;
			}
            handleUserInput(event, &movement);
		}
        handleCameraMovement(&movement, &cam);

        constructMatrix(&cam, verts, displaySurface);
        drawMatrix(pRenderer, displaySurface);
        SDL_RenderPresent(pRenderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}