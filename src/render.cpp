#include "render.h"

void Render::addMesh(std::string filename) {
    meshes.insert(meshes.end(), Mesh(filename));
}
void Render::addMesh(std::string filename, Vec3 origin) {
    meshes.insert(meshes.end(), Mesh(filename, origin));
}
void Render::drawLine(int x1, int y1, int x2, int y2) {
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
void Render::constructMatrix() {
    double cx = cam->cx; double cy = cam->cy; double cz = cam->cz;
    double tx = cam->tx; double ty = cam->ty; double tz = cam->tz;
    double ex = cam->ex; double ey = cam->ey; double ez = cam->ez;

    for(int m = 0; m < meshes.size(); m++) {
        Mesh *pCurrentMesh = &meshes.at(m);

        for(int p = 0; p < pCurrentMesh->indices.size(); p++) {
            Vec3 *pCurrentPoly = &pCurrentMesh->indices.at(p);
            int ax; int ay; int az;
            int dx; int dy; int dz;
            double bx; double by;
            int dispX; int dispY;
            int aDispX = -1; int bDispX = -1; int cDispX = -1;
            int aDispY = -1; int bDispY = -1; int cDispY = -1;
            // FIRST VERTEX
            ax = pCurrentMesh->vertices.at(pCurrentPoly->x-1).x + pCurrentMesh->origin.x;
            ay = pCurrentMesh->vertices.at(pCurrentPoly->x-1).y + pCurrentMesh->origin.y;
            az = pCurrentMesh->vertices.at(pCurrentPoly->x-1).z + pCurrentMesh->origin.z;
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
            ax = pCurrentMesh->vertices.at(pCurrentPoly->y-1).x + pCurrentMesh->origin.x;
            ay = pCurrentMesh->vertices.at(pCurrentPoly->y-1).y + pCurrentMesh->origin.y;
            az = pCurrentMesh->vertices.at(pCurrentPoly->y-1).z + pCurrentMesh->origin.z;
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
            ax = pCurrentMesh->vertices.at(pCurrentPoly->z-1).x + pCurrentMesh->origin.x;
            ay = pCurrentMesh->vertices.at(pCurrentPoly->z-1).y + pCurrentMesh->origin.y;
            az = pCurrentMesh->vertices.at(pCurrentPoly->z-1).z + pCurrentMesh->origin.z;
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
                drawLine(aDispX, aDispY, bDispX, bDispY);
                drawLine(bDispX, bDispY, cDispX, cDispY);
                drawLine(cDispX, cDispY, aDispX, aDispY);
            }
        }
    }
}
void Render::drawMatrix() {
    SDL_SetRenderDrawColor(pRenderer, 255,255,255,0);
    for(int i = 0; i < SCREEN_HEIGHT; i++) {
        for(int j = 0; j < SCREEN_WIDTH; j++) {
            if(displaySurface[i][j]) {
                SDL_RenderDrawPoint(pRenderer, j, i);
            }   
        }   
    }
}