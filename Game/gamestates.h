#ifndef GAMESTATES_H
#define GAMESTATES_H

#include "gamecoreinterface.h"

class GameState
{
protected:
    StateData* data;
public:
    virtual ~GameState(){}
    virtual void Destroy(bool save = false) = 0;
    virtual void Activate() = 0;
    virtual void ChangeState(GameStates state) = 0;
    virtual void Move(GameState* target) = 0;
};

class MainMenu: public GameState
{
    UiLayer* ui;
    void drawMenu();
    void init();
public:
    MainMenu(StateData* data);
    ~MainMenu();
    void Destroy(bool save = false);
    void Activate();
    void ChangeState(GameStates state);
    void Move(GameState *target);
};

#endif // GAMESTATES_H
