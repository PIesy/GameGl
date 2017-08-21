//
// Created by akann on 6/8/17.
//

#ifndef GAMEGL_MATERIAL_H
#define GAMEGL_MATERIAL_H

#include "../Graphics/texture.h"
#include "../Graphics/mesh.h"
#include <vector>

struct Material
{
    std::vector<Texture> texture;
    MaterialProperties properties;
};


#endif //GAMEGL_MATERIAL_H
