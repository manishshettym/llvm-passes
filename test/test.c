#include <stdio.h>

// Define a struct with a function pointer
struct Operation
{
    int (*op_func)(int, int);
};

// Basic function declarations
int add(int a, int b)
{
    return a + b;
}

int subtract(int a, int b)
{
    return a - b;
}

// Function using struct and function pointer
int operate(struct Operation *op, int x, int y)
{
    return op->op_func(x, y);
}

// Function to demonstrate recursive calls
int factorial(int n)
{
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

// Function calling other functions
void compute()
{
    struct Operation add_op = {.op_func = add};
    struct Operation sub_op = {.op_func = subtract};

    int a = 10, b = 5;

    // Call with function pointer inside struct
    printf("Addition: %d\n", operate(&add_op, a, b));
    printf("Subtraction: %d\n", operate(&sub_op, a, b));

    // Call to a recursive function
    printf("Factorial: %d\n", factorial(5));
}

// A function that calls another function and uses a conditional
void conditional_call(int condition)
{
    if (condition)
    {
        printf("Condition is true, calling add\n");
        add(1, 2);
    }
    else
    {
        printf("Condition is false, calling subtract\n");
        subtract(1, 2);
    }
}

// Main function that calls multiple other functions
int main()
{
    // Call compute which contains multiple function calls
    compute();

    // Test conditional function call
    conditional_call(1);
    conditional_call(0);

    return 0;
}