#ifndef SCENE_H
#define SCENE_H

#include "texture.h"
#include "renderpath.h"
#include "graphicsclasses.h"
#include <vector>


struct Scene
{
    std::vector<Texture> textures;
    std::vector<GraphicsObject> objects;
    std::vector<std::reference_wrapper<Program>> programs;
    RenderPath path;
};

#endif // SCENE_H
