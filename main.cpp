#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Logger/logger.h"
#include "Tests/tests.h"
#include <functional>
#include <memory>

void updateViewport(WindowEvent* e, CoreInterface* core);
void closeApp(WindowEvent* e, CoreInterface* core);
void drawTriangle(CoreInterface* engine, UiLayer* ui);
void test();

int main()
{
    CoreInterface engine;
    SDL_Window* window;
    Action<WindowEvent*> setViewport(std::bind(updateViewport, std::placeholders::_1, &engine));
    Action<WindowEvent*> endGame(std::bind(closeApp, std::placeholders::_1, &engine));

    eventsHandlerTest();
    engine.Start();
    engine.getEventHandler().setListener<WindowEvent>(setViewport);
    engine.getEventHandler().setListener<WindowEvent>(endGame);
    window = (SDL_Window*)(engine.Video()->CreateWindow("Hello", 1000, 600));
    UiLayer ui(engine.getCore(), window);
    drawTriangle(&engine, &ui);
    Logger::Log("Hello");
    return  engine.WaitEnd();
}

void closeApp(WindowEvent* e, CoreInterface* core)
{
    if(e->getPayload().eventType == WindowData::Type::Close)
    {
        std::cout << "The end is comming\n";
        core->Stop();
    }
}

void updateViewport(WindowEvent* e, CoreInterface* core)
{
    core->Video()->setViewport(0, 0, e->getPayload().coordinates[0], e->getPayload().coordinates[1]);
}

void drawTriangle(CoreInterface* engine, UiLayer* ui)
{
    Scene* scene = new Scene();
    Shader* shaders[2];
    Program* program;
    Button* box = new Button(200, 50);
    Button* box2 = new Button(200, 50, {0,1,1,1});
    Action<WindowEvent*> endGame(std::bind(closeApp, std::placeholders::_1, engine));
    VertexObject* frame = new VertexObject(Shapes::Triangle());

    ui->AddElement(box);
    ui->AddElement(box2);
    box->setPosition(400, 275);
    box2->setPosition(400, 200);
    box->setAction(Events::onClick, getAction<WindowEvent*>(std::bind(closeApp, std::placeholders::_1, engine)));
    program = engine->Video()->CreateProgram();
    shaders[0] = engine->Video()->CreateShader("VertexShader.glsl", GL_VERTEX_SHADER);
    shaders[1] = engine->Video()->CreateShader("FragmentShader.glsl", GL_FRAGMENT_SHADER);
    program->Attach(shaders[0]);
    program->Attach(shaders[1]);
    program->Compile();
    scene->passes = 2;
    scene->objects = new VertexObject*[2];
    scene->objects[0] = box->getGraphics();
    scene->objects[0]->data()->program = program;
    scene->objects[1] = frame;
    scene->objects[1]->data()->program = program;
    engine->Video()->SendScene(scene);
}
