//
// Created by akann on 8/24/17.
//

#ifndef GAMEGL_GRAPHICS_EVENTS_H
#define GAMEGL_GRAPHICS_EVENTS_H

#include "../Core/events/eventshandler.h"

struct FrameInfo
{
    long frameTime = 0;
};

class FrameInfoEvent: public Event<FrameInfo>
{
public:
    FrameInfoEvent(const FrameInfo& payload, int hint) : Event(payload, hint) {}
};

#endif //GAMEGL_GRAPHICS_EVENTS_H
