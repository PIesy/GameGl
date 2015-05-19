#ifndef GENERICCONDITION_H
#define GENERICCONDITION_H

#include <mutex>
#include <condition_variable>

template<class T>
class GenericCondition
{
    T value;
    std::mutex mutex;
    std::condition_variable condition;
public:
    GenericCondition() {}
    GenericCondition(const T& value)
    {
        this->value = value;
    }

    GenericCondition(GenericCondition<T>&& rhs)
    {
        value = rhs.value;
    }

    void setValue(const T& value)
    {
        std::lock_guard<std::mutex> lock(mutex);
        this->value = value;
        condition.notify_all();
    }

    void WaitFor(const T& value)
    {
        if(this->value != value)
        {
            std::unique_lock<std::mutex> lock(mutex);
            while(this->value != value)
                condition.wait(lock);
        }
    }

    T& getValue() const
    {
        return value;
    }

    operator const T&() const
    {
        return value;
    }
};

#endif // GENERICCONDITION_H

