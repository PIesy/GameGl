//
// Created by akann on 4/29/17.
//

#ifndef GAMEGL_GLTEXTUREFACTORY_H
#define GAMEGL_GLTEXTUREFACTORY_H

#include "Wrappers/gltexture.h"
#include "../../../Graphics/texture.h"

namespace gl {

    class GlTextureFactory
    {
        static InternalFormat decodeFormat(const TextureInfo& parameters);
        static gl::GlTexture build2dTexture(const TextureInfo& parameters, const std::vector<StoragePointer>& pointers);
    public:
        static gl::GlTexture BuildTexture(const TextureInfo& parameters, const std::vector<StoragePointer>& pointers = {});
    };
}


#endif //GAMEGL_GLTEXTUREFACTORY_H
