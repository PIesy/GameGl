#include "action.h"


template<>
void Action<>::Invoke()
{
    if (isValid)
    {
        if (hasFuture)
            isValid = false;
        fun();
    }
}
