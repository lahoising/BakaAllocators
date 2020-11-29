#include <iostream>
#include <baka_pool_allocator.h>

class Something;

class MyGlobalPoolAlloc : 
    public baka::alloc::GlobalAlloc<
        baka::PoolAllocator<Something>, // global allocator of pool allocator of something
        32                              // 32 objects (32 * sizeof(Something) bytes)
    > {};

class Something
{
public:
    Something(uint32_t b)
    {
        this->b = b; 
        printf("create something %u\n", this->b);
        this->yes = true;
    }

    ~Something()
    {
        printf("goobye cruel mem %u\n", this->b); 
        this->yes = false;
    }

    BAKA_POOL_ALLOC_NEW_OVERRIDE(MyGlobalPoolAlloc::Get())
    BAKA_POOL_ALLOC_DELETE_OVERRIDE(MyGlobalPoolAlloc::Get(), Something)

public:
    int a[8];
    uint32_t b;
    bool yes;
};

int main(int argc, char *argv[])
{
    baka::PoolAllocator<Something> pool = baka::PoolAllocator<Something>(16);
    
    Something *a = pool.Alloc();
    *a = Something(1);
    
    Something *b = pool.Alloc();
    *b = Something(2);

    Something *c = pool.Alloc();
    *c = Something(3);

    printf("from a to b: %llu\n", b - a);
    printf("from a to c: %llu\n", c - a);

    c->~Something();

    Something *ab = new Something(5);
    Something *bb = new Something(6);
    Something *cb = new Something(7);
    printf("from ab to cb: %llu\n", cb - ab);

    try
    {
        pool.Free(nullptr);
    } catch(const std::exception &e)
    {
        printf("%s\n", e.what());
    }

    pool.Free(a);
    Something *d = pool.Alloc();
    *d = Something(4);
    printf("from d to c: %llu\n", c - d);

    delete cb;
    delete ab;
    Something *db = new Something(8);
    printf("from db to bb: %llu\n", bb - db);
    
    return 0;
}