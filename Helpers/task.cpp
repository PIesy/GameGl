#include "task.h"
#include "helpers.h"

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
    isInvoked.setState(true);
}

void Task::operator()()
{
    Invoke();
}


void Task::WaitTillFinished()
{
    isInvoked.WaitForStateChange();
}
