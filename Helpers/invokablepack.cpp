#include "invokablepack.h"

template<>
void InvokablePack<>::Invoke()
{
    for(auto& action: actions)
        action->Invoke();
}
