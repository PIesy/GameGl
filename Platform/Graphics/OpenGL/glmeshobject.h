//
// Created by akann on 4/30/17.
//

#ifndef GAMEGL_GLMESHOBJECT_H
#define GAMEGL_GLMESHOBJECT_H

#include <vector>
#include "Wrappers/gltexture.h"
#include "Wrappers/glvertexarray.h"

namespace gl
{

    struct GlMeshObject
    {
        std::vector<std::reference_wrapper<gl::GlTexture>> textures;
        gl::GlVertexArray vertexArray{0};
    };

}


#endif //GAMEGL_GLMESHOBJECT_H
