#ifndef _BAKA_POOL_ALLOCATOR_H_
#define _BAKA_POOL_ALLOCATOR_H_

#include <baka_alloc_commons.h>
#include <stdexcept>

namespace baka
{

template <typename T>
class PoolAllocator
{
public:
    PoolAllocator(uint32_t count);
    ~PoolAllocator();

    T *Alloc();
    void Free(T *ptr);

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

template <typename T>
void PoolAllocator<T>::Free(T *ptr)
{
    if(ptr < this->m_pool || ptr > this->m_pool + this->m_count) 
    {
        char msg[128];
        std::snprintf(
            msg, 
            128, 
            "trying to free pointer %p that is not part of the specified pool allocator", 
            ptr);
        throw std::runtime_error(msg);
    }

    if(this->m_current > ptr)
    {
        *(T**)(ptr) = this->m_current;
        this->m_current = ptr;
    }
}

} // namespace baka

/**
 * @brief override operator new so it returns a pointer from the allocator's memory
 * @param allocator the pool allocator to get the memory from
 * @note this is meant to be placed inside a class
 */
#define BAKA_POOL_ALLOC_NEW_OVERRIDE(allocator)\
static void *operator new(size_t n) {\
    return allocator.Alloc();\
}

#endif
    