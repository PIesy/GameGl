#include "glbindings.h"
#include "gldebug.h"


//vertex array

void gl::vertexarray::generate(int count, GLuint* vertexArrays)
{
    glGenVertexArrays(count, vertexArrays);
    gl::printGlError("glGenVertexArrays");
}

void gl::vertexarray::bind(GLuint vertexArray)
{
    glBindVertexArray(vertexArray);
    gl::printGlError("glBindVertexArray");
}

void gl::vertexarray::erase(int count, GLuint* vertexArrays)
{
    glDeleteVertexArrays(count, vertexArrays);
    gl::printGlError("glDeleteVertexArrays");
}

void gl::vertexarray::disableVertexAttribute(GLuint index)
{
    glDisableVertexAttribArray(index);
    gl::printGlError("glDisableVertexAttribArray");
}

void gl::vertexarray::setVertexAttributePointer(GLuint index, int size, GLenum type, GLboolean normalized, size_t stride, const void* pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    gl::printGlError("glVertexAttribPointer");
}

//buffer

void gl::buffer::generate(int count, GLuint* buffers)
{
    glGenBuffers(count, buffers);
    gl::printGlError("glGenBuffers");
}

void gl::buffer::bind(GLenum target, GLuint buffer)
{
    glBindBuffer(target, buffer);
    gl::printGlError("glBindBuffer");
}

void gl::buffer::erase(int count, GLuint* buffers)
{
    glDeleteBuffers(count, buffers);
    gl::printGlError("glDeleteBuffers");
}

void gl::buffer::setData(GLenum target, size_t size, const void* data, GLenum usage)
{
    glBufferData(target, size, data, usage);
    gl::printGlError("glBufferData");
}

void gl::buffer::updateData(GLenum target, GLintptr offset, size_t size, const void* data)
{
    glBufferSubData(target, offset, size, data);
    gl::printGlError("glBufferSubData");
}

//texture

void gl::texture::generate(int count, GLuint* textures)
{
    glGenTextures(count, textures);
    gl::printGlError("glGenTextures");
}

void gl::texture::bind(GLenum target, GLuint texture)
{
    glBindTexture(target, texture);
    gl::printGlError("glBindTexture");
}

void gl::texture::erase(int count, GLuint* textures)
{
    glDeleteTextures(count, textures);
    gl::printGlError("glDeleteTextures");
}

void gl::texture::load2Dimage(GLenum target, GLint mipmapLevel, GLint internalFormat, unsigned width, unsigned height, GLenum format, GLenum type, const void* data)
{
    glTexImage2D(target, mipmapLevel, internalFormat, width, height, 0, format, type, data);
    gl::printGlError("glTexImage2D");
}

void gl::texture::setParameter(GLenum target, GLenum parameter, GLint value)
{
    glTexParameteri(target, parameter, value);
    gl::printGlError("glTexParameteri");
}

void gl::texture::setParameter(GLenum target, GLenum parameter, GLfloat value)
{
    glTexParameterf(target, parameter, value);
    gl::printGlError("glTexParameterf");
}

//framebuffer

void gl::framebuffer::generate(int count, GLuint* framebuffers)
{
    glGenFramebuffers(count, framebuffers);
    gl::printGlError("glGenFramebuffer");
}

void gl::framebuffer::bind(GLenum target, GLuint framebuffer)
{
    glBindFramebuffer(target, framebuffer);
    gl::printGlError("glBindFramebuffer");
}

void gl::framebuffer::erase(int count, GLuint* framebuffers)
{
    glDeleteFramebuffers(count, framebuffers);
    gl::printGlError("glDeleteFramebuffers");
}

void gl::framebuffer::setRenderbuffer(GLenum target, GLenum attachment, GLenum type, GLuint buffer)
{
    glFramebufferRenderbuffer(target, attachment, type, buffer);
    gl::printGlError("glFramebufferRenderbuffer");
}

