#include "mesh.hpp"
using namespace std;

void Mesh::readFile(string filename) {
    ifstream file(filename);
    string currLine;
    while (file.is_open() && file.good()) {
        getline(file,currLine);
        if(currLine.substr(0,2) == "v ") { // is a vertex
            int space1 = -1; int space2 = -1;
            for(int i = 2; i < currLine.length(); i++) {
                if(currLine.at(i) == ' ' && space1 == -1) { space1 = i; }
                else if(currLine.at(i) == ' ' && space1 != -1) { space2 = i; }
            }
            vertices.insert(vertices.end(), Vec3(stod(currLine.substr(2,space1-2)),
                                                             stod(currLine.substr(space1+1,space2-space1)),
                                                             stod(currLine.substr(space2+1))));
        }
        else if(currLine.substr(0,2) == "vt") { // is a texture coord
            int space1 = -1;
            for(int i = 3; i < currLine.length(); i++) {
                if(currLine.at(i) == ' ' && space1 == -1) { space1 = i; }
            }
            textureCoords.insert(textureCoords.end(), Vec2(stod(currLine.substr(2,space1-2)),
                                                                       stod(currLine.substr(space1+1))));
        }
        else if(currLine.substr(0,2) == "f ") { // is an index / texture index
            int space1 = -1; int space2 = -1;
            int slash1 = -1; int slash2 = -1; int slash3 = -1;
            for(int i = 2; i < currLine.length(); i++) {
                switch(currLine.at(i)) {
                    case ' ':
                        space1 == -1 ? space1 = i : space2 = i;
                        break;
                    case '/':
                        if(slash1 == -1) {
                            slash1 = i;
                        }
                        else if(slash2 == -1) {
                            slash2 = i;
                        }
                        else{
                            slash3 = i;
                        }
                        break;
                }
            }
            indices.insert(indices.end(), Vec3(stod(currLine.substr(2,slash1-2)),
                                                           stod(currLine.substr(space1+1,slash2-space1)),
                                                           stod(currLine.substr(space2+1,slash3-space2))));

            if(slash1 != -1) { // if fails, there are no texture indices
                textureIndices.insert(textureIndices.end(), Vec3(stod(currLine.substr(slash1+1,space1-slash1)),
                                                                             stod(currLine.substr(slash2+1,space2-slash2)),
                                                                             stod(currLine.substr(slash3+1))));
            }
        }
    }
}
