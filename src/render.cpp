#include "render.h"
using namespace std;

void Render::addMesh(string filename) {
    meshes.insert(meshes.end(), Mesh(filename));
}
void Render::addMesh(string filename, Vec3 origin) {
    meshes.insert(meshes.end(), Mesh(filename, origin));
}
bool Render::isVisible(vector<Vec3> poly) {
    for(int i = 0; i < poly.size(); i++) {
        Vec3 pointPos = poly.at(i);
        Vec3 pointRel;
        Vec3 camToPoint = pointPos.subtract(Vec3(cam->cx,cam->cy,cam->cz));
        pointRel.z = camToPoint.dotProduct(cam->zDir);
        pointRel.y = camToPoint.dotProduct(cam->yDir);
        pointRel.x = camToPoint.dotProduct(cam->xDir);
        if(pointRel.z > 500 || pointRel.z < 10) {
            return false;
        }
        double height = pointRel.z*2*tan(cam->xFov*0.5);
        if(pointRel.y > -height/2 && pointRel.y < height/2) {
            return true;
        }
        double width = pointRel.z*2*tan(cam->yFov*0.5);
        if(pointRel.x > -width/2 && pointRel.x < width/2) {
            return true;
        }

    }

    return false;
}
void Render::drawLine(int x1, int y1, int x2, int y2) {
    int delX = x2 - x1; int delY = y2 - y1;
    int relX = 0; int relY = 0;
    if(delX != 0) {
        float slope = (float)delY/delX;
        while(relX != delX && ((slope <= 1 && slope > -1) || (slope >= 1 && slope < -1))) {
            relY = slope*relX;
            displayMatrix[y1+relY][x1+relX] = true;
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
            displayMatrix[y1+relY][x1+relX] = true;
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
            displayMatrix[y1+relY][x1+relX] = true;
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
            // formula variables
            double ax; double ay; double az;
            double dx; double dy; double dz;
            double bx; double by;
            int dispX; int dispY;
            // end formula variables
            int aDispX = -1; int bDispX = -1; int cDispX = -1;
            int aDispY = -1; int bDispY = -1; int cDispY = -1;

            vector<Vec3> poly;
            poly.insert(poly.end(),Vec3(pCurrentMesh->vertices.at(pCurrentPoly->x-1).x + pCurrentMesh->origin.x,
                                                    pCurrentMesh->vertices.at(pCurrentPoly->x-1).y + pCurrentMesh->origin.y,
                                                    pCurrentMesh->vertices.at(pCurrentPoly->x-1).z + pCurrentMesh->origin.z));

            poly.insert(poly.end(),Vec3(pCurrentMesh->vertices.at(pCurrentPoly->y-1).x + pCurrentMesh->origin.x,
                                                    pCurrentMesh->vertices.at(pCurrentPoly->y-1).y + pCurrentMesh->origin.y,
                                                    pCurrentMesh->vertices.at(pCurrentPoly->y-1).z + pCurrentMesh->origin.z));

            poly.insert(poly.end(),Vec3(pCurrentMesh->vertices.at(pCurrentPoly->z-1).x + pCurrentMesh->origin.x,
                                                    pCurrentMesh->vertices.at(pCurrentPoly->z-1).y + pCurrentMesh->origin.y,
                                                    pCurrentMesh->vertices.at(pCurrentPoly->z-1).z + pCurrentMesh->origin.z));

            if(isVisible(poly)) {
                // FIRST VERTEX
                ax = poly.at(0).x;
                ay = poly.at(0).y;
                az = poly.at(0).z;
                dx = cos(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx)) - sin(ty)*(az-cz);
                dy = sin(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) + cos(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
                dz = cos(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) - sin(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));

                bx = (ez/dz)*dx + ex;
                by = (ez/dz)*dy + ey;

                dispX = bx + (MATRIX_WIDTH / 2.0);
                dispY = -by + (MATRIX_HEIGHT / 2.0);
                if((dispX >= 0 && dispX <= MATRIX_WIDTH) && (dispY >= 0 && dispY <= MATRIX_HEIGHT)) {
                    aDispX = dispX; aDispY = dispY;
                }

                // SECOND VERTEX
                ax = poly.at(1).x;
                ay = poly.at(1).y;
                az = poly.at(1).z;
                dx = cos(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx)) - sin(ty)*(az-cz);
                dy = sin(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) + cos(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
                dz = cos(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) - sin(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));

                bx = (ez/dz)*dx + ex;
                by = (ez/dz)*dy + ey;

                dispX = bx + (MATRIX_WIDTH / 2.0);
                dispY = -by + (MATRIX_HEIGHT / 2.0);
                if((dispX >= 0 && dispX <= MATRIX_WIDTH) && (dispY >= 0 && dispY <= MATRIX_HEIGHT)) {
                    bDispX = dispX; bDispY = dispY;
                }

                // THIRD VERTEX
                ax = poly.at(2).x;
                ay = poly.at(2).y;
                az = poly.at(2).z;
                dx = cos(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx)) - sin(ty)*(az-cz);
                dy = sin(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) + cos(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
                dz = cos(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) - sin(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));

                bx = (ez/dz)*dx + ex;
                by = (ez/dz)*dy + ey;

                dispX = bx + (MATRIX_WIDTH / 2.0);
                dispY = -by + (MATRIX_HEIGHT / 2.0);
                if((dispX >= 0 && dispX <= MATRIX_WIDTH) && (dispY >= 0 && dispY <= MATRIX_HEIGHT)) {
                    cDispX = dispX; cDispY = dispY;
                }

                if(aDispX != -1 && aDispY != -1 && bDispX != -1 && bDispY != -1 && cDispX != -1 && cDispY != -1) { // lol
                    drawLine(aDispX, aDispY, bDispX, bDispY);
                    drawLine(bDispX, bDispY, cDispX, cDispY);
                    drawLine(cDispX, cDispY, aDispX, aDispY);
                }
            }
        }
    }
}
void Render::drawMatrix() {
    SDL_SetRenderDrawColor(pRenderer, 255,255,255,0);
    for(int i = MATRIX_TO_WINDOW_BORDER; i < SCREEN_HEIGHT+MATRIX_TO_WINDOW_BORDER; i++) {
        for(int j = MATRIX_TO_WINDOW_BORDER; j < SCREEN_WIDTH+MATRIX_TO_WINDOW_BORDER; j++) {
            if(displayMatrix[i][j]) {
                SDL_RenderDrawPoint(pRenderer, j-MATRIX_TO_WINDOW_BORDER, i-MATRIX_TO_WINDOW_BORDER);
            }
        }
    }
}