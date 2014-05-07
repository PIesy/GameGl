#ifndef GAME_H
#define GAME_H

#include "gamestates.h"


struct Player
{
    RGBA_Color playerColor;
    Player* enemy;
};

class GridItem: public Button
{
    bool selected = false;
    bool empty = true;
    Player* owner = nullptr;
    int location[2];
public:
    GridItem();
    ~GridItem(){}
    void setPlayer(Player* player);
    void setLocation(int x, int y);
    void setEmpty();
    void Select();
    bool isEnemy(Player* player);
    bool isOwner(Player* player);
    bool isEmpty();

    friend class GameLogic;
};

class GameLogic
{
    UiLayer* gridLayer;
    GridItem grid[10][10];
    short currentPlayer = 0;
    GridItem* selectedItem = nullptr;
    Player players[2];
    void initPlayers();
    void initGrid();
    bool checkDistance(GridItem* item);
    bool checkCapturable(GridItem* item);
public:
    GameLogic(StateData* data);
    void ChangeTurn();
    void MoveItem(int x, int y);
    void GetControl(int x, int y);
    void Analyze(GridItem* item);
    VertexObject** getGrid();
    int getItemsCount();
};

class Game: public GameState
{
    GameLogic* game;
public:
    Game(StateData* data);
    ~Game(){}
    void Destroy(bool save = false);
    void Activate();
    void ChangeState(GameStates state);
    void Move(GameState *target);
};

#endif // GAME_H
