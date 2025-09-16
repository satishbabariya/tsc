// Console operations runtime for TSC
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

// Internal print function for static compiler
// This is a variadic function that can accept multiple arguments
void _print(void* first_arg, ...) {
    // For now, just print the first argument
    // TODO: Implement proper variadic argument handling
    char* str = (char*)first_arg;
    printf("%s\n", str);
}