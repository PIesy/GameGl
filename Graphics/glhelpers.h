#ifndef GLHELPERS_H
#define GLHELPERS_H

#include "../Platform/OpenGL/glbindings.h"
#include "graphicsclasses.h"
#include "rendertarget.h"
#include "../Platform/OpenGL/glbuffer.h"
#include "../Platform/OpenGL/gltexture.h"
#include "../Platform/OpenGL/glvertexarray.h"
#include "../Platform/OpenGL/glframebuffer.h"

namespace glhelpers
{

    struct VertexArrayObject
    {
        gl::GlBuffer VBO;
        gl::GlBuffer IBO;
        gl::GlVertexArray VAO;
        gl::GlTexture tex;
        bool externTex = false;
        int stepId = 0;
        int texId = 0;

        VertexArrayObject() = default;
        VertexArrayObject(const VertexArrayObject&) = delete;
        VertexArrayObject(VertexArrayObject&&) = default;
    };

    struct FramebufferObject
    {
        gl::GlFrameBuffer FBO = gl::GlFrameBuffer(0);
        std::vector<gl::GlTexture> textures;
        GLuint depthBuff = 0;
    };

    VertexArrayObject initVAO(GraphicsObject& obj);
    void updateDataBuffers(GraphicsObject& obj, VertexArrayObject& vao);
    FramebufferObject initFramebuffer(const RenderTarget& target);
}

#endif // GLHELPERS_H
