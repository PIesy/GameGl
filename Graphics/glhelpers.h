#ifndef GLHELPERS_H
#define GLHELPERS_H

#include "GL/glew.h"
#include "GL/glu.h"
#include "graphicsclasses.h"

namespace glhelpers
{

    struct VertexArrayObject
    {
        GLuint VBO;
        GLuint IBO;
        GLuint VAO;
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

}

#endif // GLHELPERS_H
