//
// Created by akann on 9/14/17.
//

#ifndef GAMEGL_LOCK_HOLDER_H
#define GAMEGL_LOCK_HOLDER_H

#include <shared_mutex>

template<typename T>
class LockedResource
{
    T& object;
protected:
    std::shared_lock<std::shared_timed_mutex> readLock;
    std::unique_lock<std::shared_timed_mutex> writeLock;
public:
    explicit LockedResource(T& object) : object(object) {}
    LockedResource(const LockedResource<T>&) = delete;
    LockedResource(LockedResource<T>&& src) noexcept = default;
    ~LockedResource() = default;

    const T& GetObject() const
    {
        return object;
    }

    T& GetObject()
    {
        return object;
    }
};

template<typename T>
class ReadLockHolder : public LockedResource<T>
{
public:
    ReadLockHolder(T& object, std::shared_timed_mutex& mutex) : LockedResource<T>(object)
    {
        this->readLock = std::shared_lock<std::shared_timed_mutex>{mutex};
    }
};

template<typename T>
class WriteLockHolder : public LockedResource<T>
{
public:
    WriteLockHolder(T& object, std::shared_timed_mutex& mutex) : LockedResource<T>(object)
    {
        this->writeLock = std::unique_lock<std::shared_timed_mutex>{mutex};
    }
};

template<typename T>
class LocalResource : public LockedResource<T>
{
public:
    explicit LocalResource(T& object) : LockedResource<T>(object) {}
};

#endif //GAMEGL_LOCK_HOLDER_H
