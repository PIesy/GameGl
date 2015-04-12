#include "uilayer.h"

void UiListener(SDL_Event* event, UiLayer* ui);
void ResolutionNotifier(SDL_Event *event, UiLayer *ui);

UiLayer::UiLayer(EngineInterface* core, SDL_Window* window)
{
    ActionOld listener((void (*)(void*, void*))UiListener, this);
    ActionOld resnotifier((void (*)(void*, void*))ResolutionNotifier, this);
    core->BindAction(listener, -1, 2);
    core->BindAction(resnotifier, SDL_WINDOWEVENT_RESIZED, 0);
    this->window = window;
    initState();
}

void UiLayer::AddElement(Element *element)
{
    element->Bind(&state);
    elements.insert(element);
}

void UiLayer::RemoveElement(Element *element)
{
    elements.erase(element);
    element->Bind(nullptr);
}

void UiLayer::initState()
{
    SDL_GetWindowSize(window, &state.resolution[0], &state.resolution[1]);
}

void UiLayer::UiHandler(SDL_Event *event)
{
    switch (event->type) {
    case SDL_MOUSEMOTION:
        updateAll(event->motion.x, event->motion.y);
        break;
    case SDL_MOUSEBUTTONDOWN:
        break;
    case SDL_MOUSEBUTTONUP:
        updateAll(event->button.x, event->button.y, true);
        break;
    }
    delete event;
}

void UiLayer::UpdateResolution(int width, int height)
{
    state.resolution[0] = width;
    state.resolution[1] = height;
    for (Element* el : elements)
        el->Update();
}

void UiLayer::updateAll(int x, int y, bool click)
{
    for (Element* el : elements)
        el->setState(x, y, click);
}

VertexObject** UiLayer::getGraphics()
{
    int i = 0;
    VertexObject** objects;

    objects = new VertexObject*[elements.size()];
    for (Element* obj : elements)
    {
        objects[i] = obj->getGraphics();
        i++;
    }
    return objects;
}

int UiLayer::getCount()
{
    return elements.size();
}

void UiListener(SDL_Event *event, UiLayer *ui)
{
    ui->UiHandler(event);
}

void ResolutionNotifier(SDL_Event *event, UiLayer *ui)
{
    ui->UpdateResolution(event->window.data1, event->window.data2);
}
