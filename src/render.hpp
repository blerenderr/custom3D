#include <SDL_render.h>
#include <iostream>
#include "mesh.hpp"
#include "texture.hpp"
#include "camera.hpp"

#pragma once

const int BUFFER_WIDTH = 1680; // 1680
const int BUFFER_HEIGHT = 1050; // 1000

typedef unsigned char BYTE;

enum TRI_TYPE {
    GENERAL_LEFT = 0,
    GENERAL_RIGHT = 1,
    FLAT_TOP = 2,
    FLAT_BOTTOM = 3
};

struct Render {
    Camera *cam;
    std::vector<Mesh> meshes;
    std::vector<Texture> textures;
    SDL_Renderer* pRenderer;
    SDL_Texture* bufferTexture;
    BYTE* frameBuffer;
    Render(Camera* cam, SDL_Renderer* pRenderer) {
        this->cam = cam; this->pRenderer = pRenderer;
        bufferTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING, BUFFER_WIDTH, BUFFER_HEIGHT);
        meshes = std::vector<Mesh>();
        textures = std::vector<Texture>();

        frameBuffer = new BYTE[BUFFER_HEIGHT*BUFFER_WIDTH*3];
    }
    void addMesh(std::string filename);
    void addMesh(std::string filename, Vec3 origin);
    void addMesh(std::string filename, Vec3 origin, double scale);
    void addTexture(std::string filename, int width, int height);

    bool isVisible(Vec3 poly[3]);
    int writeBuffer(int x, int y, BYTE r, BYTE g, BYTE b);
    void readBuffer(int x, int y, BYTE* r, BYTE* g, BYTE* b);
    void bresenhamLow(int x1, int y1, int x2, int y2);
    void bresenhamHigh(int x1, int y1, int x2, int y2);
    void drawLine(int x1, int y1, int x2, int y2);
    void fillPoly(Vec2 screenVerts[3], Vec2 uvCoords[3]);

    void fillBuffer();
    void copyBuffer();
    void clearBuffer();

    void exit();
};