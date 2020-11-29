#include <iostream>
#include <baka_double_stack_allocator.h>

void printDoubleStack(baka::DoubleStackAllocator *doubleStack)
{
    size_t sizeOfStack, available, upper, lower;
    doubleStack->GetInfo(&sizeOfStack, &available, &lower, &upper);
    printf("double stack - size of stack: %zu\tavailable: %zu\tlower: %zu\tupper: %zu\n", sizeOfStack, available, lower, upper);
}

struct Something
{
    int a, b, c;
};

int main(int argc, char *argv[])
{
    baka::DoubleStackAllocator doubleStack = baka::DoubleStackAllocator(4096);
    printDoubleStack(&doubleStack);

    // =================== LOWER TESTS =================

    std::cout << "allocate msg (8 bytes) on lower stack" << std::endl;
    char *msg = (char*)doubleStack.allocLower(8);
    snprintf(msg, 8, "hola!");
    printDoubleStack(&doubleStack);
    std::cout << msg << std::endl;

    baka::alloc::Marker marker = doubleStack.getMarkerLower();

    printf("allocate something (%zu bytes) on lower stack\n", sizeof(Something));
    Something *some = (Something*)doubleStack.allocLower( sizeof(Something));
    printDoubleStack(&doubleStack);

    constexpr size_t INT_ARR_SIZE = sizeof(int) * 4;
    printf("allocate int arr (%zu bytes) on lower stack\n", INT_ARR_SIZE);
    int *arr = (int*)doubleStack.allocLower(INT_ARR_SIZE);
    printDoubleStack(&doubleStack);

    printf("pop int arr and something (%zu + %zu = %zu bytes)\n", INT_ARR_SIZE, sizeof(Something), INT_ARR_SIZE + sizeof(Something));
    doubleStack.freeToMarkerLower(marker);
    printDoubleStack(&doubleStack);

    // ====================== UPPER TESTS ====================

    marker = doubleStack.getMarkerUpper();

    printf("allocate something (%zu bytes) on Upper stack\n", sizeof(Something));
    some = (Something*)doubleStack.allocUpper(sizeof(Something));
    printDoubleStack(&doubleStack);

    printf("allocate int arr (%zu bytes) on Upper stack\n", INT_ARR_SIZE);
    arr = (int*)doubleStack.allocUpper(INT_ARR_SIZE);
    printDoubleStack(&doubleStack);

    printf("pop int arr and something (%zu + %zu = %zu bytes)\n", INT_ARR_SIZE, sizeof(Something), INT_ARR_SIZE + sizeof(Something));
    doubleStack.freeToMarkerUpper(marker);
    printDoubleStack(&doubleStack);

    std::cout << "allocate msg (8 bytes) on Upper stack" << std::endl;
    msg = (char*)doubleStack.allocUpper(8);
    snprintf(msg, 8, "buenas!");
    printDoubleStack(&doubleStack);
    std::cout << msg << std::endl;

    // ================= CLEAR TEST ==============
    printf("clear lower stack\n");
    doubleStack.clearLower();
    printDoubleStack(&doubleStack);
    
    printf("clear Upper stack\n");
    doubleStack.clearUpper();
    printDoubleStack(&doubleStack);

    return 0;
}