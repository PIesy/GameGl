//
// Created by akann on 3/14/17.
//

#ifndef GAMEGL_GLVERTEXARRAY_H
#define GAMEGL_GLVERTEXARRAY_H

#include <vector>
#include <GL/glew.h>
#include "glbindings.h"
#include "../../Helpers/helpers.h"
#include "glbuffer.h"

namespace gl {

    class GlVertexArray;

    class GlVertexArrayFactory
    {
    public:
        static std::vector<GlVertexArray> CreateVertexArrays(int count)
        {
            std::vector<GlVertexArray> result;
            std::vector<GLuint> ids(count);

            for (GLuint& id : ids)
                result.emplace_back(id);
            return result;
        }
    };

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
        GlVertexArray(GLuint id);
        GlVertexArray(const GlVertexArray&) = delete;
        GlVertexArray& operator =(const GlVertexArray&) = delete;
        GlVertexArray(GlVertexArray&& src);
        GlVertexArray& operator =(GlVertexArray&& src);
        ~GlVertexArray();

        void BindElementBuffer(gl::GlBuffer& elementBuffer);
        void BindBuffer(gl::GlBuffer& buffer, unsigned index, ptrdiff_t offset, int size);
        void EnableAttribute(unsigned index);
        void DisableAttribute(unsigned index);
        void SetAttributeFormat(unsigned index, int size, AttributeTypeFloat type, unsigned offset, bool normalized = false);
        void LinkBuffer(unsigned attributeIndex, unsigned bufferIndex);
        void Bind();
    };
}


#endif //GAMEGL_GLVERTEXARRAY_H
