#ifndef REPEATEDPROMISE_H
#define REPEATEDPROMISE_H

#include "repeteadfuture.h"
#include <memory>
#include "dataobject.h"

template<class T>
class RepeatedPromise
{
    std::shared_ptr<DataObject> data;
public:
    RepeatedPromise();
    void Fulfill(T value);
    RepeatedFuture<T> getFuture();
};

#endif // REPEATEDPROMISE_H

