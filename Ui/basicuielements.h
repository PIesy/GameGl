#ifndef BASICUIELEMENTS_H
#define BASICUIELEMENTS_H

#include "elementinterface.h"
#include <unordered_map>

class Item: public Element
{
protected:
    bool hover = false;
    Events lastEvent = Events::None;
    int size[2] = {0,0};
    WindowState* window = nullptr;
    float factor[2] = {0,0};
    int position[2] = {0,0};
    Mesh base;
    Mesh graphics;
    std::unordered_multimap<std::underlying_type_t<Events>, GenericInvokable> actions;

    void setFactor();
    void setData(StorageApi& api);
    void rescale();
    void reposition();
    void invokeActions();
    void setColor(Mesh& target, RGBA_Color color);
public:
    Item(){}
    Item(int width, int height, StorageApi& api);

    void Update() override;
    Mesh& GetGraphics() override;
    void SetPosition(int x, int y) override;
    void SetState(int x, int y, bool click) override;
    void SetAction(Events event, const GenericInvokable& action) override;
    void Bind(WindowState* window) override;
};

#endif // BASICUIELEMENTS_H
