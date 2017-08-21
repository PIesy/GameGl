#include "task.h"

Task::Task(std::function<void()>&& f) : task(f), isValid(true) {}

Task::Task(const std::function<void()>& f) : task(f), isValid(true) {}

Task& Task::operator=(std::function<void()>&& f)
{
    task = f;
    isValid = true;
    return *this;
}

Task& Task::operator=(const std::function<void()>& f)
{
    task = f;
    isValid = true;
    return *this;
}

Task* Task::Copy() const
{
    return new Task(*this);
}

void Task::Invoke()
{
    if (isValid)
    {
        if (hasFuture)
            isValid = false;
        task();
    }
    isInvoked.setState(true);
}

void Task::operator()()
{
    Invoke();
}


void Task::WaitTillFinished()
{
    isInvoked.WaitForStateValue(true);
}

