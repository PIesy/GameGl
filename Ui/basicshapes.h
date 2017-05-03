#ifndef BASICSHAPES_H
#define BASICSHAPES_H

#include "../Graphics/graphicsapi.h"
#include "../Graphics/mesh.h"
#include "../Storage/storageapi.h"

class Shapes
{
public:
    static Mesh Rectangle(StorageApi& api, const std::string& name = "quad");
    static Mesh Plane(int xSize, int ySize, StorageApi& api, const std::string& name = "plane", Vec4&& color = {0.5f, 0.5f, 0.5f, 1.0f});
};
#endif // BASICSHAPES_H
