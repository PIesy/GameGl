#include "gldebug.h"
#include "../../../../Logger/logger.h"

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                   const void*);

#define ENGINE_GL_DEBUG_MESSAGES

#ifdef ENGINE_GL_DEBUG_MESSAGES

void gl::printGlError(const std::string& message)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        Logger::Log("GL ERROR in "+ message + ":");
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
                   const void*)
{
    // ignore non-significant error/warning codes
    //if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::string logMessage = "OpenGL Debug message " + std::to_string(id) + ": "+ message;

    logMessage += " | ";

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            logMessage += "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            logMessage += "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            logMessage += "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            logMessage += "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            logMessage += "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            logMessage += "Source: Other";
            break;
    }

    logMessage += " | ";

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            logMessage += "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            logMessage += "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            logMessage += "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            logMessage += "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            logMessage += "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            logMessage += "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            logMessage += "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            logMessage += "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            logMessage += "Type: Other";
            break;
    }

    logMessage += " | ";

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            logMessage += "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            logMessage += "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            logMessage += "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            logMessage += "Severity: notification";
            break;
    }
    Logger::Log(logMessage);
}