void gl::framebuffer::set2Dtexture(GLenum target, GLenum attachment, GLenum type, GLuint texture, GLint mipmapLevel)
{
    glFramebufferTexture2D(target, attachment, type, texture, mipmapLevel);
    gl::printGlError("glFramebufferTexture2D");
}

//renderbuffer

void gl::renderbuffer::generate(int count, GLuint* renderbuffers)
{
    glGenRenderbuffers(count, renderbuffers);
    gl::printGlError("glGenRenderbuffers");
}

void gl::renderbuffer::bind(GLenum target, GLuint renderbuffer)
{
    glBindRenderbuffer(target, renderbuffer);
    gl::printGlError("glBindRenderbuffer");
}

void gl::renderbuffer::erase(int count, GLuint* renderbuffers)
{
    glDeleteRenderbuffers(count, renderbuffers);
    gl::printGlError("glDeleteRenderBuffer");
}

void gl::renderbuffer::setStorage(GLenum target, GLenum component, unsigned width, unsigned height)
{
    glRenderbufferStorage(target, component, width, height);
    gl::printGlError("glRenderbufferStorage");
}

//depth buffer

void gl::depthbuffer::setFunction(GLenum function)
{
    glDepthFunc(function);
    gl::printGlError("glDepthFunc");
}

void gl::depthbuffer::enableWrite(GLboolean flag)
{
    glDepthMask(flag);
    gl::printGlError("glDepthMask");
}

void gl::depthbuffer::setRange(GLfloat near, GLfloat far)
{
    glDepthRange(near, far);
    gl::printGlError("glDepthRange");
}

//culling

void gl::culling::setFrontFace(GLenum face)
{
    glFrontFace(face);
    gl::printGlError("glFrontFace");
}

void gl::culling::cullFace(GLenum face)
{
    glCullFace(face);
    gl::printGlError("glCullFace");
}

//shader
GLuint gl::shader::create(GLenum type)
{
    GLuint result = glCreateShader(type);
    gl::printGlError("glCreateShader");
    return result;
}

void gl::shader::setSource(GLuint shader, const std::string& source)
{
    int size = (int)source.length();
    const char* str = source.c_str();
    glShaderSource(shader, 1, &str, &size);
    gl::printGlError("glShaderSource");
}

void gl::shader::compile(GLuint shader)
{
    glCompileShader(shader);
    gl::printGlError("glCompileShader");
}

void gl::shader::erase(GLuint shader)
{
    glDeleteShader(shader);
    gl::printGlError("glDeleteShader");
}

//program
GLuint gl::program::create()
{
    GLuint result = glCreateProgram();
    gl::printGlError("glCreateProgram");
    return result;
}

void gl::program::use(GLuint program)
{
    glUseProgram(program);
    gl::printGlError("glUseProgram");
}

void gl::program::attachShader(GLuint program, GLuint shader)
{
    glAttachShader(program, shader);
    gl::printGlError("glAttachShader");
}

void gl::program::detachShader(GLuint program, GLuint shader)
{
    glDetachShader(program, shader);
    gl::printGlError("glDetachShader");
}

void gl::program::link(GLuint program)
{
    glLinkProgram(program);
    gl::printGlError("glLinkProgram");
}

void gl::program::erase(GLuint program)
{
    glDeleteProgram(program);
    gl::printGlError("glDeleteProgram");
}


GLuint gl::program::getUniformLocation(GLuint program, const std::string& uniformName)
{
    GLuint result = glGetUniformLocation(program, uniformName.c_str());
    gl::printGlError("glGetUniformLocation");
    return result;
}

void gl::program::setUniform1f(GLuint location, int count, const void* value)
{
    glUniform1fv(location, count, (const GLfloat*)value);
    gl::printGlError("glUniform1fv");
}

void gl::program::setUniform2f(GLuint location, int count, const void* value)
{
    glUniform2fv(location, count, (const GLfloat*)value);
    gl::printGlError("glUniform2fv");
}

