#include "render.hpp"
using namespace std;

void Render::addMesh(string filename) {
    meshes.insert(meshes.end(), Mesh(filename));
}
void Render::addMesh(string filename, Vec3 origin) {
    meshes.insert(meshes.end(), Mesh(filename, origin));
}
void Render::addMesh(string filename, Vec3 origin, double scale) {
    meshes.insert(meshes.end(), Mesh(filename, origin, scale));
}
void Render::addTexture(string filename, int width, int height) {
    textures.insert(textures.end(), Texture(filename, width, height));
}
bool Render::isVisible(Vec3 poly[3]) {
    for(int i = 0; i < 3; i++) {
        Vec3 pointPos = poly[i];
        Vec3 pointRel;
        Vec3 camToPoint = pointPos.subtract(Vec3(cam->cx,cam->cy,cam->cz));
        pointRel.z = camToPoint.dotProduct(cam->zDir);
        pointRel.y = camToPoint.dotProduct(cam->yDir);
        pointRel.x = camToPoint.dotProduct(cam->xDir);
        // if one vert is too close or too far, then the whole poly is invalid
        if(pointRel.z > 1500 || pointRel.z < 10) {
            return false;
        }
        // if it fails height check, skip to next vert
        double height = pointRel.z*2*tan(cam->xFov*0.5);
        if(pointRel.y < -height/2 || pointRel.y > height/2) {
            continue;
        }
        // if it fails width check, skip to next vert
        double width = pointRel.z*2*tan(cam->yFov*0.5);
        if(pointRel.x < -width/2 || pointRel.x > width/2) {
            continue;
        }
        // if one vert passes
        return true;
    }
    // if no verts pass
    return false;
}
int Render::writeBuffer(int x, int y, BYTE r, BYTE g, BYTE b) {
    if(x >= BUFFER_WIDTH || y >= BUFFER_HEIGHT || x < 0 || y < 0) {
        return 1;
    }
    if(3*(y*BUFFER_WIDTH + x) + 2 >= BUFFER_WIDTH*BUFFER_HEIGHT*3) {
        return 1;
    }
    frameBuffer[3*(y*BUFFER_WIDTH + x) + 0] = r;
    frameBuffer[3*(y*BUFFER_WIDTH + x) + 1] = g;
    frameBuffer[3*(y*BUFFER_WIDTH + x) + 2] = b;
    return 0;

}
void Render::readBuffer(int x, int y, BYTE* r, BYTE* g, BYTE* b) {
    if(x >= BUFFER_WIDTH || y >= BUFFER_HEIGHT || x < 0 || y < 0) {
        return;
    }
    if(3*(y*BUFFER_WIDTH + x) + 2 >= BUFFER_WIDTH*BUFFER_HEIGHT*3) {
        return;
    }
    *r = frameBuffer[3*(y*BUFFER_WIDTH + x) + 0];
    *g = frameBuffer[3*(y*BUFFER_WIDTH + x) + 1];
    *b = frameBuffer[3*(y*BUFFER_WIDTH + x) + 2];
}
void Render::bresenhamLow(int x1, int y1, int x2, int y2) {
    // remember that everything here is multiplied by 2
    int dx = x2 - x1;
    int dy = y2 - y1;
    int yi = 1;
    if(dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int diff = (2 * dy) - dx;
    int y = y1;

    for(int x = x1; x <= x2; x++) {
        writeBuffer(x, y, 255, 255, 255);
        if(diff > 0) {
            y = y + yi;
            diff += (2 * (dy - dx));
        }
        else {
            diff += 2*dy;
        }
    }
}
void Render::bresenhamHigh(int x1, int y1, int x2, int y2) {
    // remember that everything here is multiplied by 2
    int dx = x2 - x1;
    int dy = y2 - y1;
    int xi = 1;
    if(dx < 0) {
        xi = -1;
        dx = -dx;
    }
    int diff = (2 * dx) - dy;
    int x = x1;

    for(int y = y1; y <= y2; y++) {
        writeBuffer(x, y, 255, 255, 255);
        if(diff > 0) {
            x = x + xi;
            diff += (2 * (dx - dy));
        }
        else {
            diff += 2*dx;
        }
    }
}
void Render::drawLine(int x1, int y1, int x2, int y2) {
    if(abs(y2 - y1) > 1000 || abs(x2 - x1) > 1000) { return; }
    if(abs(y2 - y1) < abs(x2 - x1)) {
        if(x1 > x2) {
            bresenhamLow(x2, y2, x1, y1);
        }
        else {
            bresenhamLow(x1, y1, x2, y2);
        }
    }
    else {
        if(y1 > y2) {
            bresenhamHigh(x2, y2, x1, y1);
        }
        else {
            bresenhamHigh(x1, y1, x2, y2);
        }
    }
}
void Render::fillPoly(Vec2 screenVerts[3], Vec2 uvCoords[3]) {
    Texture* pCurrentTexture = &textures.at(0);
    int width = pCurrentTexture->width;
    int height = pCurrentTexture->height;

    if( ((screenVerts[0].x < 0) && (screenVerts[1].x < 0) && (screenVerts[0].x < 0)) ||
        ((screenVerts[0].x < 0) && (screenVerts[1].x < 0) && (screenVerts[0].x < 0)) || 
        ((screenVerts[0].x < 0) && (screenVerts[1].x < 0) && (screenVerts[0].x < 0)) || 
        ((screenVerts[0].x < 0) && (screenVerts[1].x < 0) && (screenVerts[0].x < 0))) {
            return;
        }

    uvCoords[0].x = -(uvCoords[0].x * width) + width; // u coord is backwards for some reason
    uvCoords[1].x = -(uvCoords[1].x * width) + width;
    uvCoords[2].x = -(uvCoords[2].x * width) + width;
    uvCoords[0].y *= height;
    uvCoords[1].y *= height;
    uvCoords[2].y *= height;

    uvCoords[0].floorAll();
    uvCoords[1].floorAll();
    uvCoords[2].floorAll();
    

    // sort smallest to largest y
    if(screenVerts[1].y < screenVerts[0].y) {
        Vec2 swap = screenVerts[0];
        screenVerts[0] = screenVerts[1];
        screenVerts[1] = swap;

        swap = uvCoords[0];
        uvCoords[0] = uvCoords[1];
        uvCoords[1] = swap;
    }
    if(screenVerts[2].y < screenVerts[1].y) {
        Vec2 swap = screenVerts[1];
        screenVerts[1] = screenVerts[2];
        screenVerts[2] = swap;

        swap = uvCoords[1];
        uvCoords[1] = uvCoords[2];
        uvCoords[2] = swap;
    }
    if(screenVerts[1].y < screenVerts[0].y) {
        Vec2 swap = screenVerts[0];
        screenVerts[0] = screenVerts[1];
        screenVerts[1] = swap;

        swap = uvCoords[0];
        uvCoords[0] = uvCoords[1];
        uvCoords[1] = swap;
    }

    TRI_TYPE type;
    if(screenVerts[0].y == screenVerts[1].y) {
        type = FLAT_TOP;
    }
    else if(screenVerts[1].y == screenVerts[2].y) {
        type = FLAT_BOTTOM;
    }
    else if(screenVerts[2].x < screenVerts[1].x) {
        type = GENERAL_LEFT;
    }
    else {
        type = GENERAL_RIGHT;
    }


    int x0 = screenVerts[0].x; int y0 = screenVerts[0].y;
    int x1 = screenVerts[1].x; int y1 = screenVerts[1].y;
    int x2 = screenVerts[2].x; int y2 = screenVerts[2].y;

    int u0 = uvCoords[0].x; int v0 = uvCoords[0].y;
    int u1 = uvCoords[1].x; int v1 = uvCoords[1].y;
    int u2 = uvCoords[2].x; int v2 = uvCoords[2].y;

    if(y2 - y0 > BUFFER_HEIGHT && y0 > BUFFER_HEIGHT) { return; }


    double dxdyL;
    double dxdyR;
    double dudyL;
    double dudyR;
    double dvdyL;
    double dvdyR;
    double xL; double xR;
    double uL; double uR;
    double vL; double vR;

    if(type == FLAT_TOP) {
        dxdyL = (double)(x2 - x0)/(y2 - y0);
        dxdyR = (double)(x2 - x1)/(y2 - y1);

        dudyL = (double)(u2 - u0)/(y2 - y0);
        dudyR = (double)(u2 - u1)/(y2 - y1);

        dvdyL = (double)(v2 - v0)/(y2 - y0);
        dvdyR = (double)(v2 - v1)/(y2 - y1);

        xL = x0; xR = x1;
        uL = u0; uR = u1;
        vL = v0; vR = v1;
    }
    else if(type == FLAT_BOTTOM) {
        dxdyL = (double)(x2 - x0)/(y2 - y0);
        dxdyR = (double)(x1 - x0)/(y1 - y0);

        dudyL = (double)(u2 - u0)/(y2 - y0);
        dudyR = (double)(u1 - u0)/(y1 - y0);

        dvdyL = (double)(v2 - v0)/(y2 - y0);
        dvdyR = (double)(v1 - v0)/(y1 - y0);

        xL = x0; xR = x0;
        uL = u0; uR = u0;
        vL = v0; vR = v0;
    }
    else if(type == GENERAL_LEFT) {
        dxdyL = (double)(x2 - x0)/(y2 - y0);
        dxdyR = (double)(x1 - x0)/(y1 - y0);

        dudyL = (double)(u2 - u0)/(y2 - y0);
        dudyR = (double)(u1 - u0)/(y1 - y0);

        dvdyL = (double)(v2 - v0)/(y2 - y0);
        dvdyR = (double)(v1 - v0)/(y1 - y0);

        xL = x0; xR = x0;
        uL = u0; uR = u0;
        vL = v0; vR = v0;
    }
    else if(type == GENERAL_RIGHT) {
        dxdyR = (double)(x2 - x0)/(y2 - y0);
        dxdyL = (double)(x1 - x0)/(y1 - y0);

        dudyR = (double)(u2 - u0)/(y2 - y0);
        dudyL = (double)(u1 - u0)/(y1 - y0);

        dvdyR = (double)(v2 - v0)/(y2 - y0);
        dvdyL = (double)(v1 - v0)/(y1 - y0);

        xL = x0; xR = x0;
        uL = u0; uR = u0;
        vL = v0; vR = v0;
    }

    int y = y0;
    int dx;
    int signdx;
    double du; double dv;
    double u; double v;
    while(y <= y2 && y <= BUFFER_HEIGHT) {
        dx = abs(xR - xL);
        signdx = xL <= xR ? 1 : -1;

        u = uL; v = vL;

        du = dx == 0 ? (double)(uR - uL) : (double)(uR - uL)/dx; // remember u is backwards
        dv = dx == 0 ? (double)(vR - vL) : (double)(vR - vL)/dx;

        for(int i = 0; i <= dx; i++) {
            BYTE r = 0; BYTE g = 255; BYTE b = 0;
            pCurrentTexture->readTexture(floor(u), floor(v), &r, &g, &b);
            writeBuffer(xL + i*signdx, y, r, g, b);
            u -= du; v += dv;
        }
        // copyBuffer();
        // SDL_RenderPresent(pRenderer);
        // this_thread::sleep_for(chrono::milliseconds(10));

        if(y == y1) {
            if(type == GENERAL_LEFT) {
                dxdyR = (double)(x2 - x1)/(y2 - y1);
                dudyR = (double)(u2 - u1)/(y2 - y1);
                dvdyR = (double)(v2 - v1)/(y2 - y1);
            }
            else if(type == GENERAL_RIGHT) {
                dxdyL = (double)(x2 - x1)/(y2 - y1);
                dudyL = (double)(u2 - u1)/(y2 - y1);
                dvdyL = (double)(v2 - v1)/(y2 - y1);
            }
        }

        xL += dxdyL;
        xR += dxdyR;

        uL -= dudyL;
        uR -= dudyR;

        vL += dvdyL;
        vR += dvdyR;

        y++;

    }




}
void Render::fillBuffer() {
    // camera values
    double cx = cam->cx; double cy = cam->cy; double cz = cam->cz;
    double tx = cam->tx; double ty = cam->ty; double tz = cam->tz;
    double ex = cam->ex; double ey = cam->ey; double ez = cam->ez;
    // formula variables
    double ax; double ay; double az;
    double dx; double dy; double dz;
    double bx; double by;
    // each verts screen coord
    int aDispX; int bDispX; int cDispX;
    int aDispY; int bDispY; int cDispY;
    
    for(int m = 0; m < meshes.size(); m++) {
        Mesh *pCurrentMesh = &meshes.at(m);

        for(int p = 0; p < pCurrentMesh->indices.size(); p++) {
            // this stores the index of each vert.
            Vec3 *pCurrentPoly = &pCurrentMesh->indices.at(p);
            ax = 0; ay = 0; az = 0;
            dx = 0; dy = 0; dz = 0;
            bx = 0; by = 0;
            aDispX = 0; bDispX = 0; cDispX = 0;
            aDispY = 0; bDispY = 0; cDispY = 0;

            // here the actual vertex values are fetched.
            Vec3 poly[3];
            poly[0] = Vec3(pCurrentMesh->scale*pCurrentMesh->vertices.at(pCurrentPoly->x-1).x + pCurrentMesh->origin.x,
                           pCurrentMesh->scale*pCurrentMesh->vertices.at(pCurrentPoly->x-1).y + pCurrentMesh->origin.y,
                           pCurrentMesh->scale*pCurrentMesh->vertices.at(pCurrentPoly->x-1).z + pCurrentMesh->origin.z);

            poly[1] = Vec3(pCurrentMesh->scale*pCurrentMesh->vertices.at(pCurrentPoly->y-1).x + pCurrentMesh->origin.x,
                           pCurrentMesh->scale*pCurrentMesh->vertices.at(pCurrentPoly->y-1).y + pCurrentMesh->origin.y,
                           pCurrentMesh->scale*pCurrentMesh->vertices.at(pCurrentPoly->y-1).z + pCurrentMesh->origin.z);

            poly[2] = Vec3(pCurrentMesh->scale*pCurrentMesh->vertices.at(pCurrentPoly->z-1).x + pCurrentMesh->origin.x,
                           pCurrentMesh->scale*pCurrentMesh->vertices.at(pCurrentPoly->z-1).y + pCurrentMesh->origin.y,
                           pCurrentMesh->scale*pCurrentMesh->vertices.at(pCurrentPoly->z-1).z + pCurrentMesh->origin.z);

            if(isVisible(poly)) {
                // FIRST VERTEX
                ax = poly[0].x;
                ay = poly[0].y;
                az = poly[0].z;
                dx = cos(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx)) - sin(ty)*(az-cz);
                dy = sin(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) + cos(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
                dz = cos(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) - sin(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));

                bx = (ez/dz)*dx + ex;
                by = (ez/dz)*dy + ey;

                aDispX = bx + (BUFFER_WIDTH / 2.0);
                aDispY = -by + (BUFFER_HEIGHT / 2.0);

                // SECOND VERTEX
                ax = poly[1].x;
                ay = poly[1].y;
                az = poly[1].z;
                dx = cos(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx)) - sin(ty)*(az-cz);
                dy = sin(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) + cos(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
                dz = cos(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) - sin(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));

                bx = (ez/dz)*dx + ex;
                by = (ez/dz)*dy + ey;

                bDispX = bx + (BUFFER_WIDTH / 2.0);
                bDispY = -by + (BUFFER_HEIGHT / 2.0);

                // THIRD VERTEX
                ax = poly[2].x;
                ay = poly[2].y;
                az = poly[2].z;
                dx = cos(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx)) - sin(ty)*(az-cz);
                dy = sin(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) + cos(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));
                dz = cos(tx)*(cos(ty)*(az-cz) + sin(ty)*(sin(tz)*(ay-cy) + cos(tz)*(ax-cx))) - sin(tx)*(cos(tz)*(ay-cy) - sin(tz)*(ax-cx));

                bx = (ez/dz)*dx + ex;
                by = (ez/dz)*dy + ey;

                cDispX = bx + (BUFFER_WIDTH / 2.0);
                cDispY = -by + (BUFFER_HEIGHT / 2.0);

                Vec2 screenVerts[3] = {Vec2(aDispX, aDispY), Vec2(bDispX, bDispY), Vec2(cDispX, cDispY)};
                Vec2 uvCoords[3];
                uvCoords[0] = Vec2(pCurrentMesh->textureCoords.at(pCurrentMesh->textureIndices.at(p).x-1));
                uvCoords[1] = Vec2(pCurrentMesh->textureCoords.at(pCurrentMesh->textureIndices.at(p).y-1));
                uvCoords[2] = Vec2(pCurrentMesh->textureCoords.at(pCurrentMesh->textureIndices.at(p).z-1));

                fillPoly(screenVerts, uvCoords);
                
            }
        }
    }
}
void Render::copyBuffer() {
    SDL_UpdateTexture(bufferTexture, NULL, frameBuffer, sizeof(char)*BUFFER_WIDTH*3);
    SDL_RenderCopy(pRenderer, bufferTexture, NULL, NULL);
}
void Render::clearBuffer() {
    for(int i = 0; i < BUFFER_HEIGHT; i++) {
        for(int j = 0; j < BUFFER_WIDTH; j++) {
            writeBuffer(j, i, 64,64,64);
        }
    }
}

void Render::exit() {
    free(frameBuffer);
    SDL_DestroyTexture(bufferTexture);
}