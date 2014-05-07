#ifndef RENDERDEFS_H
#define RENDERDEFS_H

#include "GL/glew.h"
#include "GL/glu.h"
#include "SDL2/SDL.h"
#include "../Core/worker.h"
#include "graphicsapi.h"

struct DummyData
{
    std::mutex lock;
    std::condition_variable finish;
    SDL_GLContext context;
    SDL_Window* window;
};

class GlShader: public Shader
{
    std::mutex lock;
    Worker* context;
    GLuint shader = 0;
    std::string* str = nullptr;
    const char* src = nullptr;
    int length = 0;
    void prepareShader(std::string source);
    void cleanup();
public:
    GlShader(Worker* context);
    ~GlShader();
    void Destroy();
    void Create(std::string source, unsigned int type);
    operator unsigned int();
};

class GlProgram: public Program
{
    std::mutex lock;
    Worker* context;
    GLuint program = 0;
public:
    GlProgram(Worker* context);
    ~GlProgram();
    void Attach(Shader* shader);
    void Detach(Shader* shader);
    void Compile();
    void Use(void * attr = nullptr);
    operator unsigned int();
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
