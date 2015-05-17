#ifndef DATAOBJECT
#define DATAOBJECT

#include <queue>
#include <mutex>
#include <condition_variable>

template<class T>
class DataObject
{
    std::mutex mutex;
    std::condition_variable block;
    std::queue<T> data;
public:
    T getData();
    bool TryGetData();
    void setData(const T& data);
    int GetAmount();
};

template<class T>
T DataObject::getData()
{
    std::unique_lock<std::mutex> lock;
    while(data.empty())
        block.wait(lock);
    T result = data.front();
    data.pop();
    return result;
}

template<class T>
bool DataObject::TryGetData()
{
    return data.empty();
}

template<class T>
void DataObject::setData(const T& data)
{
    std::unique_lock<std::mutex> lock;
    this->data.emplace(data);
}

template<class T>
int DataObject::GetAmount()
{
    return data.size();
}

#endif // DATAOBJECT

