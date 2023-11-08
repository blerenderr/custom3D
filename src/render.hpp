#include <SDL_render.h>
#include <math.h>
#include "model.hpp"
#include "camera.hpp"

#pragma once

const int SCREEN_WIDTH = 1680;
const int SCREEN_HEIGHT = 1000;
const int MATRIX_TO_WINDOW_BORDER = 500; // margin of window overlapping matrix in pixels
const int MATRIX_WIDTH = SCREEN_WIDTH + MATRIX_TO_WINDOW_BORDER*2;
const int MATRIX_HEIGHT = SCREEN_HEIGHT + MATRIX_TO_WINDOW_BORDER*2;

struct Render {
    Camera *cam;
    std::vector<Mesh> meshes;
    SDL_Renderer *pRenderer;
    bool (*displayMatrix)[MATRIX_WIDTH];
    Render(Camera *cam, SDL_Renderer *pRenderer, bool displayMatrix[MATRIX_HEIGHT][MATRIX_WIDTH]) {
        this->cam = cam; this->pRenderer = pRenderer; this->displayMatrix = displayMatrix;
        meshes = std::vector<Mesh>();
    }
    void addMesh(std::string filename);
    void addMesh(std::string filename, Vec3 origin);
    void addMesh(std::string filename, Vec3 origin, double scale);
    bool isVisible(Vec3 poly[3]);
    void bresenhamLow(int x1, int y1, int x2, int y2);
    void bresenhamHigh(int x1, int y1, int x2, int y2);
    void drawLine(int x1, int y1, int x2, int y2);
    void constructMatrix();
    void drawMatrix();
    void clearViewableMatrix();

};