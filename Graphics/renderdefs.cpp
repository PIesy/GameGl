#include "renderdefs.h"
#include "../Logger/logger.h"

GLuint convertToGlShaderType(ShaderType type)
{
    switch(type)
    {
        case ShaderType::VertexShader:
            return GL_VERTEX_SHADER;
        case ShaderType::FragmentShader:
            return GL_FRAGMENT_SHADER;
        case ShaderType::GeometryShader:
            return GL_GEOMETRY_SHADER;
    }
}

void initGlew()
{
    GLenum glew_err;

    glewExperimental = 1;
    glew_err = glewInit();
    if(glew_err != GLEW_OK)
    {
        Logger::Log("GLEW ERROR");
        Logger::Log((const char*)glewGetErrorString(glew_err));
    }
    glew_err = glGetError();
}

void printContextInfo()
{
    int major, minor, shared;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
    SDL_GL_GetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, &shared);
    Logger::Log("Context info: version = " + std::to_string(major) + "."
                + std::to_string(minor) + ", isShared = " + std::to_string(shared == 1));
}
