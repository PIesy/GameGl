//
// Created by akann on 3/26/18.
//

#ifndef GAMEGL_GLFENCE_H
#define GAMEGL_GLFENCE_H

#include "glbindings.h"

namespace gl
{

    class GlFence
    {
        GLsync fence;
    public:
        GlFence();
        GlFence(const GlFence&) = delete;
        GlFence& operator=(const GlFence&) = delete;
        GlFence(GlFence&& src) noexcept;
        GlFence& operator=(GlFence&& src) noexcept;
        ~GlFence();

        void Wait();
        void ClientWait();
    };

}

#endif //GAMEGL_GLFENCE_H
