#ifndef ACTIONPACK
#define ACTIONPACK

#include "Helpers/action.h"
#include <forward_list>

template<class... Args>
class ActionPack: public TypedInvokable<Args...>
{
    std::forward_list<std::shared_ptr<TypedInvokable<Args...>>> actions;
    int id = 0;
public:
    ActionPack() {}
    ActionPack(const TypedInvokable<Args...>& source);
    void Invoke() {}
    void InvokeWithArgs(Args&&... args);
    void operator()() {}
    void AddAction(const TypedInvokable<Args...>& action);
    ActionPack<Args...>* Copy() const;
    int getId();
    void setId(int id);
};

template<class... Args>
ActionPack<Args...>::ActionPack(const TypedInvokable<Args...>& source)
{
    actions.emplace_front(source.Copy());
}

template<class... Args>
void ActionPack<Args...>::AddAction(const TypedInvokable<Args...>& action)
{
    actions.emplace_front(action.Copy());
}

template<class... Args>
void ActionPack<Args...>::InvokeWithArgs(Args&&... args)
{
    for(auto& action: actions)
        action->InvokeWithArgs(std::forward<Args>(args)...);
}

template<class... Args>
void ActionPack<Args...>::setId(int id)
{
    this->id = id;
}

template<class... Args>
int ActionPack<Args...>::getId()
{
    return id;
}

template<class... Args>
ActionPack<Args...>* ActionPack<Args...>::Copy() const
{
    return new ActionPack<Args...>(*this);
}

#endif // ACTIONPACK

