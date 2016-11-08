#ifndef GLHELPERS_H
#define GLHELPERS_H

#include "Platform/OpenGL/glbindings.h"
#include "graphicsclasses.h"
#include "rendertarget.h"

namespace glhelpers
{

    struct VertexArrayObject
    {
        GLuint VBO;
        GLuint IBO;
        GLuint VAO;
        GLuint tex = 0;
        bool externTex = false;
        int stepId = 0;
        int texId = 0;
    };

    struct FramebufferObject
    {
        GLuint FBO = 0;
        std::vector<GLuint> textures = {0};
        GLuint depthBuff = 0;
    };

    class RAIIBufferBinding
    {
        GLenum target;
    public:
        RAIIBufferBinding(GLenum target, GLuint buffer);
        ~RAIIBufferBinding();
    };

    VertexArrayObject initVAO(GraphicsObject& obj, bool unbindAfterInit = false);
    void updateDataBuffers(GraphicsObject& obj, VertexArrayObject& vao);

    FramebufferObject initFramebuffer(const RenderTarget& target);
}

#endif // GLHELPERS_H
