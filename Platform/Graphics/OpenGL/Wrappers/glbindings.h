#ifndef GLBINDINGS_H
#define GLBINDINGS_H

#include <GL/glew.h>
#include <GL/glu.h>
#include <string>

namespace gl
{

    namespace vertex_array
    {
        void generate(int count, GLuint* vertexArrays);
        void bind(GLuint vertexArray);
        void erase(int count, GLuint* vertexArrays);
        void enableVertexAttribute(GLuint target, GLuint index);
        void disableVertexAttribute(GLuint target, GLuint index);
        void bindElementBuffer(GLuint target, GLuint buffer);
        void bindVertexBuffer(GLuint target, GLuint bindingIndex, GLuint buffer, GLintptr offset, GLsizei stride);
        void bindBufferToAttribute(GLuint target, GLuint attributeIndex, GLuint bindingIndex);
        void setVertexAttributeFormat(GLuint target, GLuint index, int size, GLenum type, GLboolean normalized, GLuint offset);
    }

    namespace buffer
    {
        void generate(int count, GLuint* buffers);
        void bind(GLenum target, GLuint buffer);
        void erase(int count, GLuint* buffers);
        void setStorage(GLuint target, size_t size, const void* data, GLbitfield flags);
        void updateData(GLuint target, GLintptr offset, size_t size, const void* data);
    }

    namespace texture
    {
        void generate(int count, GLuint* textures, GLenum target);
        void bind(GLenum target, GLuint texture);
        void erase(int count, GLuint* textures);
        void allocateStorage(GLuint target, GLsizei mipmapLevels, GLenum internalFormat, GLsizei width, GLsizei height);
        void allocateStorage(GLuint target, GLsizei mipmapLevels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth);
        void loadImage(GLuint target, GLint mipmapLevel, GLint xoffset, GLint yoffset, GLsizei width,
                       GLsizei height, GLenum format, GLenum type, const void* data);
        void loadImage(GLuint target, GLint mipmapLevel, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width,
                       GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* data);
        void setParameter(GLuint target, GLenum parameter, GLint value);
        void setParameter(GLuint target, GLenum parameter, GLfloat value);
        void setActiveTexture(GLenum textureUnit);
        void generateMipmaps(GLuint target);
    }

    namespace framebuffer
    {
        void generate(int count, GLuint* framebuffers);
        void bind(GLenum target, GLuint framebuffer);
        void erase(int count, GLuint* framebuffers);
        void setRenderbuffer(GLuint target, GLenum attachment, GLuint buffer);
        void setTexture(GLuint target, GLenum attachment, GLuint texture, GLint level);
        void setDrawBuffers(GLuint target, GLsizei count, const GLenum* buffers);
        GLenum checkStatus(GLuint target, GLenum bindPoint);
        void blit(GLuint src, GLuint target, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
                  GLint targetX0, GLint targetY0, GLint targetX1, GLint targetY1, GLbitfield bufferMask, GLenum filter);
    }

    namespace renderbuffer
    {
        void generate(int count, GLuint* renderbuffers);
        void bind(GLenum target, GLuint renderbuffer);
        void erase(int count, GLuint* renderbuffers);
        void setStorage(GLuint target, GLenum component, unsigned width, unsigned height);
    }

    namespace depthbuffer
    {
        void setFunction(GLenum function);
        void enableWrite(GLboolean flag);
        void setRange(GLfloat near, GLfloat far);
    }

    namespace culling
    {
        void setFrontFace(GLenum face);
        void cullFace(GLenum face);
    }

    namespace shader
    {
        GLuint create(GLenum type);
        void setSource(GLuint shader, const std::string& source);
        void compile(GLuint shader);
        void erase(GLuint shader);
    }

    namespace program
    {
        GLuint create();
        GLuint create(GLenum type, const std::string& source);
        void attachShader(GLuint program, GLuint shader);
        void detachShader(GLuint program, GLuint shader);
        void link(GLuint program);
        void erase(GLuint program);
        void use(GLuint program);
        GLint getUniformLocation(GLuint program, const std::string& uniformName);
        void setUniform1f(GLuint program, GLuint location, int count, const void* value);
        void setUniform2f(GLuint program, GLuint location, int count, const void* value);
        void setUniform3f(GLuint program, GLuint location, int count, const void* value);
        void setUniform4f(GLuint program, GLuint location, int count, const void* value);
        void setUniform1i(GLuint program, GLuint location, int count, const void* value);
        void setUniform2i(GLuint program, GLuint location, int count, const void* value);
        void setUniform3i(GLuint program, GLuint location, int count, const void* value);
        void setUniform4i(GLuint program, GLuint location, int count, const void* value);
        void setUniform1d(GLuint program, GLuint location, int count, const void* value);
        void setUniform2d(GLuint program, GLuint location, int count, const void* value);
        void setUniform3d(GLuint program, GLuint location, int count, const void* value);
        void setUniform4d(GLuint program, GLuint location, int count, const void* value);
        void setUniform1ui(GLuint program, GLuint location, int count, const void* value);
        void setUniform2ui(GLuint program, GLuint location, int count, const void* value);
        void setUniform3ui(GLuint program, GLuint location, int count, const void* value);
        void setUniform4ui(GLuint program, GLuint location, int count, const void* value);
        void setUniform2x2f(GLuint program, GLuint location, int count, const void* value);
        void setUniform3x3f(GLuint program, GLuint location, int count, const void* value);
        void setUniform4x4f(GLuint program, GLuint location, int count, const void* value);
    }

    namespace program_pipeline
    {
        GLuint create();
        void erase(GLuint programPipeline);
        std::string getInfoLog(GLuint programPipeline);
        void bindProgramPipeline(GLuint programPipeline);
        void useProgramStages(GLuint programPipeline, GLbitfield stages, GLuint program);
        void validate(GLuint programPipeline);
        void getParameter(GLuint programPipeline, GLenum parameterName, GLint* result);
    }

    namespace sync
    {
        GLsync createFence();
        void erase(GLsync sync);
        void clientWait(GLsync sync, GLbitfield flags, uint64_t timeout);
        void wait(GLsync sync, GLbitfield flags, uint64_t timeout);
    }

    void drawElements(GLenum mode, size_t count, GLenum type, const void* indices);
    void setViewport(unsigned width, unsigned height, int offsetX = 0, int offsetY = 0);
    void clear(GLbitfield mask);
    void setClearColor(float r, float g, float b, float alpha);
    void setBlendFunction(GLenum sfactor, GLenum dfactor);
    void enable(GLenum capability);
    void disable(GLenum capability);
    void flush();
}

#endif // GLBINDINGS_H

