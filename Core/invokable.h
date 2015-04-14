#ifndef INVOKABLE_H
#define INVOKABLE_H

#include <functional>
#include "Logger/logger.h"

typedef void (*ActionFun)(void*);

class Invokable
{
public:
    virtual ~Invokable() {}
    virtual Invokable* copy() = 0;
    virtual void Invoke(void*) = 0;
    virtual void operator()(void*) = 0;
    virtual void setId(int id) = 0;
    virtual int getId() = 0;
};

//class Action: public Invokable
//{
//    using ActionObject = std::function<void(void*)>;
//    ActionObject fun;
//    bool isValid = false;
//    int id;
//    void init(int id);
//public:
//    Action(int id = -1);
//    template<typename Callable>
//    Action(Callable fun, int id = -1) { Bind(fun); init(id); }
//    template<typename Callable>
//    void Bind(Callable fun) { this->fun = ActionObject(fun); init(id); }
//    void invoke(void* arg);
//    void operator()(void* arg);
//    void setId(int id);
//    int getId();
//    bool IsCallable();
//};

template<typename ArgumentPtr>
class Action: public Invokable
{
    using TypedActionObject = std::function<void(ArgumentPtr)>;
    TypedActionObject fun;
    bool isValid = false;
    int uid = -1;
    int id;
public:
    Action(int id = -1) { this->uid = id; }
    Action(const Action& src)
    {
        if(src.uid == 5000)
            Logger::Log("Action is copied");
        uid = src.uid;
        id = src.id;
        isValid = src.isValid;
        fun = src.fun;
    }
    ~Action()
    {
        if(uid == 5000)
            Logger::Log("Action is deleted " + std::to_string(id));
    }
    template<typename Callable>
    Action(Callable fun, int id = -1) { Bind(fun); isValid = true; this->uid = id; }
    template<typename Callable>
    void Bind(Callable fun) { this->fun = TypedActionObject(fun); isValid = true; }
    Action<ArgumentPtr>* copy()
    {
        Action<ArgumentPtr>* result = new Action<ArgumentPtr>(*this);
        return result;
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
    void setId(int id) { this->id = id; }
    int getId() { return id; }
};

#endif // INVOKABLE_H
