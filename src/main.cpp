#include "main.h"
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

    Camera cam(300, SCREEN_HEIGHT, SCREEN_WIDTH);

    SDL_Event event;
    Input input(&event, &cam);
    bool displayMatrix[MATRIX_HEIGHT][MATRIX_WIDTH];

    Render render3D(&cam, pRenderer, displayMatrix);

    //render3D.addMesh("cube.obj", Vec3(0,59,72));
    //render3D.addMesh("monkey.obj", Vec3(0,0,100), 1.0);
    //render3D.addMesh("sphere.obj",Vec3(0,-50,200), 1.0);
    //render3D.addMesh("plane.obj", Vec3(0,-100,0), 10.0);

    render3D.addMesh("snake.obj", Vec3(0,-200,100), 2.4);
    render3D.addMesh("plane.obj", Vec3(0,-200,100), 40.0);

    //render3D.addMesh("measure_uv.obj", Vec3(0,0,10), 1.0);

    

    UIElement fps(pRenderer, 0,0);
    
    double deltaTime = 0.0;
    // main loop
    while (true) {
        chrono::time_point<chrono::steady_clock> timeStart = chrono::steady_clock::now();
        render3D.clearViewableMatrix();
        SDL_SetRenderDrawColor(pRenderer, 64,64,64,0);
        SDL_RenderClear(pRenderer);

        if(!input.handleUserInput()) {break;}
        input.handleCameraMovement(deltaTime);

        render3D.constructMatrix();
        render3D.drawMatrix();

        fps.setNumber((int)min((1000000000/deltaTime), 60.0));
        fps.draw();

        SDL_RenderPresent(pRenderer);

        chrono::time_point<chrono::steady_clock> timeEnd = chrono::steady_clock::now();
        deltaTime = timeEnd.time_since_epoch().count() - timeStart.time_since_epoch().count();
        this_thread::sleep_for(chrono::microseconds(16666)-(timeEnd-timeStart));
	}

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}