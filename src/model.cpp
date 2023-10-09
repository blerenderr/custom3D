#include "model.h"

void Mesh::readFile(std::string filename) {
    std::ifstream file(filename);
    std::string currLine;
    while (file.is_open() && file.good()) {
        getline(file,currLine);
        if(currLine.substr(0,1) == "v") { // is a vertex
            int pos1 = -1; int pos2 = -1;
            for(int i = 2; i < currLine.length(); i++) {
                if(currLine.at(i) == ' ' && pos1 == -1) { pos1 = i; }
                else if(currLine.at(i) == ' ' && pos1 != -1) { pos2 = i; }
            }
            vertices.insert(vertices.end(), Vec3(std::stod(currLine.substr(2,pos1-2)),
                            std::stod(currLine.substr(pos1+1,pos2-pos1)),
                            std::stod(currLine.substr(pos2+1))));
        }
        else if(currLine.substr(0,1) == "f") { // is an index
            int pos1 = -1; int pos2 = -1;
            for(int i = 2; i < currLine.length(); i++) {
                if(currLine.at(i) == ' ' && pos1 == -1) { pos1 = i; }
                else if(currLine.at(i) == ' ' && pos1 != -1) { pos2 = i; }
            }
            indices.insert(indices.end(), Vec3(std::stod(currLine.substr(2,pos1-2)),
                            std::stod(currLine.substr(pos1+1,pos2-pos1)),
                            std::stod(currLine.substr(pos2+1))));
        }
    }

}