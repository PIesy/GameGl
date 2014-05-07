#include "gamecore.h"

GameCore::GameCore()
{
    data.game = this;
    init();
}

void GameCore::setState(GameStates state)
{
    currentState = states.at(integral(state));
    if(currentState)
        currentState->Activate();
}

void GameCore::RestoreState()
{
    if(currentState)
        currentState->Destroy();
    currentState = savedState;
    if(currentState)
        currentState->Activate();
    savedState = nullptr;
}

void GameCore::saveState()
{
    if(currentState)
        currentState->Destroy(true);
    savedState = currentState;
    currentState = nullptr;
}


GameCore::~GameCore()
{
}

void GameCore::Start()
{
    engine.Start();
    data.window = (SDL_Window*)engine.Video()->CreateWindow("Polia srazhenii", 600, 600);
    setState(GameStates::MainMenu);
}

void GameCore::AddState(void *state, GameStates name)
{
    std::pair<int, GameState*> pair(integral(name), (GameState*)state);
    states.insert(pair);
}

CoreInterface* GameCore::getEngine()
{
    return &engine;
}

void GameCore::init()
{
    GameState* menu = new MainMenu(&data);
    GameState* game = new Game(&data);
    AddState(menu, GameStates::MainMenu);
    AddState(game, GameStates::Game);
}

int GameCore::WaitEnd()
{
    return engine.WaitEnd();
}
