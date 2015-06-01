#ifndef MESHREADER3DS_H
#define MESHREADER3DS_H

#include "Resources/filereader.h"
#include "Graphics/graphicsclasses.h"

class MeshReader3ds : public Scanner
{
    RenderObject result;
    void addVertex(std::string vertex);
    void addNormal(std::string normal);
    void addIndex(std::string index);
public:
    void Analyze(std::string input);
    VertexObject getResult();
    void Clear();
};

#endif // MESHREADER3DS_H
