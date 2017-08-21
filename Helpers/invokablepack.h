#ifndef ACTIONPACK
#define ACTIONPACK

#include "../Core/invokable.h"
#include <forward_list>
#include <memory>

template<class... Args>
class InvokablePack: public TypedInvokable<Args...>
{
    std::forward_list<std::shared_ptr<TypedInvokable<Args...>>> actions;
    int id = 0;
public:
    InvokablePack() {}
    InvokablePack(const TypedInvokable<Args...>& source);
    void Invoke() {}
    void InvokeWithArgs(Args&&... args);
    void operator()();
    void AddAction(const TypedInvokable<Args...>& action);
    InvokablePack<Args...>* Copy() const;
    int getId();
    void setId(int id);
};

template<class... Args>
InvokablePack<Args...>::InvokablePack(const TypedInvokable<Args...>& source)
{
    actions.emplace_front(source.Copy());
}

template<class... Args>
void InvokablePack<Args...>::AddAction(const TypedInvokable<Args...>& action)
{
    actions.emplace_front(action.Copy());
}

template<class... Args>
void InvokablePack<Args...>::InvokeWithArgs(Args&&... args)
{
    for(auto& action: actions)
        action->InvokeWithArgs(std::forward<Args>(args)...);
}

template<class... Args>
void InvokablePack<Args...>::setId(int id)
{
    this->id = id;
}

template<class... Args>
int InvokablePack<Args...>::getId()
{
    return id;
}

template<class... Args>
InvokablePack<Args...>* InvokablePack<Args...>::Copy() const
{
    return new InvokablePack<Args...>(*this);
}

template<class... Args>
void InvokablePack<Args...>::operator()()
{
    Invoke();
}

template<>
void InvokablePack<>::Invoke();

#endif // ACTIONPACK

