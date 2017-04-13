#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "../../Graphics/graphicsclasses.h"
#include <vector>

class MeshLoader
{
public:
    std::vector<GraphicsObject> Load(const std::string& path);
};

#endif // MESHLOADER_H
