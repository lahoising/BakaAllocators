#ifndef _BAKA_POOL_ALLOCATOR_H_
#define _BAKA_POOL_ALLOCATOR_H_

#include <baka_alloc_commons.h>

namespace baka
{

template <typename T>
class PoolAllocator
{
public:
    PoolAllocator(uint32_t count);
    ~PoolAllocator();

    T *Alloc();

private:
    T *m_pool;
    T *m_current;
    uint32_t m_count;
    static const bool m_isLinkedList = sizeof(T) >= sizeof(void*);
};

template <typename T>
PoolAllocator<T>::PoolAllocator(uint32_t count)
{
    this->m_pool = (T*)malloc(sizeof(T) * count);
    this->m_current = m_pool;
    this->m_count = count;

    for(uint32_t i = 0; i < count - 1; i++)
    {
        T **dst = (T**)(&m_pool[i]);
        if(PoolAllocator<T>::m_isLinkedList) 
            *dst = &m_pool[i+1];
    }

    T **dst = (T**)(&m_pool[count-1]);
    if(PoolAllocator<T>::m_isLinkedList) *dst = nullptr;
}

template <typename T>
T *PoolAllocator<T>::Alloc()
{
    if(!this->m_current) return nullptr;
    T *allocated = this->m_current;
    this->m_current = *(T**)(this->m_current);
    return allocated;
}

template <typename T>
PoolAllocator<T>::~PoolAllocator()
{
    if(this->m_pool) delete this->m_pool;
}

} // namespace baka

#endif
    