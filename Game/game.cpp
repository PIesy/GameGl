#include "game.h"
#include <iostream>

GridItem::GridItem():Button(50, 50)
{
    SetColor({1,1,1,1});
    SetHoverColor({1,1,1,0.75f});
}

void GridItem::setPlayer(Player *player)
{
    RGBA_Color color = player->playerColor;

    color.color[3] = 1;
    owner = player;
    empty = false;
    SetColor(player->playerColor);
    SetHoverColor(color);
}

void GridItem::setLocation(int x, int y)
{
    location[0] = x;
    location[1] = y;
}

void GridItem::Select()
{
    RGBA_Color color = owner->playerColor;

    selected = !selected;
    if(selected)
        color.color[3] = 1;
    SetColor(color);
}

void GridItem::setEmpty()
{
    selected = false;
    owner = nullptr;
    empty = true;
    SetColor({1,1,1,1});
    SetHoverColor({1,1,1,0.75});
}

bool GridItem::isEnemy(Player* player)
{
    return player->enemy == owner;
}

bool GridItem::isOwner(Player *player)
{
    return player == owner;
}

bool GridItem::isEmpty()
{
    return empty;
}

void gridHandler(void* item, void* logic);

GameLogic::GameLogic(StateData* data)
{
    gridLayer = new UiLayer(data->game->getEngine()->getCore(), data->window);
    initPlayers();
    initGrid();
}

void GameLogic::initPlayers()
{
    players[0].playerColor = RGBA_Color{1,0,0,0.75};
    players[0].enemy = &players[1];
    players[1].playerColor = RGBA_Color{0,0,1,0.75};
    players[1].enemy = &players[0];
}

void GameLogic::initGrid()
{
    Action analyst(gridHandler, this);
    int i, j, offset_x = 51, offset_y = 51;

    for(i = 0; i < 10; i++)
        for(j = 0; j < 10; j++)
        {
            gridLayer->AddElement(&grid[i][j]);
            grid[i][j].setAction(Events::onClick, analyst);
            grid[i][j].setLocation(i, j);
            grid[i][j].setPosition(50 + offset_x * j, 50 + offset_y * i);
            if(i == 0) grid[i][j].setPlayer(&players[0]);
            if(i == 9) grid[i][j].setPlayer(&players[1]);
        }
}

void GameLogic::ChangeTurn()
{
    currentPlayer == 0 ? currentPlayer = 1 : currentPlayer = 0;
    selectedItem = nullptr;
}

void GameLogic::MoveItem(int x, int y)
{
    selectedItem->setEmpty();
    grid[x][y].setPlayer(&players[currentPlayer]);
}

void GameLogic::GetControl(int x, int y)
{
    grid[x][y].setPlayer(&players[currentPlayer]);
}

void GameLogic::Analyze(GridItem *item)
{
    if(selectedItem == item)
    {
        item->Select();
        selectedItem = nullptr;
        return;
    }
    if(!selectedItem && item->isOwner(&players[currentPlayer]))
    {
        selectedItem = item;
        item->Select();
        return;
    }
    if(selectedItem && item->isEmpty())
        if(checkDistance(item))
        {
            MoveItem(item->location[0], item->location[1]);
            ChangeTurn();
            return;
        }
    if(selectedItem && item->isEnemy(&players[currentPlayer]))
        if(checkDistance(item))
            if(checkCapturable(item))
            {
                GetControl(item->location[0], item->location[1]);
                selectedItem->Select();
                ChangeTurn();
            }
}

bool GameLogic::checkDistance(GridItem *item)
{
    bool yes = true;
    int x = selectedItem->location[0] - item->location[0];
    int y = selectedItem->location[1] - item->location[1];

    if (x > 1 || x < -1)
        yes = false;
    if (y > 1 || y < -1)
        yes = false;
    if ((x == 1 || x == -1) && (y == 1 || y == -1))
        yes = false;
    return yes;
}

bool GameLogic::checkCapturable(GridItem *item)
{
    bool yes = true;
    int x = selectedItem->location[0] - item->location[0];
    int y = selectedItem->location[1] - item->location[1];

    if(x == 0)
    {
        if((item->location[0] + 1) < 10)
            if(grid[item->location[1]][item->location[0] + 1].isEnemy(&players[currentPlayer]))
                yes = false;
        if((item->location[0] - 1) > 0)
            if(grid[item->location[1]][item->location[0] - 1].isEnemy(&players[currentPlayer]))
                yes = false;
    }
    if(y == 0)
    {
        if((item->location[1] + 1) < 10)
            if(grid[item->location[1] + 1][item->location[0]].isEnemy(&players[currentPlayer]))
                yes = false;
        if((item->location[1] - 1) > 0)
            if(grid[item->location[1] - 1][item->location[0]].isEnemy(&players[currentPlayer]))
                yes = false;
    }
    return yes;
}

VertexObject** GameLogic::getGrid()
{
    VertexObject** gr = gridLayer->getGraphics();
    return gr;
}

int GameLogic::getItemsCount()
{
    int i = gridLayer->getCount();
    return i;
}

void gridHandler(void *item, void *logic)
{
    GridItem* i = (GridItem*)item;
    GameLogic* l = (GameLogic*)logic;

    l->Analyze(i);
}

Game::Game(StateData *data)
{
    this->data = data;
}

void Game::Activate()
{
    Scene* scene = new Scene;
    int i = 0;
    Program* program = data->game->getEngine()->Video()->CreateProgram();
    Shader* shaders[2];
    game = new GameLogic(data);

    shaders[0] = data->game->getEngine()->Video()->CreateShader("VertexShader.glsl", GL_VERTEX_SHADER);
    shaders[1] = data->game->getEngine()->Video()->CreateShader("FragmentShader.glsl", GL_FRAGMENT_SHADER);
    program->Attach(shaders[0]);
    program->Attach(shaders[1]);
    program->Compile();
    scene->objects = game->getGrid();
    scene->passes = game->getItemsCount();
    for (i = 0; i < scene->passes; i++)
        scene->objects[i]->data()->program = program;
    data->game->getEngine()->Video()->SendScene(scene);
}

void Game::Destroy(bool save)
{
    if(!save)
        delete game;
}

void Game::ChangeState(GameStates state)
{
    data->game->setState(state);
}

void Game::Move(GameState *target)
{
    return;
}
