//
// Created by akann on 4/15/17.
//

#ifndef GAMEGL_GLRENDERBUFFER_H
#define GAMEGL_GLRENDERBUFFER_H

#include "glbindings.h"
#include "globject.h"
#include "gltexture.h"

namespace gl {

    class GlRenderBuffer : public GlObject
    {
        GLuint renderBuffer = 0;
    public:
        GlRenderBuffer();
        explicit GlRenderBuffer(GLuint id);
        GlRenderBuffer(const GlRenderBuffer&) = delete;
        GlRenderBuffer& operator=(const GlRenderBuffer&) = delete;
        GlRenderBuffer(GlRenderBuffer&& src) noexcept;
        GlRenderBuffer& operator=(GlRenderBuffer&& src) noexcept;
        ~GlRenderBuffer() override;

        void SetStorage(gl::InternalFormat internalFormat, unsigned width, unsigned height);

        GLuint GetId() const override;
    };

}
#endif //GAMEGL_GLRENDERBUFFER_H
