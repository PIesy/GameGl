//
// Created by akann on 10/6/17.
//

#include "gl_generic.h"
#include "Wrappers/glbindings.h"
#include "../../../Logger/logger.h"
#include "SDL2/SDL.h"

const auto logger = Logger::GetLogger("glGeneric");

void gl::initGlew()
{
    GLenum glew_err;

    glewExperimental = 1;
    glew_err = glewInit();
    if(glew_err != GLEW_OK)
        logger.LogError((const char*)glewGetErrorString(glew_err));
    glGetError();
}

void printContextInfo()
{
    int major, minor, shared;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
    SDL_GL_GetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, &shared);
    logger.LogInfo("Context info: version = " + std::to_string(major) + "."
                   + std::to_string(minor) + ", isShared = " + std::to_string(shared == 1));
}