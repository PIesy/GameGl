//
// Created by akann on 9/14/17.
//

#ifndef GAMEGL_SYNCHRONIZED_MAP_H
#define GAMEGL_SYNCHRONIZED_MAP_H

#include "lock_holder.h"
#include <unordered_map>
#include <shared_mutex>

template<typename K, typename V>
class SynchronizedMap
{
    mutable std::shared_timed_mutex accessMutex;
    std::unordered_map<K, std::unique_ptr<std::shared_timed_mutex>> objectLocks{};
    std::unordered_map<K, V> objects{};
public:
    void Put(K&& key, V&& value)
    {
        accessMutex.lock();

        objects.emplace(key, std::move(value));
        objectLocks.emplace(key, std::make_unique<std::shared_timed_mutex>());
        accessMutex.unlock();
    }

    void Put(const K& key, V&& value)
    {
        accessMutex.lock();

        objects.emplace(key, std::move(value));
        objectLocks.emplace(key, std::make_unique<std::shared_timed_mutex>());
        accessMutex.unlock();
    }

    void Put(const K& key, const V& value)
    {
        accessMutex.lock();

        objects.emplace(key, std::move(value));
        objectLocks.emplace(key, std::make_unique<std::shared_timed_mutex>());
        accessMutex.unlock();
    }
    
    LockedResource<V> GetRead(const K& key)
    {
        accessMutex.lock_shared();

        auto result = ReadLockHolder<V>{objects.at(key), *objectLocks.at(key)};
        accessMutex.unlock_shared();
        return std::move(result);
    }

    LockedResource<V> GetWrite(const K& key)
    {
        accessMutex.lock_shared();

        auto result = WriteLockHolder<V>{objects[key], *objectLocks.at(key)};
        accessMutex.unlock_shared();
        return std::move(result);
    }

    LockedResource<V> Get(const K& key, bool read)
    {
        accessMutex.lock_shared();

        if (read)
        {
            auto result = ReadLockHolder<V>{objects[key], *objectLocks.at(key)};
            accessMutex.unlock_shared();
            return std::move(result);
        }
        auto result = WriteLockHolder<V>{objects[key], *objectLocks.at(key)};
        accessMutex.unlock_shared();
        return std::move(result);
    }

    const V& Get(const K& key) const
    {
        accessMutex.lock_shared();

        auto& result = objects.at(key);
        accessMutex.unlock_shared();
        return result;
    }

    bool Contains(const K& key) const
    {
        accessMutex.lock_shared();

        bool result = objects.count(key) != 0;
        accessMutex.unlock_shared();
        return result;
    }
};


#endif //GAMEGL_SYNCHRONIZED_MAP_H
