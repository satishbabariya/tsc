// Console operations runtime for TSC
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

// Console log function
// This is a variadic function that can accept multiple arguments
void console_log(void* first_arg, ...) {
    va_list args;
    va_start(args, first_arg);
    
    // Skip the first argument (it's always null in our current implementation)
    void* arg = va_arg(args, void*);
    
    // Process all arguments
    bool first = true;
    while (arg != NULL) {
        char* str = (char*)arg;
        if (!first) {
            printf(" ");
        }
        printf("%s", str);
        first = false;
        arg = va_arg(args, void*);
    }
    
    printf("\n");
    va_end(args);
}