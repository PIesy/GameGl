#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include "renderdefs.h"

struct QueueItem
{
    Scene* scene = nullptr;
    QueueItem* prev = nullptr;
    QueueItem* next = nullptr;
};

class RenderQueue
{
    QueueItem* queue;
    int count = 0;
    QueueItem endItem;
public:
    RenderQueue();
    ~RenderQueue();
    void Push(Scene* scene);
    Scene* Pop();
    bool IsEmpty();
};

struct RenderInitializer
{
    RenderQueue* queue;
    GraphicsData* data;
};

#endif // RENDERQUEUE_H
