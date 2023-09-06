#include "main.h"
#include "input.h"
#include "render.h"

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