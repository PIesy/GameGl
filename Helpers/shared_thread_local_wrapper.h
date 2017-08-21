//
// Created by akann on 4/22/17.
//

#ifndef GAMEGL_EXTERNTHREADLOCALWRAPPER_H
#define GAMEGL_EXTERNTHREADLOCALWRAPPER_H

template<typename T>
class SharedThreadLocalWrapper
{
    T* data = nullptr;
public:
    void Init(T& data)
    {
        this->data = &data;
    }

    bool IsInit() const
    {
        return data != nullptr;
    }

    T& Get() const
    {
        return *data;
    }
};


#endif //GAMEGL_EXTERNTHREADLOCALWRAPPER_H
