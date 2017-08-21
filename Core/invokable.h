#ifndef INVOKABLE_H
#define INVOKABLE_H

class Invokable
{
public:
    virtual ~Invokable() {}
    virtual Invokable* Copy() const = 0;
    virtual void Invoke() = 0;
    virtual void operator()() = 0;
};

template<class... Args>
class TypedInvokable: public Invokable
{
public:
    virtual ~TypedInvokable() {}
    virtual TypedInvokable<Args...>* Copy() const = 0;
    virtual void Invoke() = 0;
    virtual void InvokeWithArgs(Args&&... args) = 0;
    virtual void operator()() = 0;
};

#endif // INVOKABLE_H
