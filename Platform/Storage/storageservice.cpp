//
// Created by akann on 4/23/17.
//

#include "storageservice.h"
#include "../../Helpers/task.h"
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
        serviceThread.setTask(clear);
        clear.WaitTillFinished();
        serviceThread.Terminate();
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
    serviceThread.Join();
}

StorageDescriptor StorageService::allocateNew(size_t size, unsigned elementCount, unsigned alignment)
{
    StorageDescriptor desc;
    void* data = aligned_alloc(alignment, size * elementCount);

    desc.alignment = alignment;
    desc.size = size;
    desc.itemCount = elementCount;
    desc.pointer = StoragePointer([data]() { return data; }, []() { return true; });
    desc.id = randomEngine();

    storage.emplace(desc.id, data);
    return desc;
}

StorageDescriptor StorageService::place(const void* ptr, size_t size)
{
    StorageDescriptor desc;

    void* data = malloc(size);
    std::memcpy(data, ptr, size);
    desc.size = size;
    desc.alignment = 1;
    desc.itemCount = 1;
    desc.id = randomEngine();
    desc.pointer = StoragePointer([data]() { return data; }, []() { return true; });

    storage.emplace(desc.id, data);
    return desc;
}

void StorageService::clearAll()
{
    std::for_each(storage.begin(), storage.end(), [](auto item)
    {
        free(item.second);
    });
    storage.clear();
}

StorageDescriptor StorageService::Allocate(size_t size, unsigned elementCount, unsigned alignment)
{
    Task task;
    auto future = task.SetTask(std::bind(&StorageService::allocateNew, this, size, elementCount, alignment));
    serviceThread.setTask(Task(&StorageService::allocateNew, this, size, elementCount, alignment));
    return future.get();
}

StorageDescriptor StorageService::Place(size_t size, const void* ptr)
{
    Task task;
    auto future = task.SetTask(std::bind(&StorageService::place, this, ptr, size));
    serviceThread.setTask(task);
    return future.get();
}

StorageService::~StorageService()
{
    Stop();
}
