#ifndef _BAKA_DOUBLE_STACK_ALLOCATOR_H_
#define _BAKA_DOUBLE_STACK_ALLOCATOR_H_

#include <cinttypes>
#include <baka_alloc_commons.h>

namespace baka
{

typedef enum
{
    UPPER = 0,
    LOWER
} Portion;

class DoubleStackAllocator
{
public:
    DoubleStackAllocator(size_t size);
    ~DoubleStackAllocator();

    void *allocLower(size_t size);
    void *allocUpper(size_t size);

    inline void clearLower() {this->m_lowerTop = 0;}
    inline void clearUpper() {this->m_upperTop = this->m_sizeOfStack;}
    
    inline alloc::Marker getMarkerLower() {return this->m_lowerTop;}
    inline alloc::Marker getMarkerUpper() {return this->m_upperTop;}

    inline void freeToMarkerLower(alloc::Marker marker) {this->m_lowerTop = marker;}
    inline void freeToMarkerUpper(alloc::Marker marker) {this->m_upperTop = marker;}

    void GetInfo(size_t *sizeOfStack, size_t *available, size_t *lowerUsed, size_t *upperUsed);

private:
    char    *m_stack;
    size_t  m_sizeOfStack;
    alloc::Marker  m_lowerTop;
    alloc::Marker  m_upperTop;
};

} // namespace baka


#endif