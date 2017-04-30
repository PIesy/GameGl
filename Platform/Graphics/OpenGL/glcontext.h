//
// Created by akann on 4/29/17.
//

#ifndef GAMEGL_GLCONTEXT_H
#define GAMEGL_GLCONTEXT_H

#include <unordered_map>
#include "Wrappers/glvertexarray.h"
#include "Wrappers/gltexture.h"
#include "../../../Graphics/texture.h"
#include "glmeshobject.h"
#include "../../../Graphics/mesh.h"
#include "Wrappers/glframebuffer.h"
#include "../../../Graphics/renderstep.h"
#include "glframebuffercontainer.h"

class GlContext
{
    gl::GlTexture emptyTexture;

    std::unordered_map<std::string, gl::GlMeshObject> meshes;
    void configureTexture(gl::GlTexture& texture, const TextureParameters& parameters);
public:
    gl::GlTexture& LoadTexture(const Texture& texture);
    gl::GlMeshObject& LoadMesh(const Mesh& mesh);
    gl::GlFrameBufferContainer BuildFrameBuffer(const std::list<RenderTarget>& targets);
};


#endif //GAMEGL_GLCONTEXT_H
