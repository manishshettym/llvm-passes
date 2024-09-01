#include <stdio.h>
#include <stdlib.h>

void pointerOperations()
{
    int arr[5] = {1, 2, 3, 4, 5};
    int *ptr = arr;

    // Perform pointer arithmetic
    ptr += 3;
    printf("Pointer Arithmetic Result: %d\n", *ptr);

    int a = 10;
    void *voidPtr = &a;

    // Cast void pointer to int pointer and dereference
    int *intPtr = (int *)voidPtr;
    printf("Pointer Cast Result: %d\n", *intPtr);

    int *rawPtr = (int *)malloc(sizeof(int));
    *rawPtr = 20; // Dereference raw pointer to assign value

    printf("Raw Pointer Dereference Result: %d\n", *rawPtr);
    free(rawPtr);
}

int main()
{
    pointerOperations();
    return 0;
}