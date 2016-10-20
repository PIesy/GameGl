#ifndef MESHREADEROBJ_H
#define MESHREADEROBJ_H

#include <vector>
#include "Resources/filereader.h"
#include "Graphics/graphicsclasses.h"

class MeshReaderObj : public Scanner
{
    unsigned indexOffset = 0;
    std::vector<GraphicsObject> result;
    void addVertex(std::string vertex);
    void addNormal(std::string normal);
    void addIndex(std::string index);
public:
    void Analyze(std::string input);
    std::vector<GraphicsObject> getResult();
    void Clear();
};

#endif // MESHREADEROBJ_H
