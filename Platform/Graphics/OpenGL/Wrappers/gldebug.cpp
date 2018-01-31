#include "gldebug.h"
#include "../../../../Logger/logger.h"

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                   const void*);

const auto logger = Logger::GetLogger("OpenGl");

#define ENGINE_GL_DEBUG_MESSAGES

#ifdef ENGINE_GL_DEBUG_MESSAGES

void gl::printGlError(const std::string& message)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        logger.LogError("GL ERROR in "+ message + ":");
        logger.LogError((const char*) gluErrorString(err));
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
void gl::printGlError(const std::string&)
{
}

void gl::registerDebugCallback()
{
}
#endif

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                   const void*)
{
    std::string logMessage = "GL Debug " + std::to_string(id) + ": "+ message;
    LogMessageSeverity logSeverity = LogMessageSeverity::Info;

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
            logSeverity = LogMessageSeverity::Error;
            logMessage += "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            logSeverity = LogMessageSeverity::Warning;
            logMessage += "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            logSeverity = LogMessageSeverity::Warning;
            logMessage += "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            logSeverity = LogMessageSeverity::Warning;
            logMessage += "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            logSeverity = LogMessageSeverity::Warning;
            logMessage += "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            logSeverity = LogMessageSeverity::Info;
            logMessage += "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            logSeverity = LogMessageSeverity::Info;
            logMessage += "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            logSeverity = LogMessageSeverity::Info;
            logMessage += "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            logSeverity = LogMessageSeverity::Info;
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
    logger.Log(logMessage, logSeverity);
}
