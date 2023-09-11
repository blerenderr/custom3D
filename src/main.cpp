#include "main.h"
#include <chrono>
using namespace std;

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

int main(int argc, char *argv[]) {
    // sdl init
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Window *pWindow = SDL_CreateWindow("hi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, 0, SDL_RENDERER_SOFTWARE);

    // input init
    SDL_Event event;
    Input movement;

    // render init
    Camera cam;
    cam.cx = 0.0; cam.cy = 0.0; cam.cz = 0.0;
    cam.tx = 0.0; cam.ty = 0.0; cam.tz = 0.0;
    cam.ex = 0.0; cam.ey = 0.0; cam.ez = 150.0; // dist. of display surface from camera point
    bool displaySurface[SCREEN_HEIGHT][SCREEN_WIDTH];

    Render render3D(&cam, pRenderer, displaySurface);

    render3D.addMesh("monkey.obj", Vec3(0,0,50));
    
    // main loop
    while (true) {
        chrono::time_point<chrono::steady_clock> timeStart = chrono::steady_clock::now();
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
        render3D.constructMatrix();
        render3D.drawMatrix();
        SDL_RenderPresent(pRenderer);
        chrono::time_point<chrono::steady_clock> timeEnd = chrono::steady_clock::now();
        this_thread::sleep_for(chrono::milliseconds(16)-(timeEnd-timeStart));
	}

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}