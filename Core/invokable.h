#ifndef INVOKABLE_H
#define INVOKABLE_H

#include <functional>

typedef void (*ActionFun)(void*);

class Invokable
{
public:
    virtual void invoke(void*) {}
    virtual void operator()(void*) {}
};

class Action: public Invokable
{
    using ActionObject = std::function<void(void*)>;
    ActionObject fun;
public:
    Action() {}
    template<typename Callable>
    Action(Callable fun) { bind(fun); }
    template<typename Callable>
    void bind(Callable fun) { this->fun = ActionObject(fun); }
    void invoke(void* arg);
    void operator()(void* arg);
};

#endif // INVOKABLE_H
