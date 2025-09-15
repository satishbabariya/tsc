// Console operations runtime for TSC
#include <stdio.h>
#include <stdarg.h>

// Console log function
// This is a variadic function that can accept multiple arguments
void console_log(void* first_arg, ...) {
    va_list args;
    va_start(args, first_arg);
    
    // Skip the first argument (it's always null in our current implementation)
    void* arg = va_arg(args, void*);
    
    // Process the actual string argument
    if (arg != NULL) {
        char* str = (char*)arg;
        printf("%s\n", str);
    } else {
        printf("(null)\n");
    }
    
    va_end(args);
}