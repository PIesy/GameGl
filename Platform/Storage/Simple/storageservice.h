//
// Created by akann on 4/23/17.
//

#ifndef GAMEGL_STORAGESERVICE_H
#define GAMEGL_STORAGESERVICE_H


#include <unordered_map>
#include <random>
#include "../../../Core/service/service.h"
#include "../../../Storage/storagedescriptor.h"
#include "../../../Core/enginecore.h"

class StorageService : public Service
{
    Executor& serviceThread = core::core.Get().GetExecutor(true, getClassName<StorageService>());
    std::unordered_map<unsigned long, StorageDescriptor> storage;
    std::unordered_map<size_t, StorageDescriptor> storageExternalKey;
    std::mt19937_64 randomEngine;
    bool isRunning = false;

    StorageDescriptor allocateNew(size_t size, unsigned elementCount, unsigned alignment);
    StorageDescriptor place(const void* ptr, size_t size);
    StorageDescriptor placeId(const void* ptr, size_t size, size_t id);
    StorageDescriptor get(size_t id);
    void clearAll();
public:
    virtual ~StorageService();

    void Start() override;
    void Stop() override;
    void Pause() override;
    void Resume() override;
    void Restart() override;
    void Wait() override;

    StorageDescriptor Allocate(size_t size, unsigned elementCount, unsigned alignment);
    StorageDescriptor Place(size_t size, const void* ptr);
    StorageDescriptor Place(size_t size, const void* ptr, size_t id);
    StorageDescriptor Get(size_t id);
};


#endif //GAMEGL_STORAGESERVICE_H
