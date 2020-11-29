#ifndef _BAKA_ALLOC_COMMONS_H_
#define _BAKA_ALLOC_COMMONS_H_

#include <cinttypes>

namespace baka
{
    namespace alloc
    {
        typedef size_t Marker;

        template<typename Alloc, size_t n>
        class GlobalAlloc
        {
        public:
            static Alloc &Get() {
                static Alloc pool = Alloc(n);
                return pool;
            }
        };
    }
}

#endif