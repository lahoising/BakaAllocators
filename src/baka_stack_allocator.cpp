#include <memory>
#include <baka_stack_allocator.h>

namespace baka
{
    
StackAllocator::StackAllocator(size_t size)
{
    this->m_stack = (char*)std::malloc(size);
    this->m_sizeOfStack = size;
    this->m_top = 0;
}

StackAllocator::~StackAllocator()
{
    if(this->m_stack) std::free(this->m_stack);
}

void *StackAllocator::alloc(size_t size)
{
    if(this->m_top + size > this->m_sizeOfStack) return nullptr;

    void *allocation = this->m_stack + this->m_top;
    this->m_top += size;
    return allocation;
}

void StackAllocator::getInfo(size_t *sizeOfStack, size_t *available, size_t *inUse)
{
    if(sizeOfStack) *sizeOfStack = this->m_sizeOfStack;
    if(available) *available = this->m_sizeOfStack - this->m_top;
    if(inUse) *inUse = this->m_top;
}

} // namespace baka
