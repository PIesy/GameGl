#ifndef RENDERDEFS_H
#define RENDERDEFS_H

#include "GL/glew.h"
#include "GL/glu.h"
#include "SDL2/SDL.h"
#include "../Core/worker.h"
#include "graphicsapi.h"

GLuint convertToGlShaderType(ShaderType type);

struct DummyData
{
    std::mutex lock;
    std::condition_variable finish;
    SDL_GLContext context;
    SDL_Window* window;
};

class GlShader: public Shader
{
    Worker* context;
    GLuint shader = 0;
    ShaderType type;
public:
    GlShader(Worker* context);
    void Create(std::string source, ShaderType type);
    ShaderType getType() const;
    operator GLuint() const;
};

class GlProgram: public Program
{
    std::mutex lock;
    Worker* context;
    GLuint program = 0;
public:
    GlProgram(Worker* context);
    void Attach(const Shader& shader);
    void Detach(const Shader& shader);
    void Compile();
    void Use();
    operator GLuint() const;
};

struct GraphicsData
{
    std::mutex resizing;
    SDL_Window* window = NULL;
    SDL_GLContext context;
    SDL_GLContext dummyContext;
    int dimensions[2];
    Worker worker;
    bool resolutionChanged = false;
};

#endif // RENDERDEFS_H
