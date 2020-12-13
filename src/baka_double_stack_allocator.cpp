#include <cstdlib>
#include <baka_double_stack_allocator.h>

namespace baka
{
    
DoubleStackAllocator::DoubleStackAllocator(size_t size)
{
    this->m_stack = (char*)std::malloc(size);
    this->m_sizeOfStack = size;
    this->m_lowerTop = 0;
    this->m_upperTop = size;
}

DoubleStackAllocator::~DoubleStackAllocator()
{
    if(this->m_stack) std::free(this->m_stack);
}

void *DoubleStackAllocator::allocLower(size_t size)
{
    if(this->m_upperTop - this->m_lowerTop + size < 0) return nullptr;
    void *allocation = this->m_stack + this->m_lowerTop;
    this->m_lowerTop += size;
    return allocation;
}

void *DoubleStackAllocator::allocUpper(size_t size)
{
    if(this->m_upperTop - this->m_lowerTop + size < 0) return nullptr;
    void *allocation = this->m_stack + this->m_upperTop - size;
    this->m_upperTop -= size;
    return allocation;
}

void DoubleStackAllocator::GetInfo(size_t *sizeOfStack, size_t *available, size_t *lowerUsed, size_t *upperUsed)
{
    if(sizeOfStack) *sizeOfStack = this->m_sizeOfStack;
    if(available) *available = this->m_upperTop - this->m_lowerTop;
    if(lowerUsed) *lowerUsed = this->m_lowerTop;
    if(upperUsed) *upperUsed = this->m_sizeOfStack - this->m_upperTop;
}

} // namespace baka
