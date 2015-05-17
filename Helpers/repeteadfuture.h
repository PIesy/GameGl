#ifndef REPETEADFUTURE_H
#define REPETEADFUTURE_H

#include <memory>

template<class T>
class RepeatedFuture
{
    //std::shared_ptr<>
    RepeatedFuture();
public:
    ~RepeatedFuture();
    T get();
    friend class RepeatedPromise;
};

#endif // REPETEADFUTURE_H
