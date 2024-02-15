#include "main.hpp"
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

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_Window* pWindow = SDL_CreateWindow("hi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        BUFFER_WIDTH, BUFFER_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer* pRenderer = SDL_CreateRenderer(pWindow, 0, SDL_RENDERER_SOFTWARE);
    SDL_RenderSetVSync(pRenderer, 1);

    Camera cam(300, BUFFER_HEIGHT, BUFFER_WIDTH);

    SDL_Event event;
    Input input(&event, &cam);

    Render render3D(&cam, pRenderer);

    //render3D.addMesh("cube.obj", Vec3(0,59,72));
    //render3D.addMesh("monkey.obj", Vec3(0,0,100), 1.0);
    //render3D.addMesh("sphere.obj",Vec3(0,-50,200), 1.0);
    //render3D.addMesh("plane.obj", Vec3(0,-100,0), 10.0);

    //render3D.addMesh("snake.obj", Vec3(0,-200,100), 2.4);
    //render3D.addMesh("plane.obj", Vec3(0,-200,100), 40.0);

    render3D.addMesh("measure_uv.obj", Vec3(0,0,30), 3.0);
    render3D.addTexture("chiyo-hanging.data", 643, 485);

    //render3D.addMesh("single.obj", Vec3(0,0,30), 1.0);
    

    UIElement fps(pRenderer, 0,0);
    
    double deltaTime = 0.0;
    // main loop
    while (true) {
        chrono::time_point<chrono::steady_clock> timeStart = chrono::steady_clock::now();
        render3D.clearBuffer();
        SDL_RenderClear(pRenderer);

        if(!input.handleUserInput()) {break;}
        input.handleCameraMovement(deltaTime);

        render3D.fillBuffer();
        render3D.copyBuffer();

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
    render3D.exit();
    return 0;
}