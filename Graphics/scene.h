#ifndef SCENE_H
#define SCENE_H

#include "texture.h"
#include "renderpath.h"
#include "graphicsclasses.h"
#include "mesh.h"
#include "meshdescriptor.h"
#include <vector>


struct Scene
{
    std::vector<MeshDescriptor> meshes;
    RenderPath path;
};

#endif // SCENE_H
