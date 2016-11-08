#ifndef GLBINDINGS_H
#define GLBINDINGS_H

#include <GL/glew.h>
#include <GL/glu.h>
#include <string>

namespace gl {

namespace vertexarray {
    void generate(int count, GLuint* vertexArrays);
    void bind(GLuint vertexArray);
    void erase(int count, GLuint* vertexArrays);
    void enableVertexAttribute(GLuint index);
    void disableVertexAttribute(GLuint index);
    void setVertexAttributePointer(GLuint index, int size, GLenum type, GLboolean normalized, size_t stride, const void* pointer);
}

namespace buffer {
    void generate(int count, GLuint* buffers);
    void bind(GLenum target, GLuint buffer);
    void erase(int count, GLuint* buffers);
    void setData(GLenum target, size_t size, const void* data, GLenum usage);
    void updateData(GLenum target, GLintptr offset, size_t size, const void* data);
}

namespace texture {
    void generate(int count, GLuint* textures);
    void bind(GLenum target, GLuint texture);
    void erase(int count, GLuint* textures);
    void load2Dimage(GLenum target, GLint mipmapLevel, GLint internalFormat, unsigned width,
                     unsigned height, GLenum format, GLenum type, const void* data);
    void setParameter(GLenum target, GLenum parameter, GLint value);
    void setParameter(GLenum target, GLenum parameter, GLfloat value);
}

namespace framebuffer {
    void generate(int count, GLuint* framebuffers);
    void bind(GLenum target, GLuint framebuffer);
    void erase(int count, GLuint* framebuffers);
    void setRenderbuffer(GLenum target, GLenum attachment, GLenum type, GLuint buffer);
    void set2Dtexture(GLenum target, GLenum attachment, GLenum type, GLuint texture, GLint mipmapLevel);
}

namespace renderbuffer {
    void generate(int count, GLuint* renderbuffers);
    void bind(GLenum target, GLuint renderbuffer);
    void erase(int count, GLuint* renderbuffers);
    void setStorage(GLenum target, GLenum component, unsigned width, unsigned height);
}

namespace depthbuffer {
    void setFunction(GLenum function);
    void enableWrite(GLboolean flag);
    void setRange(GLfloat near, GLfloat far);
}

namespace culling {
    void setFrontFace(GLenum face);
    void cullFace(GLenum face);
}

namespace shader {
    GLuint create(GLenum type);
    void setSource(GLuint shader, const std::string& source);
    void compile(GLuint shader);
    void erase(GLuint shader);
}

namespace program {
    GLuint create();
    void attachShader(GLuint program, GLuint shader);
    void detachShader(GLuint program, GLuint shader);
    void link(GLuint program);
    void erase(GLuint program);
    void use(GLuint program);
    GLuint getUniformLocation(GLuint program, const std::string& uniformName);
    void setUniform1f(GLuint location, int count, const void* value);
    void setUniform2f(GLuint location, int count, const void* value);
    void setUniform3f(GLuint location, int count, const void* value);
    void setUniform4f(GLuint location, int count, const void* value);
    void setUniform1i(GLuint location, int count, const void* value);
    void setUniform2i(GLuint location, int count, const void* value);
    void setUniform3i(GLuint location, int count, const void* value);
    void setUniform4i(GLuint location, int count, const void* value);
    void setUniform1ui(GLuint location, int count, const void* value);
    void setUniform2ui(GLuint location, int count, const void* value);
    void setUniform3ui(GLuint location, int count, const void* value);
    void setUniform4ui(GLuint location, int count, const void* value);
    void setUniform2x2f(GLuint location, int count, const void* value);
    void setUniform3x3f(GLuint location, int count, const void* value);
    void setUniform4x4f(GLuint location, int count, const void* value);
}

void drawElements(GLenum mode, size_t count, GLenum type, const void* indices);
void setViewport(unsigned width, unsigned height, int offsetX = 0, int offsetY = 0);
void clear(GLbitfield mask);
void setClearColor(float r, float g, float b, float alpha);
void setBlendFunction(GLenum sfactor, GLenum dfactor);
void enable(GLenum capacity);
void disable(GLenum capacity);



}

#endif // GLBINDINGS_H

