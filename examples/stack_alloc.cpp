#include <iostream>
#include <baka_stack_allocator.h>

void print_stack(baka::StackAllocator *stack)
{
    size_t size, available, inUse;
    stack->getInfo(&size, &available, &inUse);
    printf("stack info - size: %zu\tavailable: %zu\tin use: %zu\n", size, available, inUse);
}

struct Person
{
    char *name;
    int age;
    char *vocation;
};

class GlobalStackAlloc
{
public:
    static baka::StackAllocator &Get() { 
        static baka::StackAllocator stack = baka::StackAllocator(4096);
        return stack; 
    }
};

class Something
{
public:
    Something(int id)
    {
        m_id = id;
        printf("hey from something %d!\n", m_id);
    } 

    ~Something()
    {
        printf("bye from something %d\n", m_id);
    }

    BAKA_STACK_ALLOC_NEW_OVERRIDE(GlobalStackAlloc::Get())

private:
    int m_id;
};

void person_greet(Person *person)
{
    printf("Hey! I'm %s, a %d years old %s\n", person->name, person->age, person->vocation);
}

int main(int argc, char *argv[])
{
    baka::StackAllocator stack = baka::StackAllocator(1024);
    print_stack(&stack);

    char *msg = (char*)stack.alloc(16);
    printf("allocate 16 bytes\n");
    print_stack(&stack);
    snprintf(msg, 16, "Hola cómo le va?");
    printf("%s\n", msg);

    baka::alloc::Marker marker = stack.getMarker();
    
    printf("allocate a person (%zu)\n", sizeof Person);
    Person *person = (Person*)stack.alloc(sizeof Person);
    print_stack(&stack);
    
    printf("allocate person name (32 bytes)\n");
    person->name = (char*)stack.alloc(32);
    snprintf(person->name, 32, "Luis Alfredo Chiang");
    print_stack(&stack);
    
    person->age = 123123123;
    
    printf("allocate person vocation (16 bytes)\n");
    person->vocation = (char*)stack.alloc(16);
    snprintf(person->vocation, 16, "student I guess");
    print_stack(&stack);
    
    person_greet(person);
    
    printf("free person, name and vocation\n");
    stack.freeToMarker(marker); // pop person, name and vocation
    print_stack(&stack);

    constexpr uint32_t INT_ARR_SIZE = sizeof(int) * 5;
    printf("allocate int array (%u bytes)\n", INT_ARR_SIZE);
    int *arr = (int*)stack.alloc(INT_ARR_SIZE);
    print_stack(&stack);
    for(int i = 0; i < INT_ARR_SIZE / sizeof(int); i++)
        arr[i] = i * 7;
    for(int i = 0; i < INT_ARR_SIZE / sizeof(int); i++)
        printf("\tarr[%d] = %d\n", i, arr[i]);
    
    printf("clear stack\n");
    stack.clear();
    print_stack(&stack);

    Something *a = new Something(10);
    Something *b = (Something*)stack.alloc(sizeof(Something));
    
    // warning: this will create two objects, copy the first to the second and destroy the first.
    // check move contructor and assignment to resolve that.
    *b = Something(7);  

    return 0;
}