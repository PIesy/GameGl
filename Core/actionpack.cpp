#include "actionpack.h"

ActionPack::ActionPack(const Invokable& source)
{
    actions.emplace_front(source.Copy());
}

void ActionPack::AddAction(const Invokable &action)
{
    actions.emplace_front(action.Copy());
}

void ActionPack::Invoke(void *arg)
{
    for(auto& action: actions)
        action->Invoke(arg);
}

void ActionPack::operator()(void* arg)
{
    Invoke(arg);
}

void ActionPack::setId(int id)
{
    this->id = id;
}

int ActionPack::getId()
{
    return id;
}

Invokable* ActionPack::Copy() const
{
    return new ActionPack(*this);
}
