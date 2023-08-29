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
        cam->ty -= 2.0*ONE_DEGREE;
    }
    if(movement->look_right) {
        cam->ty += 2.0*ONE_DEGREE;
    }
}
// precondition: points are gaurenteed to be in range of displaySurface.
void lineDrawHelper(int x1, int y1, int x2, int y2, bool displaySurface[][SCREEN_WIDTH]) {
    int delX = x2 - x1; int delY = y2 - y1;
    int relX = 0; int relY = 0;
    if(delX != 0) {
        float slope = (float)delY/delX;
        while(relX != delX && ((slope <= 1 && slope > -1) || (slope >= 1 && slope < -1))) {
            relY = slope*relX;
            displaySurface[y1+relY][x1+relX] = true;
            if(delX < 0) {
                relX--;
            }
            else {
                relX++;
            }
        }
        // not sure why this trickery works but it does
        // there might be some lines drawn twice which should be fixed later
        while(relY != delY && ((slope >= -1 || slope < 1) || (slope <= -1 || slope > 1))) {
            relX = 1/slope*relY;
            displaySurface[y1+relY][x1+relX] = true;
            if(delY < 0) {
                relY--;
            }
            else {
                relY++;
            }
        }
    }
    else { // vertical line
        while(relY != delY) {
            displaySurface[y1+relY][x1+relX] = true;
            if(delY < 0) {
                relY--;
            }
            else {
                relY++;
            }
        }
    }

}

void constructMatrix(Camera *cam, Poly polys[], bool displaySurface[][SCREEN_WIDTH]) {
    double cx = cam->cx; double cy = cam->cy; double cz = cam->cz;
    double tx = cam->tx; double ty = cam->ty; double tz = cam->tz;
    double ex = cam->ex; double ey = cam->ey; double ez = cam->ez;

    for(int p = 0; p < POLYS_ARR_SIZE; p++) {
        Poly *pCurrentPoly = &polys[p];
        int ax; int ay; int az;
        int dx; int dy; int dz;
        double bx; double by;
        int dispX; int dispY;
        int aDispX = -1; int bDispX = -1; int cDispX = -1;
        int aDispY = -1; int bDispY = -1; int cDispY = -1;
        // FIRST VERTEX
        ax = pCurrentPoly->a.x; ay = pCurrentPoly->a.y; az = pCurrentPoly->a.z;
        dx = cos(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx)) - sin(ty)*(az-cz);
        dy = sin(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) + cos(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
        dz = cos(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) - sin(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));

        bx = (ez/dz)*dx + ex;
        by = (ez/dz)*dy + ey;

        dispX = bx + (SCREEN_WIDTH / 2.0);
        dispY = -by + (SCREEN_HEIGHT / 2.0);
        if((dispX >= 0 && dispX <= SCREEN_WIDTH) && (dispY >= 0 && dispY <= SCREEN_HEIGHT)) {
            displaySurface[dispY][dispX] = true;
            aDispX = dispX; aDispY = dispY;
        }
        // SECOND VERTEX
        ax = pCurrentPoly->b.x; ay = pCurrentPoly->b.y; az = pCurrentPoly->b.z;
        dx = cos(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx)) - sin(ty)*(az-cz);
        dy = sin(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) + cos(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
        dz = cos(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) - sin(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));

        bx = (ez/dz)*dx + ex;
        by = (ez/dz)*dy + ey;

        dispX = bx + (SCREEN_WIDTH / 2.0);
        dispY = -by + (SCREEN_HEIGHT / 2.0);
        if((dispX >= 0 && dispX <= SCREEN_WIDTH) && (dispY >= 0 && dispY <= SCREEN_HEIGHT)) {
            displaySurface[dispY][dispX] = true;
            bDispX = dispX; bDispY = dispY;
        }
        // THIRD VERTEX
        ax = pCurrentPoly->c.x; ay = pCurrentPoly->c.y; az = pCurrentPoly->c.z;
        dx = cos(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx)) - sin(ty)*(az-cz);
        dy = sin(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) + cos(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
        dz = cos(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) - sin(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));

        bx = (ez/dz)*dx + ex;
        by = (ez/dz)*dy + ey;

        dispX = bx + (SCREEN_WIDTH / 2.0);
        dispY = -by + (SCREEN_HEIGHT / 2.0);
        if((dispX >= 0 && dispX <= SCREEN_WIDTH) && (dispY >= 0 && dispY <= SCREEN_HEIGHT)) {
            displaySurface[dispY][dispX] = true;
            cDispX = dispX; cDispY = dispY;
        }

        // draw lines from a to b to c to a
        if(aDispX != -1 && aDispY != -1 && bDispX != -1 && bDispY != -1 && cDispX != -1 && cDispY != -1) { // lol
            lineDrawHelper(aDispX, aDispY, bDispX, bDispY, displaySurface);
            lineDrawHelper(bDispX, bDispY, cDispX, cDispY, displaySurface);
            lineDrawHelper(cDispX, cDispY, aDispX, aDispY, displaySurface);
        }
    }
}

