#ifndef GENERICCONDITION_H
#define GENERICCONDITION_H

#include <mutex>
#include <condition_variable>

template<class T>
class GenericCondition
{
    T value;
    mutable std::mutex mutex;
    mutable std::condition_variable condition;
public:
    GenericCondition() = default;

    explicit GenericCondition(const T& value)
    {
        this->value = value;
    }

    GenericCondition(GenericCondition<T>&& rhs) noexcept
    {
        value = rhs.value;
    }

    void SetValue(const T& value)
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
            while (this->value != value)
                condition.wait(lock);
        }
    }

    const T& GetValue() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return value;
    }

    operator const T&() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return value;
    }

    GenericCondition<T>& operator=(const T& value)
    {
        std::lock_guard<std::mutex> lock(mutex);
        this->value = value;
    }

    GenericCondition<T>& operator=(T&& value)
    {
        std::lock_guard<std::mutex> lock(mutex);
        this->value = value;
    }
};

#endif // GENERICCONDITION_H

