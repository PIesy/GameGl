#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Game/gamecore.h"

using namespace std;

void updateViewport(void*e, void* core);
void closeApp(void*, void* o);
void drawTriangle(CoreInterface* engine, UiLayer* ui);

int main()
{
    //CoreInterface engine;
    GameCore game;
    game.Start();
//    SDL_Window** window = new SDL_Window*;
//    Action setViewport(updateViewport, &engine);
//    Action endGame(closeApp, &engine);

//    engine.Start();
//    engine.BindAction(setViewport, SDL_WINDOWEVENT_RESIZED, 0);
//    engine.BindAction(endGame, SDL_SCANCODE_Q, 1);
//    engine.BindAction(endGame, SDL_WINDOWEVENT_CLOSE, 0);
    //*window = (SDL_Window*)(engine.Video()->CreateWindow("Hello", 1000, 600));
    //UiLayer ui(engine.getCore(), *window);
    //drawTriangle(&engine, &ui);
    return  game.WaitEnd();
}

void closeApp(void*, void *o)
{
    cout << "The end is comming\n";
    CoreInterface* engine = (CoreInterface*)o;
    engine->Stop();
}

void updateViewport(void *e, void *core)
{
    SDL_Event* ev = (SDL_Event*)e;
    CoreInterface* engine = (CoreInterface*)core;
    engine->Video()->setViewport(0, 0, ev->window.data1, ev->window.data2);
}

void drawTriangle(CoreInterface* engine, UiLayer* ui)
{
    Scene* scene = new Scene;
    Shader* shaders[2];
    Program* program;
    Button* box = new Button(200, 50);
    Button* box2 = new Button(200, 50, {0,1,1,1});
    Action endGame(closeApp, engine);
    VertexObject* frame = new VertexObject(Shapes::Triangle());
    frame->Append(Shapes::Hexagon());

    ui->AddElement(box);
    ui->AddElement(box2);
    box->setPosition(400, 275);
    box2->setPosition(400, 200);
    box->setAction(Events::onClick, endGame);
    program = engine->Video()->CreateProgram();
    shaders[1] = engine->Video()->CreateShader("VertexShader.glsl", GL_VERTEX_SHADER);
    shaders[2] = engine->Video()->CreateShader("FragmentShader.glsl", GL_FRAGMENT_SHADER);
    program->Attach(shaders[1]);
    program->Attach(shaders[2]);
    program->Compile();
    scene->passes = 1;
    *scene->objects = new VertexObject[2];
    scene->objects[0] = box->getGraphics();
    scene->objects[0]->data()->program = program;
    scene->objects[1] = frame;
    scene->objects[1]->data()->program = program;
    engine->Video()->SendScene(scene);
}
