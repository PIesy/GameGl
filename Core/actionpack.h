#ifndef ACTIONPACK
#define ACTIONPACK

#include "invokable.h"
#include <forward_list>

class ActionPack: public Invokable
{
    std::forward_list<std::shared_ptr<Invokable>> actions;
    int id = 0;
public:
    ActionPack() {}
    ActionPack(const Invokable& source);
    void Invoke(void* arg);
    void operator()(void* arg);
    void AddAction(const Invokable& action);
    Invokable* Copy() const;
    int getId();
    void setId(int id);
};

#endif // ACTIONPACK