void gl::program::setUniform3f(GLuint location, int count, const void* value)
{
    glUniform3fv(location, count, (const GLfloat*)value);
    gl::printGlError("glUniform3fv");
}

void gl::program::setUniform4f(GLuint location, int count, const void* value)
{
    glUniform4fv(location, count, (const GLfloat*)value);
    gl::printGlError("glUniform4fv");
}

void gl::program::setUniform1i(GLuint location, int count, const void* value)
{
    glUniform1iv(location, count, (const GLint*)value);
    gl::printGlError("glUniform1iv");
}

void gl::program::setUniform2i(GLuint location, int count, const void* value)
{
    glUniform2iv(location, count, (const GLint*)value);
    gl::printGlError("glUniform2iv");
}

void gl::program::setUniform3i(GLuint location, int count, const void* value)
{
    glUniform3iv(location, count, (const GLint*)value);
    gl::printGlError("glUniform3iv");
}

void gl::program::setUniform4i(GLuint location, int count, const void* value)
{
    glUniform4iv(location, count, (const GLint*)value);
    gl::printGlError("glUniform4iv");
}

void gl::program::setUniform1ui(GLuint location, int count, const void* value)
{
    glUniform1uiv(location, count, (const GLuint*)value);
    gl::printGlError("glUniform1uiv");
}

void gl::program::setUniform2ui(GLuint location, int count, const void* value)
{
    glUniform2uiv(location, count, (const GLuint*)value);
    gl::printGlError("glUniform2uiv");
}

void gl::program::setUniform3ui(GLuint location, int count, const void* value)
{
    glUniform3uiv(location, count, (const GLuint*)value);
    gl::printGlError("glUniform3uiv");
}

void gl::program::setUniform4ui(GLuint location, int count, const void* value)
{
    glUniform4uiv(location, count, (const GLuint*)value);
    gl::printGlError("glUniform4uiv");
}

void gl::program::setUniform2x2f(GLuint location, int count, const void* value)
{
    glUniformMatrix2fv(location, count, GL_FALSE, (const GLfloat*)value);
    gl::printGlError("glUniformMatrix2fv");
}

void gl::program::setUniform3x3f(GLuint location, int count, const void* value)
{
    glUniformMatrix3fv(location, count, GL_FALSE, (const GLfloat*)value);
    gl::printGlError("glUniformMatrix3fv");
}

void gl::program::setUniform4x4f(GLuint location, int count, const void* value)
{
    glUniformMatrix4fv(location, count, GL_FALSE, (const GLfloat*)value);
    gl::printGlError("glUniformMatrix4fv");
}

//generic

void gl::drawElements(GLenum mode, size_t count, GLenum type, const void* indices)
{
    glDrawElements(mode, count, type, indices);
    gl::printGlError("glDrawElements");
}

void gl::setViewport(unsigned width, unsigned height, int offsetX, int offsetY)
{
    glViewport(offsetX, offsetY, width, height);
    gl::printGlError("glViewport");
}

void gl::clear(GLbitfield mask)
{
    glClear(mask);
    gl::printGlError("glClear");
}

void gl::setClearColor(float r, float g, float b, float alpha)
{
    glClearColor(r, g, b, alpha);
    gl::printGlError("glClearColor");
}

void gl::vertexarray::enableVertexAttribute(GLuint index)
{
    glEnableVertexAttribArray(index);
    gl::printGlError("glEnableVertexAttribArray");
}

void gl::enable(GLenum capacity)
{
    glEnable(capacity);
    gl::printGlError("glEnable");
}

void gl::disable(GLenum capacity)
{
    glDisable(capacity);
    gl::printGlError("glDisable");
}

void gl::setBlendFunction(GLenum sfactor, GLenum dfactor)
{
    glBlendFunc(sfactor, dfactor);
    gl::printGlError("glBlendFunc");
}
