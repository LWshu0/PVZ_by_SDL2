#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <memory>
#include <list>
#include <queue>

template <typename T>
class ObjectFactory {
public:
    virtual std::shared_ptr<T> create() = 0;
};

template <typename T>
class ObjectPool {
private:
    std::shared_ptr<ObjectFactory<T>> m_objectFactory;
    std::queue<std::shared_ptr<T>> m_objects;
public:
    ObjectPool(const std::shared_ptr<ObjectFactory<T>>& factory, int init_size = 0) :
        m_objectFactory(factory)
    {
        while (init_size > 0)
        {
            init_size--;
            m_objects.push(m_objectFactory->create());
        }
    }
    std::shared_ptr<T> getReusable()
    {
        if (m_objects.empty()) m_objects.push(m_objectFactory->create());
        auto ptr = m_objects.front();
        m_objects.pop();
        return std::move(ptr);
    }
    void returnReusable(std::shared_ptr<T>& ptr)
    {
        m_objects.push(std::move(ptr));
    }

    inline size_t size()
    {
        return m_objects.size();
    }

    inline bool empty()
    {
        return m_objects.empty();
    }

    void clear()
    {
        while(!m_objects.empty())
        {
            m_objects.pop();
        }
    }

    ~ObjectPool() {}
};

#endif