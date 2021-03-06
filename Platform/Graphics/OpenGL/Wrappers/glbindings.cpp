#include <memory>
#include "glbindings.h"
#include "gldebug.h"


//vertex array

void gl::vertex_array::generate(int count, GLuint* vertexArrays)
{
    glCreateVertexArrays(count, vertexArrays);
    gl::printGlError("glGenVertexArrays");
}

void gl::vertex_array::bind(GLuint vertexArray)
{
    glBindVertexArray(vertexArray);
    gl::printGlError("glBindVertexArray");
}

void gl::vertex_array::erase(int count, GLuint* vertexArrays)
{
    glDeleteVertexArrays(count, vertexArrays);
    gl::printGlError("glDeleteVertexArrays");
}

void gl::vertex_array::disableVertexAttribute(GLuint target, GLuint index)
{
    glDisableVertexArrayAttrib(target, index);
    gl::printGlError("glDisableVertexArrayAttrib");
}

void gl::vertex_array::setVertexAttributeFormat(GLuint target, GLuint index, int size, GLenum type, GLboolean normalized, GLuint offset)
{
    glVertexArrayAttribFormat(target, index, size, type, normalized, offset);
    gl::printGlError("glVertexArrayAttribFormat");
}

void gl::vertex_array::enableVertexAttribute(GLuint target, GLuint index)
{
    glEnableVertexArrayAttrib(target, index);
    gl::printGlError("glEnableVertexArrayAttrib");
}

void gl::vertex_array::bindElementBuffer(GLuint target, GLuint buffer)
{
    glVertexArrayElementBuffer(target, buffer);
    gl::printGlError("glVertexArrayElementBuffer");
}

void gl::vertex_array::bindVertexBuffer(GLuint target, GLuint bindingIndex, GLuint buffer, GLintptr offset, GLsizei stride)
{
    glVertexArrayVertexBuffer(target, bindingIndex, buffer, offset, stride);
    gl::printGlError("glVertexArrayVertexBuffer");
}

void gl::vertex_array::bindBufferToAttribute(GLuint target, GLuint attributeIndex, GLuint bindingIndex)
{
    glVertexArrayAttribBinding(target, attributeIndex, bindingIndex);
    gl::printGlError("glVertexArrayAttribBinding");
}

//buffer

