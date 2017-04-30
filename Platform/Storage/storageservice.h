//
// Created by akann on 4/23/17.
//

#ifndef GAMEGL_STORAGESERVICE_H
#define GAMEGL_STORAGESERVICE_H


#include <unordered_map>
#include <random>
#include "../../Core/service/service.h"
#include "../../Storage/storagedescriptor.h"

class StorageService : public Service
{
    Worker serviceThread{"storage service"};
    std::unordered_map<unsigned long, void*> storage;
    std::mt19937_64 randomEngine;
    bool isRunning = false;

    StorageDescriptor allocateNew(size_t size, unsigned elementCount, unsigned alignment);
    StorageDescriptor place(const void* ptr, size_t size);
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
};


#endif //GAMEGL_STORAGESERVICE_H