void drawMatrix(SDL_Renderer *pRenderer, bool displaySurface[][SCREEN_WIDTH]) {
    SDL_SetRenderDrawColor(pRenderer, 255,255,255,0);
    for(int i = 0; i < SCREEN_HEIGHT; i++) {
        for(int j = 0; j < SCREEN_WIDTH; j++) {
            if(displaySurface[i][j]) {
                SDL_RenderDrawPoint(pRenderer, j, i);
                // SDL_RenderDrawPoint(pRenderer, j+1, i+1);
                // SDL_RenderDrawPoint(pRenderer, j+1, i);
                // SDL_RenderDrawPoint(pRenderer, j, i+1);
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
    cam.ex = 0.0; cam.ey = 0.0; cam.ez = 150.0;
    
    bool displaySurface[SCREEN_HEIGHT][SCREEN_WIDTH];
    // construct a cube
    Vec3 verts[VERTS_ARR_SIZE];
    verts[0].x = 10; verts[0].y = 10; verts[0].z = 10;
    verts[1].x = 20; verts[1].y = 10; verts[1].z = 10;
    verts[2].x = 10; verts[2].y = 20; verts[2].z = 10;
    verts[3].x = 20; verts[3].y = 20; verts[3].z = 10;
    verts[4].x = 10; verts[4].y = 10; verts[4].z = 20;
    verts[5].x = 20; verts[5].y = 10; verts[5].z = 20;
    verts[6].x = 10; verts[6].y = 20; verts[6].z = 20;
    verts[7].x = 20; verts[7].y = 20; verts[7].z = 20;

    Poly polys[POLYS_ARR_SIZE];
    polys[0].a = verts[0]; polys[0].b = verts[1]; polys[0].c = verts[3];
    polys[1].a = verts[0]; polys[1].b = verts[2]; polys[1].c = verts[3];
    polys[2].a = verts[5]; polys[2].b = verts[4]; polys[2].c = verts[6];
    polys[3].a = verts[5]; polys[3].b = verts[7]; polys[3].c = verts[6];
    polys[4].a = verts[4]; polys[4].b = verts[0]; polys[4].c = verts[2];
    polys[5].a = verts[4]; polys[5].b = verts[6]; polys[5].c = verts[2];
    polys[6].a = verts[1]; polys[6].b = verts[5]; polys[6].c = verts[7];
    polys[7].a = verts[1]; polys[7].b = verts[3]; polys[7].c = verts[7];
    polys[8].a = verts[2]; polys[8].b = verts[3]; polys[8].c = verts[7];
    polys[9].a = verts[2]; polys[9].b = verts[6]; polys[9].c = verts[7];
    polys[10].a = verts[4]; polys[10].b = verts[5]; polys[10].c = verts[1];
    polys[11].a = verts[4]; polys[11].b = verts[0]; polys[11].c = verts[1];

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

        constructMatrix(&cam, polys, displaySurface);
        drawMatrix(pRenderer, displaySurface);
        SDL_RenderPresent(pRenderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}