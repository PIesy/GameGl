//
// Created by akann on 4/30/17.
//

#ifndef GAMEGL_GLFRAMEBUFFERCONTAINER_H
#define GAMEGL_GLFRAMEBUFFERCONTAINER_H

#include "Wrappers/glframebuffer.h"

namespace gl
{

    struct GlFrameBufferContainer
    {
        gl::GlFrameBuffer frameBuffer{0};
        std::vector<gl::GlRenderBuffer> renderBuffers;
    };

}

#endif //GAMEGL_GLFRAMEBUFFERCONTAINER_H
