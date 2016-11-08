#include "gldebug.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include "Logger/logger.h"
#define ENGINE_GL_DEBUG_MESSAGES

#ifdef ENGINE_GL_DEBUG_MESSAGES
void gl::printGlError(const std::string& message)
{
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        Logger::Log("GL ERROR in " + message + ":");
        Logger::Log((const char*)gluErrorString(err));
    }
}
#else
void printGlError(const std::string&)
{
}
#endif
