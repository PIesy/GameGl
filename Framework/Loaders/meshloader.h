#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "../../Graphics/graphicsclasses.h"
#include "textureloader.h"
#include "../../Graphics/mesh.h"
#include <vector>

class MeshLoader
{
    StorageApi& api;
public:
    MeshLoader(StorageApi& storage);
    std::vector<Mesh> Load(const std::string& path);
};

#endif // MESHLOADER_H
