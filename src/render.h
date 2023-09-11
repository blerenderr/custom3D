#include <SDL.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include "model.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

#ifndef CAMERA_STRUCT
#define CAMERA_STRUCT
struct Camera {
    //position
    double cx; double cy; double cz;
    //rotation
    double tx; double ty; double tz;
    //disp. surface pos (relative)
    double ex; double ey; double ez;
};
#endif

struct Render {
    Camera *cam;
    std::vector<Mesh> meshes;
    SDL_Renderer *pRenderer;
    bool (*displaySurface)[SCREEN_WIDTH];
    Render(Camera *cam, SDL_Renderer *pRenderer, bool displaySurface[SCREEN_HEIGHT][SCREEN_WIDTH]) {
        this->cam = cam; this->pRenderer = pRenderer; this->displaySurface = displaySurface;
        meshes = std::vector<Mesh>();
    }
    void addMesh(std::string filename);
    void addMesh(std::string filename, Vec3 origin);
    void drawLine(int x1, int y1, int x2, int y2);
    void constructMatrix();
    void drawMatrix();

};