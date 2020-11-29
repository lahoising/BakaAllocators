#include <iostream>
#include <baka_pool_allocator.h>

class Something
{
public:
    Something(){printf("create something\n");}
    ~Something(){printf("goobye cruel mem\n");}

public:
    int a[8];
    uint32_t b;
    bool yes;
};

int main(int argc, char *argv[])
{
    baka::PoolAllocator<Something> pool = baka::PoolAllocator<Something>(16);
    
    Something *a = pool.Alloc();
    *a = Something();
    a->b = 1;
    a->yes = true;
    
    Something *b = pool.Alloc();
    *b = Something();
    b->b = 2;
    b->yes = false;

    Something *c = pool.Alloc();
    *c = Something();
    c->b = 42;
    c->yes = a->yes & b->yes;

    printf("from a to b: %llu\n", b - a);
    printf("from a to c: %llu\n", c - a);

    c->~Something();
    
    return 0;
}