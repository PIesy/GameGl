//
// Created by akann on 4/15/17.
//

#ifndef GAMEGL_GLRENDERBUFFER_H
#define GAMEGL_GLRENDERBUFFER_H

#include "glbindings.h"
#include "globject.h"
#include "gltexture.h"

namespace gl {

    class GlRenderBuffer : GlObject
    {
        GLuint renderBuffer = 0;
    public:
        GlRenderBuffer();
        GlRenderBuffer(GLuint id);
        GlRenderBuffer(const GlRenderBuffer&) = delete;
        GlRenderBuffer& operator=(const GlRenderBuffer&) = delete;
        GlRenderBuffer(GlRenderBuffer&& src);
        GlRenderBuffer& operator=(GlRenderBuffer&& src);
        ~GlRenderBuffer();

        void SetStorage(gl::InternalFormat internalFormat, unsigned width, unsigned height);

        GLuint GetId() const override;
    };

}
#endif //GAMEGL_GLRENDERBUFFER_H
