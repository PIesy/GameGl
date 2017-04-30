#ifndef RENDERDEFS_H
#define RENDERDEFS_H

#include "GL/glew.h"
#include "GL/glu.h"
#include "SDL2/SDL.h"
#include "../Core/worker.h"
#include "graphicsapi.h"
#include "../Platform/Graphics/OpenGL/sdlclasses.h"

GLuint convertToGlShaderType(ShaderType type);

void initGlew();
void printContextInfo();

#endif // RENDERDEFS_H
