#ifndef GAMECORE_H
#define GAMECORE_H

#include <unordered_map>
#include "gamecoreinterface.h"
#include "game.h"

class GameCore: public GameCoreInterface
{
    StateData data;
    CoreInterface engine;
    GameStates state = GameStates::MainMenu;
    std::unordered_map<int, GameState*> states;
    GameState* savedState = nullptr;
    GameState* currentState = nullptr;
    void init();
public:
    GameCore();
    ~GameCore();
    void setState(GameStates state);
    void RestoreState();
    void AddState(void* state, GameStates name);
    void saveState();
    void Start();
    int WaitEnd();
    CoreInterface* getEngine();
};

#endif // GAMECORE_H
