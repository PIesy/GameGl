#ifndef INVOKABLE_H
#define INVOKABLE_H

#include <functional>
#include "Logger/logger.h"

class Invokable
{
public:
    virtual ~Invokable() {}
    virtual Invokable* Copy() const = 0;
    virtual void Invoke(void*) = 0;
    virtual void operator()(void*) = 0;
    virtual void setId(int id) = 0;
    virtual int getId() = 0;
};

template<typename ArgumentPtr>
class Action: public Invokable
{
    using TypedActionObject = std::function<void(ArgumentPtr)>;
    TypedActionObject fun;
    bool isValid = false;
    int id;
public:
    Action(int id = -1)
    {
        this->id = id;
    }

    template<typename Callable>
    Action(Callable fun, int id = -1)
    {
        Bind(fun);
        isValid = true;
        this->id = id;
    }

    template<typename Callable>
    void Bind(Callable fun)
    {
        this->fun = TypedActionObject(fun);
        isValid = true;
    }

    Action<ArgumentPtr>* Copy() const
    {
        return new Action<ArgumentPtr>(*this);
    }

    void Invoke(void* arg)
    {
        if(isValid)
            fun((ArgumentPtr)arg);
    }

    void operator()(void* arg)
    {
        Invoke(arg);
    }

    void setId(int id)
    {
        this->id = id;
    }

    int getId()
    {
        return id;
    }
};

#endif // INVOKABLE_H
