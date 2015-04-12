#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <unordered_map>

class BroadcasterToken
{
public:
    virtual void Update(int){}
};

class Broadcaster
{
    std::unordered_map<int, BroadcasterToken> subscribers;
    int current_id = 0;
public:
    void Notify(int msg, int id);
    void NotifyAll(int msg);
    int Subscribe(BroadcasterToken subscriber);
    void Unsubscribe(int id);
};

class GlobalBroadcaster
{
    static Broadcaster globalBroadcaster;
public:
    static void Notify(int msg, int id)
    {
        globalBroadcaster.Notify(msg, id);
    }
    static void NotifyAll(int msg)
    {
        globalBroadcaster.NotifyAll(msg);
    }
    static int Subscribe(BroadcasterToken subscriber)
    {
        return globalBroadcaster.Subscribe(subscriber);
    }
    static void Unsubscribe(int id)
    {
        globalBroadcaster.Unsubscribe(id);
    }
};


#endif // BROADCASTER_H
