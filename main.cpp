#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Logger/logger.h"
#include "Tests/tests.h"
#include "Public/shaderreader.h"
#include "Public/meshreader3ds.h"
#include <functional>
#include <memory>

Mat4 pers;
float aspectFactor;

void updateViewport(WindowEvent* e, Renderer* renderer);
void closeApp(void*, CoreInterface* core);
void drawTriangle(CoreInterface* engine, UiLayer* ui, Renderer& renderer);
void test();
void switchPerspective(void*,Program* prog);
void shiftKeyboard(KeyboardEvent* event, Program* prog);
void zoom(KeyboardEvent* event, Program* prog);
void cubeSwitch(void*, VertexObject* cube, VertexObject* mesh, Renderer* renderer);

int main()
{
    CoreInterface engine;
    Renderer& renderer = engine.Video()->GetRenderer();
    Action<WindowEvent*> setViewport(updateViewport, std::placeholders::_1, &renderer);
    Action<WindowEvent*> endGame(closeApp, std::placeholders::_1, &engine);
    aspectFactor = 1000.0f / 600.0f;
    pers = perspective;
    engine.Start();
    eventsHandlerTest();
    engine.getEventHandler().setListener<WindowEvent>(setViewport, [](EventInterface* e) { return e->getHint() == integral(WindowData::Type::Resize); });
    engine.getEventHandler().setListener<WindowEvent>(endGame, [](EventInterface* e) { return e->getHint() == integral(WindowData::Type::Close); });
    Window window = engine.Video()->CreateWindow("Hello", 1000, 600);
    pers[0][0] = pers[0][0] / aspectFactor;
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

void switchPerspective(void *, Program *prog)
{
    static bool persp = true;
    if(persp)
    {
        persp = false;
        prog->SetPerspective(identity);
        Logger::Log("Perspective off");
    }
    else
    {
        persp = true;
        prog->SetPerspective(pers);
        Logger::Log("Perspective on");
    }
}

void zoom(KeyboardEvent *event, Program *prog)
{
    if(event->getPayload().scancode == SDL_SCANCODE_EQUALS)
    {
        pers[0][0] += 0.01 / aspectFactor;
        pers[1][1] += 0.01;
    }
    else
    {
        pers[0][0] -= 0.01 / aspectFactor;
        pers[1][1] -= 0.01;
    }
    prog->SetPerspective(pers);
}

void shiftKeyboard(KeyboardEvent *event, Program *prog)
{
    static Vec2 offset = {0, 0};
    if(event->getPayload().scancode == SDL_SCANCODE_LEFT)
        offset[0] -= 0.1;
    if(event->getPayload().scancode == SDL_SCANCODE_RIGHT)
        offset[0] += 0.1;
    if(event->getPayload().scancode == SDL_SCANCODE_UP)
        offset[1] += 0.1;
    if(event->getPayload().scancode == SDL_SCANCODE_DOWN)
        offset[1] -= 0.1;
    prog->SetOffset(offset);
}

void cubeSwitch(void*,VertexObject *cube, VertexObject *mesh, Renderer* renderer)
{
    static bool isCube = false;
    if(isCube)
    {
        isCube = false;
        Scene scene;
        scene.objects.push_back(mesh);
        scene.passes = 1;
        renderer->Draw(scene);
    }
    else
    {
        isCube = true;
        Scene scene;
        scene.objects.push_back(cube);
        scene.passes = 1;
        renderer->Draw(scene);
    }
}

void drawTriangle(CoreInterface* engine, UiLayer* ui, Renderer& renderer)
{
    ShaderReader reader;
    MeshReader3ds meshReader;
    engine->Resources().ReadByTemplate("test_2.obj", meshReader);
    VertexObject* mesh = new VertexObject(meshReader.getResult());
    Scene* scene = new Scene();
    engine->Resources().ReadByTemplate("VertexShader.glsl", reader);
    std::string vertexsrc = reader.getResult();
    engine->Resources().ReadByTemplate("FragmentShader.glsl", reader);
    std::string fragsrc = reader.getResult();
    Shader& shader1 = engine->Video()->CreateShader(vertexsrc, ShaderType::VertexShader);
    Shader& shader2 = engine->Video()->CreateShader(fragsrc, ShaderType::FragmentShader);
    Program& program = engine->Video()->CreateProgram();
    Button* box = new Button(200, 50);
    VertexObject* frame = new VertexObject(Shapes::Box());
    Action<MouseEvent*> endGame(switchPerspective, std::placeholders::_1, &program);
    Action<KeyboardEvent*> zoomObj(zoom, std::placeholders::_1, &program);
    Action<KeyboardEvent*> move(shiftKeyboard, std::placeholders::_1, &program);
    Action<KeyboardEvent*> cube(cubeSwitch, std::placeholders::_1, frame, mesh, &renderer);
    engine->getEventHandler().setListener<KeyboardEvent>(move);
    engine->getEventHandler().setListener<KeyboardEvent>(zoomObj, [](EventInterface* e) { return e->getHint() == SDL_SCANCODE_EQUALS ||
                e->getHint() == SDL_SCANCODE_MINUS; });
    engine->getEventHandler().setListener<KeyboardEvent>(cube, [](EventInterface* e) { return e->getHint() == SDL_SCANCODE_Q; });

    ui->AddElement(box);
    box->setPosition(400, 275);
    box->setAction(Events::onClick, endGame);
    program.Attach(shader1);
    program.Attach(shader2);
    program.Compile();
    program.SetOffset({0.0, 0.0});
    program.SetPerspective(pers);
    program.SetRotation(rotation);
    program.SetLight(light);
    program.SetIntensity(1);
    scene->passes = 1;
    scene->objects.push_back(mesh);
    mesh->data().program = &program;
    frame->data().program = &program;
    scene->objects[0]->data().program = &program;
    renderer.Draw(*scene);
}
