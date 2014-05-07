#include "renderer.h"

void StartRenderer(RenderInitializer* initializer)
{
    Renderer renderer(initializer->queue, initializer->data);
    renderer.Start();
}

Renderer::Renderer(RenderQueue* queue, GraphicsData* data)
{
    this->queue = queue;
    this->graphicsData = data;
    init();
}

void Renderer::Start()
{
    while(!terminate)
    {
        render();
        draw();
        SDL_Delay(1);
    }
}

void Renderer::init()
{
    glClearColor(0,0,0,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenBuffers(1, &data.VBO);
    glGenBuffers(1, &data.IBO);
    glGenVertexArrays(1, &data.VAO);
    glBindVertexArray(data.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.IBO);
    glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 100 * sizeof(int), NULL, GL_DYNAMIC_DRAW);
}

void Renderer::render()
{
    int i;
    if (graphicsData->resolutionChanged)
    {
        graphicsData->resizing.lock();
        glViewport(0, 0, graphicsData->dimensions[0], graphicsData->dimensions[1]);
        graphicsData->resizing.unlock();
    }
    if (!queue->IsEmpty())
    {
        if(currentScene) delete currentScene;
        currentScene = queue->Pop();
    }
    if(currentScene)
        for (i = 0; i < currentScene->passes; i ++)
        {
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            currentScene->objects[i]->data()->vertexCount * sizeof(Vertex),
                            currentScene->objects[i]->data()->vertices);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, coords));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, color));
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                            currentScene->objects[i]->data()->indices.count * sizeof(int),
                            currentScene->objects[i]->data()->indices.indices);
            currentScene->objects[i]->data()->program->Use(nullptr);
            glDrawElements(GL_TRIANGLES, currentScene->objects[i]->data()->indices.count, GL_UNSIGNED_INT, nullptr);
        }
}

void Renderer::draw()
{
    SDL_GL_SwapWindow(graphicsData->window);
    glClear(GL_COLOR_BUFFER_BIT);
}
