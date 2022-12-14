#pragma once
#include <map>
#include <list>
#include <vector>
#include "../GameObject/Object.h"

using namespace std;

template <typename T>
class ObjectPool
{
private:
    std::list<T*> unuse;
    std::list<T*> use;

public:
    ObjectPool();
    ~ObjectPool();

    void Init(int cacheSize = 100);
    void Release();

    void Update(float dt);
    void Reset();

    T* Get();
    void Return(T* obj);

    const list<T*>& GetUseList();
    const list<T*>& GetUnuseList();

    void (*OnCreate)(T*);
};

template <typename T>
void ObjectPool<T>::Reset()
{
    for (auto ptr : use)
    {
        ptr->SetActive(false);
        unuse.push_back(ptr);
    }
    use.clear();
}

template <typename T>
const list<T*>& ObjectPool<T>::GetUseList()
{
    return use;
}

template <typename T>
const list<T*>& ObjectPool<T>::GetUnuseList()
{
    return unuse;
}

template <typename T>
ObjectPool<T>::ObjectPool() : OnCreate(nullptr)
{
    
}

template <typename T>
ObjectPool<T>::~ObjectPool()
{
    
}

template <typename T>
void ObjectPool<T>::Init(int cacheSize)
{
    for (int i = 0; i < cacheSize; ++i)
    {
        T* ptr = new T();
        ptr->Init();
        ptr->SetActive(false);
        unuse.push_back(ptr);
        if (OnCreate != nullptr)
        {
            OnCreate(ptr);
        }
    }

    for (auto ptr : unuse)
    {
        ptr->Init();
        ptr->SetActive(false);
    }
}

template <typename T>
void ObjectPool<T>::Release()
{
    for (auto obj : unuse)
    {
        delete obj;
    }

    for (auto obj : use)
    {
        delete obj;
    }

    unuse.clear();
    use.clear();
}

template <typename T>
void ObjectPool<T>::Update(float dt)
{
    auto it = use.begin();
    while (it != use.end())
    {
        if (!(*it)->GetActive())
        {
            (*it)->SetActive(false);
            unuse.push_back(*it);
            it = use.erase(it);
        }
        else
        {
            (*it)->Update(dt);
            ++it;
        }
    }
}

template <typename T>
T* ObjectPool<T>::Get()
{
    if (unuse.empty())
    {
        for (int i = 0; i < use.size(); ++i)
        {
            T* ptr = new T();
            ptr->Init();
            ptr->SetActive(false);
            unuse.push_back(ptr);
            if (OnCreate != nullptr)
            {
                OnCreate(ptr);
            }
        }
    }

    auto obj = unuse.front();
    unuse.pop_front();
    use.push_back(obj);

    obj->Reset();
    return obj;
}

template<typename T>
inline void ObjectPool<T>::Return(T* obj)
{
    obj->SetActive(false);

    use.remove(obj);
    unuse.push_back(obj);
}