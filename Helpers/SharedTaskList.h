//
// Created by akann on 8/16/17.
//

#ifndef GAMEGL_TASKLIST_H
#define GAMEGL_TASKLIST_H

#include <list>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "../Core/invokable.h"

class SharedTaskList
{
    std::shared_ptr<std::list<std::shared_ptr<Invokable>>> tasks{new std::list<std::shared_ptr<Invokable>>};
    std::shared_ptr<std::mutex> mutex{new std::mutex()};
    std::shared_ptr<std::condition_variable> newTask{new std::condition_variable};
public:
    void Add(const Invokable& task);
    std::shared_ptr<Invokable> Get();
    std::shared_ptr<Invokable> GetAndRemove();
    void Pop();
    bool IsEmpty();
};


#endif //GAMEGL_TASKLIST_H
