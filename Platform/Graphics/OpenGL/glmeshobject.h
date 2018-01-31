//
// Created by akann on 4/30/17.
//

#ifndef GAMEGL_GLMESHOBJECT_H
#define GAMEGL_GLMESHOBJECT_H

#include <vector>
#include "Wrappers/gltexture.h"
#include "Wrappers/glvertexarray.h"
#include "../../../Helpers/lock_holder.h"

namespace gl
{

    struct GlMeshObject
    {
        gl::GlVertexArray vertexArray{0};
    };

}


#endif //GAMEGL_GLMESHOBJECT_H
