#ifndef GAMECOREINTERFACE_H
#define GAMECOREINTERFACE_H

#include "../Public/coreinterface.h"
#include "../Public/ui.h"

enum class GameStates:int {MainMenu, Game};

class GameCoreInterface
{
public:
    virtual ~GameCoreInterface(){}
    virtual void RestoreState() = 0;
    virtual void setState(GameStates state) = 0;
    virtual void AddState(void* state, GameStates name) = 0;
    virtual void saveState() = 0;
    virtual void Start() = 0;
    virtual CoreInterface* getEngine() = 0;
};

struct StateData
{
    SDL_Window* window = nullptr;
    GameCoreInterface* game;
};

#endif // GAMECOREINTERFACE_H
