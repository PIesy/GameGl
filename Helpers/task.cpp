#include "task.h"

Task* Task::Copy() const
{
    return new Task(*this);
}

void Task::Invoke()
{
    if(isValid)
    {
        if(hasFuture)
            isValid = false;
        task();
    }
}

void Task::operator()()
{
    Invoke();
}
