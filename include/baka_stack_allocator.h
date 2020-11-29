#ifndef _BAKA_STACK_ALLOCATOR_H_
#define _BAKA_STACK_ALLOCATOR_H_

#include <cinttypes>
#include <baka_alloc_commons.h>

namespace baka
{

/// @note taken from the book "Game Engine Architecture" by Jason Gregory
class StackAllocator
{
public:
    /// @param size the size of the stack in bytes
    StackAllocator(size_t size);
    ~StackAllocator();

    /**
     * @brief allocate size bytes from the stack
     * @param size the size in bytes to be allocated
     * @return a pointer to the allocated block
     */
    void *alloc(size_t size);

    /**
     * @brief gets the current top of the stack
     * @return the current top of the stack
     */
    inline alloc::Marker getMarker() { return this->m_top; }

    /**
     * @brief free the stack to that marker. Anything after the mark will be considered free.
     * @param marker the point to free to. This will become the new top of the stack.
     * @note does not destroy the objects that are freed.
     */
    inline void freeToMarker(alloc::Marker marker) {this->m_top = marker;}

    /**
     * @brief marks the entire stack as free
     * @note does not destroy the objects that are freed.
     */
    inline void clear() {this->m_top = 0;}

    /**
     * @brief get the request stack info
     * @param sizeOfStack if not null, stores the size in bytes of stack
     * @param available if not null, stores available bytes of stack
     * @param inUse if not null, stores bytes in use of stack
     */
    void getInfo(size_t *sizeOfStack, size_t *available, size_t *inUse);

private:
    char *m_stack;
    alloc::Marker m_top;
    size_t m_sizeOfStack;
};

#define BAKA_STACK_ALLOC_NEW_OVERRIDE(allocator)\
static void *operator new(size_t size) {\
    return allocator.alloc(size);\
}

} // namespace baka

#endif