#ifndef UILAYER_H
#define UILAYER_H

#include "Core/engineinterface.h"
#include "elementinterface.h"
#include <unordered_set>
#include "Input/inputevents.h"
#include "SDL2/SDL.h"

class UiLayer
{
    SDL_Window* window;
    WindowState state;
    std::unordered_set<Element*> elements;
    void updateAll(int x, int y, bool click = false);
    void initState();
public:
    UiLayer(EngineInterface* core, SDL_Window* window);
    void AddElement(Element* element);
    void RemoveElement(Element* element);
    void UiHandler(MouseEvent* event);
    void UpdateResolution(int width, int height);
    VertexObject** getGraphics();
    int getCount();
};

#endif // UILAYER_H