void gl::buffer::generate(int count, GLuint* buffers)
{
    glCreateBuffers(count, buffers);
    gl::printGlError("glGreateBuffers");
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

void gl::buffer::updateData(GLuint target, GLintptr offset, size_t size, const void* data)
{
    glNamedBufferSubData(target, offset, size, data);
    gl::printGlError("glNamedBufferSubData");
}

void gl::buffer::setStorage(GLuint target, size_t size, const void* data, GLbitfield flags)
{
    glNamedBufferStorage(target, size, data, flags);
    gl::printGlError("glNamedBufferStorage");
}

//texture

void gl::texture::generate(int count, GLuint* textures, GLenum target)
{
    glCreateTextures(target, count, textures);
    gl::printGlError("glCreateTextures");
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

void gl::texture::loadImage(GLuint target, GLint mipmapLevel, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
                            GLenum format, GLenum type, const void* data)
{
    glTextureSubImage2D(target, mipmapLevel, xoffset, yoffset, width, height, format, type, data);
    gl::printGlError("glTextureImage2D");
}

void gl::texture::setParameter(GLuint target, GLenum parameter, GLint value)
{
    glTextureParameteri(target, parameter, value);
    gl::printGlError("glTextureParameteri");
}

void gl::texture::setParameter(GLuint target, GLenum parameter, GLfloat value)
{
    glTextureParameterf(target, parameter, value);
    gl::printGlError("glTextureParameterf");
}

void gl::texture::allocateStorage(GLuint target, GLsizei mipmapLevels, GLenum internalFormat, GLsizei width, GLsizei height)
{
    glTextureStorage2D(target, mipmapLevels, internalFormat, width, height);
    gl::printGlError("glTextureStorage2D");
}

void gl::texture::setActiveTexture(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    gl::printGlError("glTextureStorage2D");
}

void gl::texture::allocateStorage(GLuint target, GLsizei mipmapLevels, GLenum internalFormat, GLsizei width,
                                    GLsizei height, GLsizei depth)
{
    glTextureStorage3D(target, mipmapLevels, internalFormat, width, height, depth);
    gl::printGlError("glTextureStorage3D");
}

void gl::texture::loadImage(GLuint target, GLint mipmapLevel, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width,
                         GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* data)
{
    glTextureSubImage3D(target, mipmapLevel, xoffset, yoffset, zoffset, width, height, depth, format, type, data);
    gl::printGlError("glTextureImage3D");
}

void gl::texture::generateMipmaps(GLuint target)
{
    glGenerateTextureMipmap(target);
    gl::printGlError("glGenerateTextureMipmap");
}

//framebuffer

void gl::framebuffer::generate(int count, GLuint* framebuffers)
{
    glCreateFramebuffers(count, framebuffers);
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

void gl::framebuffer::setRenderbuffer(GLuint target, GLenum attachment, GLuint buffer)
{
    glNamedFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, buffer);
    gl::printGlError("glFramebufferRenderbuffer");
}

void gl::framebuffer::setTexture(GLuint target, GLenum attachment, GLuint texture, GLint level)
{
    glNamedFramebufferTexture(target, attachment, texture, level);
    gl::printGlError("glNamedFramebufferTexture");
}

GLenum gl::framebuffer::checkStatus(GLuint target, GLenum bindPoint)
{
    GLenum result = glCheckNamedFramebufferStatus(target, bindPoint);
    gl::printGlError("glCheckNamedFramebufferStatus");
    return result;
}

void gl::framebuffer::setDrawBuffers(GLuint target, GLsizei count, const GLenum* buffers)
{
    glNamedFramebufferDrawBuffers(target, count, buffers);
    gl::printGlError("glNamedFramebufferDrawBuffers");
}

void gl::framebuffer::blit(GLuint src, GLuint target, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint targetX0, GLint targetY0, GLint targetX1,
                           GLint targetY1, GLbitfield bufferMask, GLenum filter)
{
    glBlitNamedFramebuffer(src, target, srcX0, srcY0, srcX1, srcY1, targetX0, targetY0, targetX1, targetY1, bufferMask, filter);
    gl::printGlError("glBlitNamedFramebuffer");
}

//renderbuffer

void gl::renderbuffer::generate(int count, GLuint* renderbuffers)
{
    glCreateRenderbuffers(count, renderbuffers);
    gl::printGlError("glCreateRenderbuffers");
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

void gl::renderbuffer::setStorage(GLuint target, GLenum component, unsigned width, unsigned height)
{
    glNamedRenderbufferStorage(target, component, width, height);
    gl::printGlError("glNamedRenderbufferStorage");
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
    auto size = (int)source.length();
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

GLuint gl::program::create(GLenum type, const std::string& source)
{
    const char* text = source.c_str();
    GLuint result = glCreateShaderProgramv(type, 1, &text);
    gl::printGlError("glCreateShaderProgramv");
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


GLint gl::program::getUniformLocation(GLuint program, const std::string& uniformName)
{
    GLint result = glGetUniformLocation(program, uniformName.c_str());
    gl::printGlError("glGetUniformLocation");
    return result;
}

void gl::program::setUniform1f(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform1fv(program, location, count, (const GLfloat*)value);
    gl::printGlError("glProgramUniform1fv");
}

void gl::program::setUniform2f(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform2fv(program, location, count, (const GLfloat*)value);
    gl::printGlError("glProgramUniform2fv");
}

void gl::program::setUniform3f(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform3fv(program, location, count, (const GLfloat*)value);
    gl::printGlError("glProgramUniform3fv");
}

void gl::program::setUniform4f(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform4fv(program, location, count, (const GLfloat*)value);
    gl::printGlError("glProgramUniform4fv");
}

void gl::program::setUniform1i(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform1iv(program, location, count, (const GLint*)value);
    gl::printGlError("glProgramUniform1iv");
}

void gl::program::setUniform2i(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform2iv(program, location, count, (const GLint*)value);
    gl::printGlError("glProgramUniform2iv");
}

void gl::program::setUniform3i(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform3iv(program, location, count, (const GLint*)value);
    gl::printGlError("glProgramUniform3iv");
}

void gl::program::setUniform4i(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform4iv(program, location, count, (const GLint*)value);
    gl::printGlError("glProgramUniform4iv");
}

void gl::program::setUniform1ui(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform1uiv(program, location, count, (const GLuint*)value);
    gl::printGlError("glProgramUniform1uiv");
}

void gl::program::setUniform2ui(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform2uiv(program, location, count, (const GLuint*)value);
    gl::printGlError("glProgramUniform2uiv");
}

void gl::program::setUniform3ui(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform3uiv(program, location, count, (const GLuint*)value);
    gl::printGlError("glProgramUniform3uiv");
}

void gl::program::setUniform4ui(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform4uiv(program, location, count, (const GLuint*)value);
    gl::printGlError("glProgramUniform4uiv");
}

void gl::program::setUniform1d(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform1dv(program, location, count, (const GLdouble*)value);
    gl::printGlError("glProgramUniform1dv");
}

void gl::program::setUniform2d(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform2dv(program, location, count, (const GLdouble*)value);
    gl::printGlError("glProgramUniform2dv");
}

void gl::program::setUniform3d(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform3dv(program, location, count, (const GLdouble*)value);
    gl::printGlError("glProgramUniform3dv");
}

void gl::program::setUniform4d(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniform4dv(program, location, count, (const GLdouble*)value);
    gl::printGlError("glProgramUniform4dv");
}

void gl::program::setUniform2x2f(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniformMatrix2fv(program, location, count, GL_FALSE, (const GLfloat*)value);
    gl::printGlError("glProgramUniformMatrix2fv");
}

void gl::program::setUniform3x3f(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniformMatrix3fv(program, location, count, GL_FALSE, (const GLfloat*)value);
    gl::printGlError("glProgramUniformMatrix3fv");
}

void gl::program::setUniform4x4f(GLuint program, GLuint location, int count, const void *value)
{
    glProgramUniformMatrix4fv(program, location, count, GL_FALSE, (const GLfloat*)value);
    gl::printGlError("glProgramUniformMatrix4fv");
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

void gl::enable(GLenum capability)
{
    glEnable(capability);
    gl::printGlError("glEnable");
}

void gl::disable(GLenum capability)
{
    glDisable(capability);
    gl::printGlError("glDisable");
}

void gl::setBlendFunction(GLenum sfactor, GLenum dfactor)
{
    glBlendFunc(sfactor, dfactor);
    gl::printGlError("glBlendFunc");
}

void gl::flush()
{
    glFlush();
}

GLuint gl::program_pipeline::create()
{
    GLuint result = 0;
    glCreateProgramPipelines(1, &result);
    gl::printGlError("glCreateProgramPipelines");
    return result;
}

void gl::program_pipeline::erase(GLuint programPipeline)
{
    glDeleteProgramPipelines(1, &programPipeline);
    gl::printGlError("glDeleteProgramPipelines");
}

std::string gl::program_pipeline::getInfoLog(GLuint programPipeline)
{
    GLint infoLogLength = 0;
    glGetProgramPipelineiv(programPipeline, GL_INFO_LOG_LENGTH, &infoLogLength);
    gl::printGlError("glGetProgramPipelineiv");
    if (!infoLogLength)
        return "";

    auto result = std::unique_ptr<char[]>(new char[infoLogLength]);
    glGetProgramPipelineInfoLog(programPipeline, infoLogLength, nullptr, result.get());
    return result.get();
}

void gl::program_pipeline::useProgramStages(GLuint programPipeline, GLbitfield stages, GLuint program)
{
    glUseProgramStages(programPipeline, stages, program);
    gl::printGlError("glUseProgramStages");
}

void gl::program_pipeline::bindProgramPipeline(GLuint programPipeline)
{
    glBindProgramPipeline(programPipeline);
    gl::printGlError("glBindProgramPipeline");
}

void gl::program_pipeline::validate(GLuint programPipeline)
{
    glValidateProgramPipeline(programPipeline);
    gl::printGlError("glValidateProgramPipeline");
}

void gl::program_pipeline::getParameter(GLuint programPipeline, GLenum parameterName, GLint* result)
{
    glGetProgramPipelineiv(programPipeline, parameterName, result);
    gl::printGlError("glGetProgramPipelineiv");
}

GLsync gl::sync::createFence()
{
    return glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void gl::sync::erase(GLsync sync)
{
    glDeleteSync(sync);
}

void gl::sync::clientWait(GLsync sync, GLbitfield flags, uint64_t timeout)
{
    glClientWaitSync(sync, flags, timeout);
}

void gl::sync::wait(GLsync sync, GLbitfield flags, uint64_t timeout)
{
    glWaitSync(sync, flags, timeout);
}
