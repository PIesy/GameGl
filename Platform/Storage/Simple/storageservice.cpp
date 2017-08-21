//
// Created by akann on 4/23/17.
//

#include "storageservice.h"
#include "../../../Helpers/task.h"
#include <algorithm>
#include <cstring>

void StorageService::Start()
{
    isRunning = true;
}

void StorageService::Stop()
{
    if (isRunning)
    {
        isRunning = false;
        Task clear{std::bind(&StorageService::clearAll, this)};
        serviceThread.Execute(clear);
    }
}

void StorageService::Pause()
{

}

void StorageService::Resume()
{

}

void StorageService::Restart()
{

}

void StorageService::Wait()
{
}

StorageDescriptor StorageService::allocateNew(size_t size, unsigned elementCount, unsigned alignment)
{
    StorageDescriptor desc;
    size_t elementSize = std::max((size_t)alignment, size);
    void* data = aligned_alloc(alignment, elementSize * elementCount);

    desc.alignment = alignment;
    desc.size = elementSize;
    desc.itemCount = elementCount;
    desc.pointer = StoragePointer([data]() { return data; }, []() { return true; });
    desc.id = randomEngine();

    storage.emplace(desc.id, desc);
    return desc;
}

StorageDescriptor StorageService::place(const void* ptr, size_t size)
{
    StorageDescriptor desc;

    void* data = std::malloc(size);
    std::memcpy(data, ptr, size);
    desc.size = size;
    desc.alignment = 1;
    desc.itemCount = 1;
    desc.id = randomEngine();
    desc.pointer = StoragePointer([data]() { return data; }, []() { return true; });

    storage.emplace(desc.id, desc);
    return desc;
}

StorageDescriptor StorageService::placeId(const void* ptr, size_t size, size_t id)
{
    StorageDescriptor desc;

    void* data = std::malloc(size);
    std::memcpy(data, ptr, size);
    desc.size = size;
    desc.alignment = 1;
    desc.itemCount = 1;
    desc.id = id;
    desc.pointer = StoragePointer([data]() { return data; }, []() { return true; });

    storageExternalKey.emplace(desc.id, desc);
    return desc;
}

void StorageService::clearAll()
{
    std::for_each(std::begin(storageExternalKey), std::end(storageExternalKey), [](auto& item)
    {
        free(item.second.pointer);
    });
    storageExternalKey.clear();

    std::for_each(std::begin(storage), std::end(storage), [](auto& item)
    {
        free(item.second.pointer);
    });
    storage.clear();
}

StorageDescriptor StorageService::Allocate(size_t size, unsigned elementCount, unsigned alignment)
{
    if (!serviceThread.IsValid())
        return StorageDescriptor{};
    Task task;
    auto future = task.SetTask(std::bind(&StorageService::allocateNew, this, size, elementCount, alignment));
    serviceThread.Execute(task);
    return future.get();
}

StorageDescriptor StorageService::Place(size_t size, const void* ptr)
{
    if (!serviceThread.IsValid())
        return StorageDescriptor{};
    Task task;
    auto future = task.SetTask(std::bind(&StorageService::place, this, ptr, size));
    serviceThread.Execute(task);
    return future.get();
}

StorageService::~StorageService()
{
    Stop();
}

StorageDescriptor StorageService::Place(size_t size, const void* ptr, size_t id)
{
    if (!serviceThread.IsValid())
        return StorageDescriptor{};
    Task task;
    auto future = task.SetTask(std::bind(&StorageService::placeId, this, ptr, size, id));
    serviceThread.Execute(task);
    return future.get();
}

StorageDescriptor StorageService::Get(size_t id)
{
    if (!serviceThread.IsValid())
        return StorageDescriptor{};
    Task task;
    auto future = task.SetTask(std::bind(&StorageService::get, this, id));
    serviceThread.Execute(task);
    return future.get();
}

StorageDescriptor StorageService::get(size_t id)
{
    if (storageExternalKey.count(id))
        return storageExternalKey[id];
    if (storage.count(id))
        return storage[id];
    return StorageDescriptor();
}
