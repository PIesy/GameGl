#include "renderqueue.h"

RenderQueue::RenderQueue()
{
    queue = &endItem;
}

RenderQueue::~RenderQueue()
{
    QueueItem* j;

    for (auto* i = queue; i != &endItem;)
    {
        if (!i) break;
        j = i->next;
        delete i;
        i = j;
    }
}

void RenderQueue::Push(Scene *scene)
{
    QueueItem* item = new QueueItem;

    item->scene = scene;
    item->next = queue;
    queue->prev = item;
    queue = item;
    count++;
}

Scene* RenderQueue::Pop()
{
    QueueItem* item = endItem.prev;
    Scene* scene = nullptr;

    if (item)
    {
        if (queue == item)
            queue = queue->next;
        scene = item->scene;
        endItem.prev = item->prev;
        delete item;
        count--;
    }
    return scene;
}

bool RenderQueue::IsEmpty()
{
    return count == 0;
}
