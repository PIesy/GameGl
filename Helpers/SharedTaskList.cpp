//
// Created by akann on 8/16/17.
//

#include "SharedTaskList.h"

void SharedTaskList::Add(const Invokable& task)
{
    std::lock_guard<std::mutex> guard{*mutex};
    tasks->emplace_back(task.Copy());
    newTask->notify_one();
}

std::shared_ptr<Invokable> SharedTaskList::Get()
{
    std::unique_lock<std::mutex> guard{*mutex};
    while (IsEmpty())
        newTask->wait(guard);
    return tasks->front();
}

void SharedTaskList::Pop()
{
    std::lock_guard<std::mutex> guard{*mutex};
    tasks->pop_front();
}

bool SharedTaskList::IsEmpty()
{
    std::lock_guard<std::mutex> guard{*mutex};
    return tasks->empty();
}

std::shared_ptr<Invokable> SharedTaskList::GetAndRemove()
{
    std::unique_lock<std::mutex> guard{*mutex};
    while (tasks->empty())
        newTask->wait(guard);
    std::shared_ptr<Invokable> result = tasks->front();
    tasks->pop_front();
    return result;
}
