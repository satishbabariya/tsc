// Runtime exception handling implementation for TSC
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Simple exception type
typedef struct {
    int32_t type;  // Exception type identifier
    void* data;    // Exception data
} Exception;

// Global exception state
static Exception* current_exception = NULL;

// Runtime functions called by generated LLVM code
void __throw_exception(int64_t exception_value) {
    // Convert the exception value to our exception structure
    Exception* ex = malloc(sizeof(Exception));
    ex->type = 1;  // Generic exception type
    ex->data = (void*)(intptr_t)exception_value;
    
    current_exception = ex;
    
    // For now, just print and exit
    // In a full implementation, this would use setjmp/longjmp or similar
    printf("Exception thrown: %ld\n", exception_value);
    exit(1);
}

void __rethrow_exception() {
    if (current_exception) {
        printf("Exception re-thrown: %ld\n", (int64_t)(intptr_t)current_exception->data);
        exit(1);
    } else {
        printf("No exception to re-throw\n");
        exit(1);
    }
}

// Helper function to check if an exception is currently active
int __has_exception() {
    return current_exception != NULL;
}

// Helper function to get the current exception
int64_t __get_exception() {
    if (current_exception) {
        return (int64_t)(intptr_t)current_exception->data;
    }
    return 0;
}

// Helper function to clear the current exception
void __clear_exception() {
    if (current_exception) {
        free(current_exception);
        current_exception = NULL;
    }
}