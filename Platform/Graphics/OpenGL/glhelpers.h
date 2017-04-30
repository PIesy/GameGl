#ifndef GLHELPERS_H
#define GLHELPERS_H

#include "Wrappers/glbindings.h"
#include "../../../Graphics/graphicsclasses.h"
#include "../../../Graphics/rendertarget.h"
#include "Wrappers/glbuffer.h"
#include "Wrappers/gltexture.h"
#include "Wrappers/glvertexarray.h"
#include "Wrappers/glframebuffer.h"
#include "../../../Graphics/mesh.h"

namespace glhelpers
{

    struct VertexArrayObject
    {
        gl::GlBuffer VBO{0};
        gl::GlBuffer IBO{0};
        gl::GlVertexArray VAO{0};
        gl::GlTexture* tex = nullptr;
        bool externTex = false;
        int stepId = 0;
        int texId = 0;

        VertexArrayObject() = default;
        VertexArrayObject(const VertexArrayObject&) = delete;
        VertexArrayObject(VertexArrayObject&&) = default;
    };

    struct FramebufferObject
    {
        gl::GlFrameBuffer FBO{0};
        gl::GlRenderBuffer depthBuff{0};
    };

    VertexArrayObject initVAO(Mesh& mesh);
    void updateDataBuffers(Mesh& mesh, VertexArrayObject& vao);
    FramebufferObject initFramebuffer(std::list<RenderTarget>& targets);
}

#endif // GLHELPERS_H
