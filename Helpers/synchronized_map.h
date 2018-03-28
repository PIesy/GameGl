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

    void put(const K& key, V&& value)
    {
        objects.emplace(key, std::move(value));
        objectLocks.emplace(key, std::make_unique<std::shared_timed_mutex>());
    }

    void put(const K& key, const V& value)
    {
        objects.emplace(key, value);
        objectLocks.emplace(key, std::make_unique<std::shared_timed_mutex>());
    }

    LockedResource<V> get(const K& key, bool read)
    {
        if (read)
            return ReadLockHolder<V>{objects[key], *objectLocks.at(key)};
        return WriteLockHolder<V>{objects[key], *objectLocks.at(key)};
    }
public:
    void Put(const K& key, V&& value)
    {
        std::unique_lock<std::shared_timed_mutex> guard{accessMutex};

        put(key, std::move(value));
    }

    void Put(const K& key, const V& value)
    {
        std::unique_lock<std::shared_timed_mutex> guard{accessMutex};

        put(key, value);
    }

    LockedResource<V> PutAndLock(const K& key, V&& value, bool read)
    {
        std::unique_lock<std::shared_timed_mutex> guard{accessMutex};

        put(key, std::move(value));
        return get(key, read);
    }

    LockedResource<V> PutAndLock(const K& key, const V& value, bool read)
    {
        std::unique_lock<std::shared_timed_mutex> guard{accessMutex};

        put(key, value);
        return get(key, read);
    }
    
    LockedResource<V> GetRead(const K& key)
    {
        std::shared_lock<std::shared_timed_mutex> guard{accessMutex};

        return ReadLockHolder<V>{objects.at(key), *objectLocks.at(key)};
    }

    LockedResource<V> GetWrite(const K& key)
    {
        std::shared_lock<std::shared_timed_mutex> guard{accessMutex};

        return WriteLockHolder<V>{objects[key], *objectLocks.at(key)};
    }

    LockedResource<V> Get(const K& key, bool read)
    {
        std::shared_lock<std::shared_timed_mutex> guard{accessMutex};

        return get(key, read);
    }

    const V& Get(const K& key) const
    {
        std::shared_lock<std::shared_timed_mutex> guard{accessMutex};

        return objects.at(key);
    }

    bool Contains(const K& key) const
    {
        std::shared_lock<std::shared_timed_mutex> guard{accessMutex};

        return objects.find(key) != objects.end();
    }
};


#endif //GAMEGL_SYNCHRONIZED_MAP_H
