#include "broadcaster.h"

Broadcaster GlobalBroadcaster::globalBroadcaster;

void Broadcaster::Notify(int msg, int id)
{
    BroadcasterToken& t = subscribers.at(id);
    t.Update(msg);
}

void Broadcaster::NotifyAll(int msg)
{
    for(std::pair<const int, BroadcasterToken> token : subscribers)
        token.second.Update(msg);
}

int Broadcaster::Subscribe(BroadcasterToken subscriber)
{
    std::pair<int, BroadcasterToken> pair(current_id, subscriber);
    subscribers.insert(pair);
    current_id++;
    return current_id - 1;
}

void Broadcaster::Unsubscribe(int id)
{
    subscribers.erase(id);
}
