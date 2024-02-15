#include "texture.hpp"
using namespace std;

void Texture::readFile(string filename) {
    ifstream file(filename);
    if(file) {
        file.seekg(0, ios::end);
        streampos length = file.tellg();
        file.seekg(0, ios::beg);

        data.resize(length);
        file.read(&data[0],length);
    }
}
void Texture::readTexture(int x, int y, BYTE* r, BYTE* g, BYTE* b) {
    if(x >= width || y >= height || x < 0 || y < 0) {
        return;
    }
    *r = data[3*(y*width + x) + 0];
    *g = data[3*(y*width + x) + 1];
    *b = data[3*(y*width + x) + 2];
}