#include "gldebug.h"
#include <GL/glew.h>
#include "../../Logger/logger.h"

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                   const void* param);

#define ENGINE_GL_DEBUG_MESSAGES

#ifdef ENGINE_GL_DEBUG_MESSAGES

void gl::printGlError(const std::string& message)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        Logger::Log("GL ERROR in " + message + ":");
        Logger::Log((const char*) gluErrorString(err));
    }
}

void gl::registerDebugCallback()
{
    gl::enable(GL_DEBUG_OUTPUT);
    gl::enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    printGlError("gl debug");
}

#else
void printGlError(const std::string&)
{
}

void gl::registerDebugCallback()
{
}
#endif

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                   const void* param)
{
    // ignore non-significant error/warning codes
    //if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    Logger::Log("Debug message " + std::to_string(id) + ": " + message);

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            Logger::Log("Source: API");
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            Logger::Log("Source: Window System");
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            Logger::Log("Source: Shader Compiler");
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            Logger::Log("Source: Third Party");
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            Logger::Log("Source: Application");
            break;
        case GL_DEBUG_SOURCE_OTHER:
            Logger::Log("Source: Other");
            break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            Logger::Log("Type: Error");
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            Logger::Log("Type: Deprecated Behaviour");
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            Logger::Log("Type: Undefined Behaviour");
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            Logger::Log("Type: Portability");
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            Logger::Log("Type: Performance");
            break;
        case GL_DEBUG_TYPE_MARKER:
            Logger::Log("Type: Marker");
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            Logger::Log("Type: Push Group");
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            Logger::Log("Type: Pop Group");
            break;
        case GL_DEBUG_TYPE_OTHER:
            Logger::Log("Type: Other");
            break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            Logger::Log("Severity: high");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            Logger::Log("Severity: medium");
            break;
        case GL_DEBUG_SEVERITY_LOW:
            Logger::Log("Severity: low");
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            Logger::Log("Severity: notification");
            break;
    }
}
