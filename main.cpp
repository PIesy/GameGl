#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Logger/logger.h"
#include "Tests/tests.h"
#include "Public/shaderreader.h"
#include <functional>
#include <memory>

void updateViewport(WindowEvent* e, Renderer* renderer);
void closeApp(void*, CoreInterface* core);
void drawTriangle(CoreInterface* engine, UiLayer* ui, Renderer& renderer);
void test();

int main()
{
    CoreInterface engine;
    Renderer& renderer = engine.Video()->GetRenderer();
    Action<WindowEvent*> setViewport(updateViewport, std::placeholders::_1, &renderer);
    Action<WindowEvent*> endGame(closeApp, std::placeholders::_1, &engine);

    engine.Start();
    eventsHandlerTest();
    engine.getEventHandler().setListener<WindowEvent>(setViewport, [](EventInterface* e) { return e->getHint() == integral(WindowData::Type::Resize); });
    engine.getEventHandler().setListener<WindowEvent>(endGame, [](EventInterface* e) { return e->getHint() == integral(WindowData::Type::Close); });
    Window window = engine.Video()->CreateWindow("Hello", 1000, 600);
    renderer.SetWindow(window);
    UiLayer ui(engine.getCore(), window);
    drawTriangle(&engine, &ui, renderer);
    return engine.WaitEnd();
}

void closeApp(void*, CoreInterface* core)
{
    Logger::Log("The end is comming");
    core->Stop();
}

void updateViewport(WindowEvent* e, Renderer* renderer)
{
    renderer->SetViewport(e->getPayload().coordinates[0], e->getPayload().coordinates[1]);
}

void drawTriangle(CoreInterface* engine, UiLayer* ui, Renderer& renderer)
{
    ShaderReader reader;
    Scene* scene = new Scene();
    engine->Resources().ReadByTemplate("VertexShader.glsl", reader);
    std::string vertexsrc = reader.getResult();
    engine->Resources().ReadByTemplate("FragmentShader.glsl", reader);
    std::string fragsrc = reader.getResult();
    Shader& shader1 = engine->Video()->CreateShader(vertexsrc, ShaderType::VertexShader);
    Shader& shader2 = engine->Video()->CreateShader(fragsrc, ShaderType::FragmentShader);
    Program& program = engine->Video()->CreateProgram();
    Button* box = new Button(200, 50);
    Button* box2 = new Button(200, 50, {0,1,1,1});
    Action<MouseEvent*> endGame(closeApp, std::placeholders::_1, engine);
    VertexObject* frame = new VertexObject(Shapes::Triangle());

    ui->AddElement(box);
    ui->AddElement(box2);
    box->setPosition(400, 275);
    box2->setPosition(400, 200);
    box->setAction(Events::onClick, endGame);
    program.Attach(shader1);
    program.Attach(shader2);
    program.Compile();
    program.SetOffset({0.2, 0.2});
    program.SetPerspective(perspective);
    scene->passes = 1;
    scene->objects.push_back(box->getGraphics());
    scene->objects[0]->data().program = &program;
    scene->objects.push_back(frame);
    scene->objects[1]->data().program = &program;
    renderer.Draw(*scene);
}
