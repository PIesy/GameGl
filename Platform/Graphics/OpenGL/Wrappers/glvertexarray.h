//
// Created by akann on 3/14/17.
//

#ifndef GAMEGL_GLVERTEXARRAY_H
#define GAMEGL_GLVERTEXARRAY_H

#include <vector>
#include <GL/glew.h>
#include "glbindings.h"
#include "../../../../Helpers/helpers.h"
#include "glbuffer.h"

namespace gl {

    enum class AttributeTypeFloat : GLenum
    {
        Float = GL_FLOAT,
        Byte = GL_BYTE,
        Double = GL_DOUBLE,
        HalfFloat = GL_HALF_FLOAT
    };


    class GlVertexArray
    {
        GLuint vertexArray = 0;
    public:
        GlVertexArray();
        explicit GlVertexArray(GLuint id);
        GlVertexArray(const GlVertexArray&) = delete;
        GlVertexArray& operator =(const GlVertexArray&) = delete;
        GlVertexArray(GlVertexArray&& src) noexcept;
        GlVertexArray& operator =(GlVertexArray&& src) noexcept;
        ~GlVertexArray();

        void BindElementBuffer(gl::GlBuffer& elementBuffer);
        void BindBuffer(gl::GlBuffer& buffer, unsigned index, ptrdiff_t offset, int size);
        void UnbindBuffer(unsigned index);
        void EnableAttribute(unsigned index);
        void DisableAttribute(unsigned index);
        void SetAttributeFormat(unsigned index, int size, AttributeTypeFloat type, unsigned offset, bool normalized = false);
        void LinkBuffer(unsigned attributeIndex, unsigned bufferIndex);
        void Bind();
    };
}


#endif //GAMEGL_GLVERTEXARRAY_H
