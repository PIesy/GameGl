#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H

#include <memory>
#include "Logger/logger.h"
#include "Helpers/helpers.h"

template<class T>
class SharedState
{
    std::shared_ptr<T> state;
    std::shared_ptr<bool> isStateChanged = std::make_shared<bool>(false);
    std::shared_ptr<std::mutex> ownership = std::make_shared<std::mutex>();
    std::shared_ptr<std::mutex> mutex = std::make_shared<std::mutex>();
    std::shared_ptr<std::condition_variable> stateChanged = std::make_shared<std::condition_variable>();
public:
    SharedState()
    {
        state = std::make_shared<T>();
    }

    SharedState(const T& state)
    {
        this->state = std::make_shared<T>(state);
    }

    T& getState()
    {
        return *state;
    }

    void setState(const T& state)
    {
        std::lock_guard<std::mutex> lock(*mutex);
        *(this->state) = state;
        *isStateChanged = true;
        stateChanged->notify_all();
    }

    void WaitForStateChange()
    {
        if(!*isStateChanged)
        {
            std::unique_lock<std::mutex> lock(*mutex);
            while(!*isStateChanged)
                stateChanged->wait(lock);
            *isStateChanged = false;
        }
    }

    void WaitForStateValue(const T& value)
    {
        if(*state != value)
        {
            std::unique_lock<std::mutex> lock(*mutex);
            while(*state != value)
                stateChanged->wait(lock);
        }
    }

    void GetOwnership()
    {
        ownership->lock();
    }

    void ReleaseOwnership()
    {
        ownership->unlock();
    }
};

#endif // SHAREDSTATE_H

