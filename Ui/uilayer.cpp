#include "uilayer.h"

void UiListener(MouseEvent* event, UiLayer* ui);
void ResolutionNotifier(WindowEvent* event, UiLayer* ui);

UiLayer::UiLayer(EngineInterface* core, Window& window):window(window)
{
    Action<MouseEvent*> listener(UiListener, std::placeholders::_1, this);
    Action<WindowEvent*> resnotifier(ResolutionNotifier, std::placeholders::_1, this);
    core->getEventHandler().setListener<MouseEvent>(listener);
    core->getEventHandler().setListener<WindowEvent>(resnotifier);
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
    WindowSize size = window.getSize();
    state.resolution[0] = size.width;
    state.resolution[1] = size.height;
}

void UiLayer::UiHandler(MouseEvent* event)
{
    MouseData data = event->getPayload();

    switch(data.eventType) {
    case MouseData::Type::Motion:
        updateAll(data.coordinates[0], data.coordinates[1]);
        break;
    default:
        break;
    case MouseData::Type::Button:
        if(!data.state)
            updateAll(data.coordinates[0], data.coordinates[1], true);
        break;
    }
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
    for(Element* el : elements)
        el->setState(x, y, click);
}

GraphicsObject** UiLayer::getGraphics()
{
    int i = 0;
    GraphicsObject** objects;

    objects = new GraphicsObject*[elements.size()];
    for(Element* obj : elements)
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

void UiListener(MouseEvent* event, UiLayer *ui)
{
    ui->UiHandler(event);
}

void ResolutionNotifier(WindowEvent* event, UiLayer *ui)
{
    WindowData data = event->getPayload();

    if(data.eventType == WindowData::Type::Resize)
        ui->UpdateResolution(data.coordinates[0], data.coordinates[1]);
}
