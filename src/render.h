#include <SDL.h>
#include <SDL_video.h>
#include <SDL_render.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int VERTS_ARR_SIZE = 8;
const int POLYS_ARR_SIZE = 12;

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

void lineDrawHelper(int x1, int y1, int x2, int y2, bool displaySurface[][SCREEN_WIDTH]);

void constructMatrix(Camera *cam, Poly polys[], bool displaySurface[][SCREEN_WIDTH]);

void drawMatrix(SDL_Renderer *pRenderer, bool displaySurface[][SCREEN_WIDTH]);