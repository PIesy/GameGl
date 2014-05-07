#include "gamestates.h"
#include <iostream>

void endGame(void*, void* o);
void startGame(void*, void* menu);

MainMenu::MainMenu(StateData *data)
{
    this->data = data;
    ui = new UiLayer(data->game->getEngine()->getCore(), data->window);
    init();
}

MainMenu::~MainMenu()
{

}

void MainMenu::init()
{
    Button* startBtn = new Button(200, 50);
    Button* closeBtn = new Button(200, 50, {1,0,0,1});
    Action theEnd(endGame, data->game->getEngine());
    Action start(startGame, this);

    ui->AddElement(startBtn);
    ui->AddElement(closeBtn);
    closeBtn->setAction(Events::onClick, theEnd);
    startBtn->setAction(Events::onClick, start);
    startBtn->setPosition(200, 275);
    closeBtn->setPosition(200, 200);
}

void MainMenu::Activate()
{
    static int r = 0;
    Scene* scene = new Scene;
    int i = 0;
    Program* program = data->game->getEngine()->Video()->CreateProgram();
    Shader* shaders[2];

    shaders[0] = data->game->getEngine()->Video()->CreateShader("VertexShader.glsl", GL_VERTEX_SHADER);
    shaders[1] = data->game->getEngine()->Video()->CreateShader("FragmentShader.glsl", GL_FRAGMENT_SHADER);
    program->Attach(shaders[0]);
    program->Attach(shaders[1]);
    program->Compile();
    scene->objects = ui->getGraphics();
    scene->passes = ui->getCount();
    for (i = 0; i < scene->passes; i++)
        scene->objects[i]->data()->program = program;
    std::cout << r << std::endl;
    data->game->getEngine()->Video()->SendScene(scene);
    r++;
}

void MainMenu::ChangeState(GameStates state)
{
    data->game->setState(state);
}

void MainMenu::Destroy(bool save)
{
    return;
}

void MainMenu::Move(GameState *target)
{
    return;
}

void endGame(void*, void *o)
{
    CoreInterface* engine = (CoreInterface*)o;
    engine->Stop();
}

void startGame(void *, void *menu)
{
    MainMenu* m = (MainMenu*)menu;

    m->ChangeState(GameStates::Game);
}
